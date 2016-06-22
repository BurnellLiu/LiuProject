
#include <cmath>

#include <vector>
using std::vector;

#include "LKDTree.h"
#include "LDataStruct/LOrderedList.h"



/// @brief KD���ڵ�
struct LKDTreeNode
{
    enum
    {
        UNDEFINE_SPLIT = -1 // ��ʾδ����ķָ����
    };
    int Split; ///< ��ֱ�ڷָ��ķ��������(���ֵΪUNDEFINE_SPLIT, ��ʾ�ýڵ�ΪҶ�ӽڵ�)
    unsigned int DataIndex; ///< �ڵ����ݵ�����
    LKDTreeNode* Parent; ///< ���ڵ�
    LKDTreeNode* LeftChildren; ///< ���ӽڵ�
    LKDTreeNode* RightChildren; ///< �Һ��� �ڵ�
};

/// @brief KD���ڵ���Ŀ���ľ���
struct LKDTreeNodeDistance
{
    unsigned int DataIndex; ///< ��������
    float Distance; ///< ����ֵ

    bool operator < (IN const LKDTreeNodeDistance& B) const
    {
        if (this->Distance < B.Distance)
            return true;
        else
            return false;
    }
};

typedef vector<LKDTreeNode*> LKDTreeNodeList; ///< ���ڵ��б�

/// @brief KD��
class CKDTree
{
public:
    /// @brief ���캯��
    CKDTree()
    {
        this->m_pRootNode = 0;
    }

    /// @brief ��������
    ~CKDTree()
    {
        this->ClearTree(m_pRootNode);
    }

    /// @brief ����KD��
    void BuildTree(IN const LKDTreeMatrix& dataSet)
    {
        // ������
        if (this->m_pRootNode != 0)
            this->ClearTree(this->m_pRootNode);

        // ������ݼ�
        if (dataSet.RowLen < 1 || dataSet.ColumnLen < 1)
            return;

        // �������ݼ�
        this->m_dataSet = dataSet;

        // �ݹ鹹����
        this->m_pRootNode = new LKDTreeNode();
        vector<unsigned int> dataIndexList(dataSet.RowLen);
        for (unsigned int i = 0; i < dataIndexList.size(); i++)
        {
            dataIndexList[i] = i;
        }
        this->CreateTree(0, this->m_pRootNode, dataIndexList);
    }

    /// @brief �����ݼ���������ָ���������ڽ�����������
    int SearchNearestNeighbor(IN const LKDTreeMatrix& data)
    {
        LKDTreeList indexList;
        bool bRet = this->SearchKNearestNeighbors(data, 1, indexList);
        if (!bRet)
        {
            return -1;
        }

        return indexList[0][0];
    }

    /// @brief �����ݼ���������ָ���������ڽ���K����������
    bool SearchKNearestNeighbors(IN const LKDTreeMatrix& data, IN unsigned int k, OUT LKDTreeList& indexList)
    {
        // ������
        if (data.RowLen != 1 || data.ColumnLen != m_dataSet.ColumnLen)
            return false;
        if (k < 1)
            return false;

        LKDTreeNodeList searchPath;
        this->SearchTree(data, searchPath);
        if (searchPath.size() < 1)
            return false;

        LOrderedList<LKDTreeNodeDistance> nearestDistanceList;

        // ������·���в����Ƿ���ڱȵ�ǰ����㻹���ĵ�
        for (int i = (int)searchPath.size() - 1; i >= 0; i--)
        {
            LKDTreeNode* node = searchPath[i];

            if (nearestDistanceList.Size() < k)
            {
                LKDTreeNodeList nodeList;
                this->TraverseTree(node, nodeList);
                if (nodeList.size() >= k)
                {
                    LKDTreeNodeDistance nodeDistance;
                    for (unsigned int n = 0; n < nodeList.size(); n++)
                    {
                        nodeDistance.DataIndex = nodeList[n]->DataIndex;
                        nodeDistance.Distance = this->CalculateDistance(data, nodeDistance.DataIndex);
                        nearestDistanceList.Insert(nodeDistance);
                    }
                }
                continue;
            }

            while (nearestDistanceList.Size() > k)
            {
                nearestDistanceList.PopBack();
            }


            LKDTreeNodeDistance currentNearestDistanceMax = nearestDistanceList.End()->Data;
            // ��Ŀ���͵�ǰ�����ľ���Ϊ�뾶����Բ����ͷָ���ཻ�������ָ������һ������
            float dif = data[0][node->Split] - m_dataSet[node->DataIndex][node->Split];
            dif = abs(dif);
            if (dif >= currentNearestDistanceMax.Distance)
            {
                continue;
            }

            LKDTreeNodeList nodeList;
            nodeList.push_back(node);
            if (m_dataSet[currentNearestDistanceMax.DataIndex][node->Split] < m_dataSet[node->DataIndex][node->Split])
            {
                this->TraverseTree(node->RightChildren, nodeList);
            }
            else if (m_dataSet[currentNearestDistanceMax.DataIndex][node->Split] > m_dataSet[node->DataIndex][node->Split])
            {
                this->TraverseTree(node->LeftChildren, nodeList);
            }
            else
            {
                this->TraverseTree(node->RightChildren, nodeList);
                this->TraverseTree(node->LeftChildren, nodeList);
            }

            for (unsigned int n = 0; n < nodeList.size(); n++)
            {
                float distance = this->CalculateDistance(data, nodeList[n]->DataIndex);
                LKDTreeNodeDistance nodeDistance;
                if (distance < currentNearestDistanceMax.Distance)
                {
                    nodeDistance.Distance = distance;
                    nodeDistance.DataIndex = nodeList[n]->DataIndex;
                    nearestDistanceList.Insert(nodeDistance);
                    nearestDistanceList.PopBack();
                    currentNearestDistanceMax = nearestDistanceList.End()->Data;
                }
            }


        }

        if (nearestDistanceList.Size() < k)
            return false;


        indexList.Reset(1, k, -1);

        const LOrderedListNode<LKDTreeNodeDistance>* pCurrentNode = nearestDistanceList.Begin();

        int col = 0;
        while (pCurrentNode)
        {
            indexList[0][col] = pCurrentNode->Data.DataIndex;
            col++;

            pCurrentNode = pCurrentNode->PNext;
        }

        return true;
    }

private:
    /// @brief ����KD��
    /// @param[in] pParent ���ڵ�
    /// @param[in] pNode ��ǰ�ڵ�
    /// @param[in] dataIndexList ���������б�
    void CreateTree(
        IN LKDTreeNode* pParent,
        IN LKDTreeNode* pNode,
        IN const vector<unsigned int>& dataIndexList)
    {
        if (pNode == 0)
            return;

        pNode->Parent = pParent;

        // ֻʣһ������, ��ΪҶ�ӽڵ�
        if (dataIndexList.size() == 1)
        {
            pNode->DataIndex = dataIndexList[0];
            pNode->Split = LKDTreeNode::UNDEFINE_SPLIT;
            pNode->LeftChildren = 0;
            pNode->RightChildren = 0;

            return;
        }

        unsigned int bestColIndex = 0; // �����󷽲��ά������
        unsigned int midDataIndex = 0; // �����ѷָ�������

        this->FindMaxVarianceColumn(dataIndexList, bestColIndex);

        this->FindMidValueOnColumn(dataIndexList, bestColIndex, midDataIndex);

        pNode->Split = bestColIndex;
        pNode->DataIndex = midDataIndex;

        // �����ݷ�Ϊ����������
        vector<unsigned int> leftDataIndexList;
        leftDataIndexList.reserve(dataIndexList.size() * 2 / 3); // Ԥ�ȷ�����ڴ�, ��ֹ��push_back�����ж���ظ��������Ч��
        vector<unsigned int> rightDataIndexList;
        rightDataIndexList.reserve(dataIndexList.size() * 2 / 3); // Ԥ�ȷ�����ڴ�, ��ֹ��push_back�����ж���ظ��������Ч��
        for (unsigned int i = 0; i < dataIndexList.size(); i++)
        {
            unsigned int m = dataIndexList[i];
            if (m == midDataIndex)
                continue;

            if (this->m_dataSet[m][bestColIndex] <= this->m_dataSet[midDataIndex][bestColIndex])
                leftDataIndexList.push_back(dataIndexList[i]);
            else
                rightDataIndexList.push_back(dataIndexList[i]);
        }

        // ����������
        if (leftDataIndexList.size() == 0)
        {
            pNode->LeftChildren = 0;
        }
        if (leftDataIndexList.size() != 0)
        {
            pNode->LeftChildren = new LKDTreeNode;
            this->CreateTree(pNode, pNode->LeftChildren, leftDataIndexList);
        }

        // ����������
        if (rightDataIndexList.size() == 0)
        {
            pNode->RightChildren = 0;
        }
        if (rightDataIndexList.size() != 0)
        {
            pNode->RightChildren = new LKDTreeNode;
            this->CreateTree(pNode, pNode->RightChildren, rightDataIndexList);
        }
    }

    /// @brief ��ָ�������ݼ���, �ҳ���󷽲���
    /// @param[in] dataIndexList ���������б�, Ҫ������Ҫ����������
    /// @param[out] col �洢������
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool FindMaxVarianceColumn(IN const vector<unsigned int>& dataIndexList, OUT unsigned int& col)
    {
        if (dataIndexList.size() < 2)
            return false;


        // �ҳ�������󷽲��ά��
        float maxVariance = 0.0f; // �������ά���ϵ����ݷ�������ֵ
        unsigned int bestCol = 0; // �����󷽲��ά������
        for (unsigned int n = 0; n < this->m_dataSet.ColumnLen; n++)
        {
            float sumValue = 0.0f; // ָ���е����ݺ�
            for (unsigned int i = 0; i < dataIndexList.size(); i++)
            {
                unsigned int m = dataIndexList[i];
                sumValue += this->m_dataSet[m][n];
            }

            float averageValue = sumValue / (float)dataIndexList.size(); // ����ָ���е�ƽ��ֵ

            float variance = 0.0f; // ָ���еķ���ֵ
            for (unsigned int i = 0; i < dataIndexList.size(); i++)
            {
                unsigned int m = dataIndexList[i];
                float dif = averageValue - this->m_dataSet[m][n];
                variance += dif * dif;
            }
            variance = variance / (float)dataIndexList.size();

            if (variance > maxVariance)
            {
                maxVariance = variance;
                bestCol = n;
            }
        }

        col = bestCol;

        return true;
    }

    /// @brief ��ָ�������ݼ���, �ҳ�ָ���е���ֵ��������(���ƽ����)
    /// @param[in] dataIndexList ���������б�, Ҫ������Ҫ����������
    /// @param[in] col ������
    /// @param[out] dataIndex �洢��������
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool FindMidValueOnColumn(IN const vector<unsigned int>& dataIndexList, IN unsigned int col, OUT unsigned int& dataIndex)
    {
        if (dataIndexList.size() < 1)
            return false;


        float sum = 0.0f;
        for (unsigned int i = 0; i < dataIndexList.size(); i++)
        {
            unsigned int m = dataIndexList[i];

            sum += m_dataSet[m][col];
        }

        float avg = sum / dataIndexList.size();

        unsigned int midDataIndex = dataIndexList[0];
        float miniDif = abs(m_dataSet[midDataIndex][col] - avg);

        for (unsigned int i = 0; i < dataIndexList.size(); i++)
        {
            unsigned int m = dataIndexList[i];

            float dif = abs(m_dataSet[m][col] - avg);
            if (dif < miniDif)
            {
                miniDif = dif;
                midDataIndex = m;
            }

        }

        dataIndex = midDataIndex;

        return true;
    }

    /// @brief ������
    /// @param[in] pNode ���ڵ�
    /// @param[out] nodeList ���������Ľڵ��б�
    void TraverseTree(IN LKDTreeNode* pNode, OUT LKDTreeNodeList& nodeList)
    {
        if (pNode == 0)
            return;

        nodeList.push_back(pNode);

        this->TraverseTree(pNode->LeftChildren, nodeList);
        this->TraverseTree(pNode->RightChildren, nodeList);
    }

    /// @brief ������
    /// @param[in] data Դ����
    /// @param[out] searchPath ��������·��
    void SearchTree(IN const LKDTreeMatrix& data, OUT LKDTreeNodeList& searchPath)
    {
        searchPath.clear();

        LKDTreeNode* currentNode = m_pRootNode;
        while (currentNode != 0)
        {
            searchPath.push_back(currentNode);

            // ����Ҷ�ӽڵ�, ��������
            if (currentNode->Split == LKDTreeNode::UNDEFINE_SPLIT)
            {
                currentNode = 0;
                continue;
            }

            float splitData = m_dataSet[currentNode->DataIndex][currentNode->Split];
            if (data[0][currentNode->Split] <= splitData)
            {
                currentNode = currentNode->LeftChildren;
                continue;
            }
            if (data[0][currentNode->Split] > splitData)
            {
                currentNode = currentNode->RightChildren;
                continue;
            }

        }
    }

    /// @brief ����ָ�����������ݼ���һ�����ݵľ���ֵ
    /// @param[in] data ָ��������
    /// @param[in] index ���ݼ��е���������
    /// @return ���ؾ���ֵ(ŷ����þ���), ʹ��ǰ�뱣֤������ȷ
    float CalculateDistance(IN const LKDTreeMatrix& data, IN unsigned int index)
    {
        float sqrSum = 0.0f;
        for (unsigned int i = 0; i < data.ColumnLen; i++)
        {
            float dif = data[0][i] - m_dataSet[index][i];
            sqrSum += dif * dif;
        }
        sqrSum = sqrtf(sqrSum);

        return sqrSum;
    }

    /// @brief ������
    /// @param[in] pNode ��Ҫ����Ľڵ�
    void ClearTree(IN LKDTreeNode*& pNode)
    {
        if (pNode == 0)
            return;

        this->ClearTree(pNode->LeftChildren);
        this->ClearTree(pNode->RightChildren);

        delete pNode;
        pNode = 0;
    }

private:
    LKDTreeNode* m_pRootNode; ///< ���ڵ�
    LKDTreeMatrix m_dataSet; ///< ���ݼ�
};


LKDTree::LKDTree()
    : m_pKDTree(0)
{
    m_pKDTree = new CKDTree();
}

LKDTree::~LKDTree()
{
    if (m_pKDTree != 0)
    {
        delete m_pKDTree;
        m_pKDTree = 0;
    }
}

void LKDTree::BuildTree(IN const LKDTreeMatrix& dataSet)
{
    m_pKDTree->BuildTree(dataSet);
}

int LKDTree::SearchNearestNeighbor(IN const LKDTreeMatrix& data)
{
    return m_pKDTree->SearchNearestNeighbor(data);
}

bool LKDTree::SearchKNearestNeighbors(IN const LKDTreeMatrix& data, IN unsigned int k, OUT LKDTreeList& indexList)
{
    return m_pKDTree->SearchKNearestNeighbors(data, k, indexList);
}