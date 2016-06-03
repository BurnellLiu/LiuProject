
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
    CKDTree();

    /// @brief ��������
    ~CKDTree();

    /// @brief ����KD��
    void BuildTree(IN const LKDTreeMatrix& dataSet);

    /// @brief �����ݼ���������ָ���������ڽ�����������
    int SearchNearestNeighbor(IN const LKDTreeMatrix& data);

    /// @brief �����ݼ���������ָ���������ڽ���K����������
    int SearchKNearestNeighbors(IN const LKDTreeMatrix& data, IN unsigned int k, OUT LKDTreeList& indexList);

private:
    /// @brief ����KD��
    /// @param[in] pParent ���ڵ�
    /// @param[in] pNode ��ǰ�ڵ�
    /// @param[in] dataIndexList ���������б�
    void BuildTree(
        IN LKDTreeNode* pParent,
        IN LKDTreeNode* pNode,
        IN const vector<unsigned int>& dataIndexList);

    /// @brief ������
    /// @param[in] pNode ���ڵ�
    /// @param[out] nodeList ���������Ľڵ��б�
    void TraverseTree(IN LKDTreeNode* pNode, OUT LKDTreeNodeList& nodeList);

    /// @brief ������
    /// @param[in] data Դ����
    /// @param[out] searchPath ��������·��
    void SearchTree(IN const LKDTreeMatrix& data, OUT LKDTreeNodeList& searchPath);

    /// @brief ����ָ�����������ݼ���һ�����ݵľ���ֵ
    /// @param[in] data ָ��������
    /// @param[in] index ���ݼ��е���������
    /// @return ���ؾ���ֵ(ŷ����þ���), ʹ��ǰ�뱣֤������ȷ
    float CalculateDistance(IN const LKDTreeMatrix& data, IN unsigned int index);

    /// @brief ������
    /// @param[in] pNode ��Ҫ����Ľڵ�
    void ClearTree(IN LKDTreeNode*& pNode);
private:
    LKDTreeNode* m_pRootNode; ///< ���ڵ�
    LKDTreeMatrix m_dataSet; ///< ���ݼ�
};


CKDTree::CKDTree()
{
    this->m_pRootNode = 0;
}

CKDTree::~CKDTree()
{
    this->ClearTree(m_pRootNode);
}

void CKDTree::BuildTree(IN const LKDTreeMatrix& dataSet)
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
    this->BuildTree(0, this->m_pRootNode, dataIndexList);
}

int CKDTree::SearchNearestNeighbor(IN const LKDTreeMatrix& data)
{
    const int ERROR_RET = -1; // ���󷵻�

    // ������
    if (data.RowLen != 1 || data.ColumnLen != m_dataSet.ColumnLen)
        return ERROR_RET;

    // ��ȡ����·��
    LKDTreeNodeList searchPath;
    this->SearchTree(data, searchPath);

    if (searchPath.size() < 1)
        return ERROR_RET;

    // ��������·���е����һ��Ϊ��ǰ�����
    LKDTreeNode* currentNearestNode = searchPath[searchPath.size()-1];
    float currentNearestDistance = this->CalculateDistance(data, currentNearestNode->DataIndex);


    // ������·���в����Ƿ���ڱȵ�ǰ����㻹���ĵ�
    for (int i = (int)searchPath.size() - 1; i >=0; i--)
    {
        LKDTreeNode* node = searchPath[i];

        // Ҷ�ӽڵ�ֱ������, ֻ�����һ�����п���ΪҶ�ӽڵ�
        if (node->Split == LKDTreeNode::UNDEFINE_SPLIT)
            continue;

        // ��Ŀ���͵�ǰ�����ľ���Ϊ�뾶����Բ����ͷָ���ཻ�������ָ������һ������
        float dif = data[0][node->Split] - m_dataSet[node->DataIndex][node->Split];
        dif = abs(dif);
        if (dif >= currentNearestDistance)
        {
            continue;
        }

        LKDTreeNodeList nodeList;
        nodeList.push_back(node);
        if (m_dataSet[currentNearestNode->DataIndex][node->Split] < m_dataSet[node->DataIndex][node->Split])
        {
            this->TraverseTree(node->RightChildren, nodeList);
        }
        else if (m_dataSet[currentNearestNode->DataIndex][node->Split] > m_dataSet[node->DataIndex][node->Split])
        {
            this->TraverseTree(node->LeftChildren, nodeList);
        }
        else
        {
            this->TraverseTree(node->RightChildren, nodeList);
            this->TraverseTree(node->LeftChildren, nodeList);
        }

        for (unsigned int k = 0; k < nodeList.size(); k++)
        {
            float distance = this->CalculateDistance(data, nodeList[k]->DataIndex);
            if (distance < currentNearestDistance)
            {
                currentNearestDistance = distance;
                currentNearestNode = nodeList[k];
            }
        }


    }

    return (int)currentNearestNode->DataIndex;
}

int CKDTree::SearchKNearestNeighbors(IN const LKDTreeMatrix& data, IN unsigned int k, OUT LKDTreeList& indexList)
{
    const int ERROR_RET = -1; // ���󷵻�

    // ������
    if (data.RowLen != 1 || data.ColumnLen != m_dataSet.ColumnLen)
        return ERROR_RET;
    if (k < 1)
        return ERROR_RET;

    LKDTreeNodeList searchPath;
    this->SearchTree(data, searchPath);
    if (searchPath.size() < 1)
        return ERROR_RET;

    LOrderedList<LKDTreeNodeDistance> nearestNodesDistanceList;

    // ������·���в����Ƿ���ڱȵ�ǰ����㻹���ĵ�
    for (int i = (int)searchPath.size() - 1; i >=0; i--)
    {
        LKDTreeNode* node = searchPath[i];

        if (nearestNodesDistanceList.Size() < k)
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
                    nearestNodesDistanceList.Insert(nodeDistance);
                }
            }
            continue;
        }

        while (nearestNodesDistanceList.Size() > k)
        {
            nearestNodesDistanceList.PopBack();
        }


        LKDTreeNodeDistance currentNearestDistanceMax = nearestNodesDistanceList.End()->Data;
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
                nearestNodesDistanceList.Insert(nodeDistance);
                nearestNodesDistanceList.PopBack();
                currentNearestDistanceMax = nearestNodesDistanceList.End()->Data;
            }
        }


    }

    if (nearestNodesDistanceList.Size() < k)
        return ERROR_RET;

    
    indexList.Reset(1, k, -1);

    const LOrderedListNode<LKDTreeNodeDistance>* pCurrentNode = nearestNodesDistanceList.Begin();

    int col = 0;
    while (pCurrentNode)
    {
        indexList[0][col] = pCurrentNode->Data.DataIndex;
        col++;

        pCurrentNode = pCurrentNode->PNext;
    }

    return k;
}

void CKDTree::BuildTree(
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

    // �ҳ�������󷽲��ά��
    float maxDispersion = 0.0f; // �������ά���ϵ����ݷ�������ֵ
    unsigned int bestColIndex = 0; // �����󷽲��ά������
    unsigned int bestListIndex = 0; // �����ѷָ���������б�����
    for (unsigned int n = 0; n < this->m_dataSet.ColumnLen; n++)
    {
        float sumValue = 0.0f; // ָ���е����ݺ�
        for (unsigned int i = 0; i < dataIndexList.size(); i++)
        {
            unsigned int m = dataIndexList[i];
            sumValue += this->m_dataSet[m][n];
        }

        float averageValue = sumValue/(float)dataIndexList.size(); // ����ָ���е�ƽ��ֵ

        float dispersion = 0.0f; // ָ���еķ���ֵ
        float minDif = abs(averageValue-this->m_dataSet[dataIndexList[0]][n]);
        unsigned int listIndex = 0;
        for (unsigned int i = 0; i < dataIndexList.size(); i++)
        {
            unsigned int m = dataIndexList[i];
            float dif = averageValue - this->m_dataSet[m][n];
            dispersion += dif * dif;

            if (abs(dif) < minDif)
            {
                minDif = abs(dif);
                listIndex = i;
            }
        }
        dispersion = dispersion/(float)dataIndexList.size();

        if (dispersion > maxDispersion)
        {
            maxDispersion = dispersion;
            bestColIndex = n;
            bestListIndex = listIndex;
        }
    }

    pNode->Split = bestColIndex;
    pNode->DataIndex = dataIndexList[bestListIndex];

    // �����ݷ�Ϊ����������
    vector<unsigned int> leftDataIndexList;
	leftDataIndexList.reserve(dataIndexList.size() * 2 / 3); // Ԥ�ȷ�����ڴ�, ��ֹ��push_back�����ж���ظ��������Ч��
    vector<unsigned int> rightDataIndexList;
	rightDataIndexList.reserve(dataIndexList.size() * 2 / 3); // Ԥ�ȷ�����ڴ�, ��ֹ��push_back�����ж���ظ��������Ч��
    for (unsigned int i = 0; i < dataIndexList.size(); i++)
    {
        if (i == bestListIndex)
            continue;

        unsigned int mid = dataIndexList[bestListIndex];
        unsigned int m = dataIndexList[i];
        if (this->m_dataSet[m][bestColIndex] <= this->m_dataSet[mid][bestColIndex])
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
        this->BuildTree(pNode, pNode->LeftChildren, leftDataIndexList);
    }

    // ����������
    if (rightDataIndexList.size() == 0)
    {
        pNode->RightChildren = 0;
    }
    if (rightDataIndexList.size() != 0)
    {
        pNode->RightChildren = new LKDTreeNode;
        this->BuildTree(pNode, pNode->RightChildren, rightDataIndexList);
    }
}

void CKDTree::TraverseTree(IN LKDTreeNode* pNode, OUT LKDTreeNodeList& nodeList)
{
    if (pNode == 0)
        return;

    nodeList.push_back(pNode);

    this->TraverseTree(pNode->LeftChildren, nodeList);
    this->TraverseTree(pNode->RightChildren, nodeList);
}

void CKDTree::SearchTree(IN const LKDTreeMatrix& data, OUT LKDTreeNodeList& searchPath)
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

float CKDTree::CalculateDistance(IN const LKDTreeMatrix& data, IN unsigned int index)
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

void CKDTree::ClearTree(IN LKDTreeNode*& pNode)
{
    if (pNode == 0)
        return;

    this->ClearTree(pNode->LeftChildren);
    this->ClearTree(pNode->RightChildren);

    delete pNode;
    pNode = 0;
}

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

int LKDTree::SearchKNearestNeighbors(IN const LKDTreeMatrix& data, IN unsigned int k, OUT LKDTreeList& indexList)
{
    return m_pKDTree->SearchKNearestNeighbors(data, k, indexList);
}