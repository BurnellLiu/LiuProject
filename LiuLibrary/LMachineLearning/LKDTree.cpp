
#include <cmath>

#include <vector>
using std::vector;

#include "LKDTree.h"
#include "LDataStruct/LOrderedList.h"



/// @brief KD树节点
struct LKDTreeNode
{
    enum
    {
        UNDEFINE_SPLIT = -1 // 表示未定义的分割序号
    };
    int Split; ///< 垂直于分割超面的方向轴序号(如果值为UNDEFINE_SPLIT, 表示该节点为叶子节点)
    unsigned int DataIndex; ///< 节点数据的索引
    LKDTreeNode* Parent; ///< 父节点
    LKDTreeNode* LeftChildren; ///< 左孩子节点
    LKDTreeNode* RightChildren; ///< 右孩子 节点
};

/// @brief KD树节点与目标点的距离
struct LKDTreeNodeDistance
{
    unsigned int DataIndex; ///< 数据索引
    float Distance; ///< 距离值

    bool operator < (IN const LKDTreeNodeDistance& B) const
    {
        if (this->Distance < B.Distance)
            return true;
        else
            return false;
    }
};

typedef vector<LKDTreeNode*> LKDTreeNodeList; ///< 树节点列表

/// @brief KD树
class CKDTree
{
public:
    /// @brief 构造函数
    CKDTree();

    /// @brief 析构函数
    ~CKDTree();

    /// @brief 构造KD树
    void BuildTree(IN const LKDTreeMatrix& dataSet);

    /// @brief 在数据集中搜索与指定数据最邻近的数据索引
    int SearchNearestNeighbor(IN const LKDTreeMatrix& data);

    /// @brief 在数据集中搜索与指定数据最邻近的K个数据索引
    int SearchKNearestNeighbors(IN const LKDTreeMatrix& data, IN unsigned int k, OUT LKDTreeList& indexList);

private:
    /// @brief 构造KD树
    /// @param[in] pParent 父节点
    /// @param[in] pNode 当前节点
    /// @param[in] dataIndexList 数据索引列表
    void BuildTree(
        IN LKDTreeNode* pParent,
        IN LKDTreeNode* pNode,
        IN const vector<unsigned int>& dataIndexList);

    /// @brief 遍历树
    /// @param[in] pNode 树节点
    /// @param[out] nodeList 遍历出来的节点列表
    void TraverseTree(IN LKDTreeNode* pNode, OUT LKDTreeNodeList& nodeList);

    /// @brief 搜索树
    /// @param[in] data 源数据
    /// @param[out] searchPath 搜索出的路径
    void SearchTree(IN const LKDTreeMatrix& data, OUT LKDTreeNodeList& searchPath);

    /// @brief 计算指定数据与数据集中一个数据的距离值
    /// @param[in] data 指定的数据
    /// @param[in] index 数据集中的数据索引
    /// @return 返回距离值(欧几里得距离), 使用前请保证参数正确
    float CalculateDistance(IN const LKDTreeMatrix& data, IN unsigned int index);

    /// @brief 清理树
    /// @param[in] pNode 需要清理的节点
    void ClearTree(IN LKDTreeNode*& pNode);
private:
    LKDTreeNode* m_pRootNode; ///< 根节点
    LKDTreeMatrix m_dataSet; ///< 数据集
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
    // 清理树
    if (this->m_pRootNode != 0)
        this->ClearTree(this->m_pRootNode);

    // 检查数据集
    if (dataSet.RowLen < 1 || dataSet.ColumnLen < 1)
        return;

    // 复制数据集
    this->m_dataSet = dataSet;

    // 递归构建树
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
    const int ERROR_RET = -1; // 错误返回

    // 检查参数
    if (data.RowLen != 1 || data.ColumnLen != m_dataSet.ColumnLen)
        return ERROR_RET;

    // 获取搜索路径
    LKDTreeNodeList searchPath;
    this->SearchTree(data, searchPath);

    if (searchPath.size() < 1)
        return ERROR_RET;

    // 假设搜索路径中的最后一项为当前最近点
    LKDTreeNode* currentNearestNode = searchPath[searchPath.size()-1];
    float currentNearestDistance = this->CalculateDistance(data, currentNearestNode->DataIndex);


    // 在搜索路径中查找是否存在比当前最近点还近的点
    for (int i = (int)searchPath.size() - 1; i >=0; i--)
    {
        LKDTreeNode* node = searchPath[i];

        // 叶子节点直接跳过, 只有最后一个点有可能为叶子节点
        if (node->Split == LKDTreeNode::UNDEFINE_SPLIT)
            continue;

        // 以目标点和当前最近点的距离为半径作的圆如果和分割超面相交则搜索分割面的另一边区域
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
    const int ERROR_RET = -1; // 错误返回

    // 检查参数
    if (data.RowLen != 1 || data.ColumnLen != m_dataSet.ColumnLen)
        return ERROR_RET;
    if (k < 1)
        return ERROR_RET;

    LKDTreeNodeList searchPath;
    this->SearchTree(data, searchPath);
    if (searchPath.size() < 1)
        return ERROR_RET;

    LOrderedList<LKDTreeNodeDistance> nearestNodesDistanceList;

    // 在搜索路径中查找是否存在比当前最近点还近的点
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
        // 以目标点和当前最近点的距离为半径作的圆如果和分割超面相交则搜索分割面的另一边区域
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

    // 只剩一个数据, 则为叶子节点
    if (dataIndexList.size() == 1)
    {
        pNode->DataIndex = dataIndexList[0];
        pNode->Split = LKDTreeNode::UNDEFINE_SPLIT;
        pNode->LeftChildren = 0;
        pNode->RightChildren = 0;

        return;
    }

    // 找出具有最大方差的维度
    float maxDispersion = 0.0f; // 标记所有维度上的数据方差的最大值
    unsigned int bestColIndex = 0; // 标记最大方差的维度索引
    unsigned int bestListIndex = 0; // 标记最佳分割点索引的列表索引
    for (unsigned int n = 0; n < this->m_dataSet.ColumnLen; n++)
    {
        float sumValue = 0.0f; // 指定列的数据和
        for (unsigned int i = 0; i < dataIndexList.size(); i++)
        {
            unsigned int m = dataIndexList[i];
            sumValue += this->m_dataSet[m][n];
        }

        float averageValue = sumValue/(float)dataIndexList.size(); // 计算指定列的平均值

        float dispersion = 0.0f; // 指定列的方差值
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

    // 将数据分为左右两部分
    vector<unsigned int> leftDataIndexList;
	leftDataIndexList.reserve(dataIndexList.size() * 2 / 3); // 预先分配好内存, 防止在push_back过程中多次重复分配提高效率
    vector<unsigned int> rightDataIndexList;
	rightDataIndexList.reserve(dataIndexList.size() * 2 / 3); // 预先分配好内存, 防止在push_back过程中多次重复分配提高效率
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

    // 构建左子树
    if (leftDataIndexList.size() == 0)
    {
        pNode->LeftChildren = 0;
    }
    if (leftDataIndexList.size() != 0)
    {
        pNode->LeftChildren = new LKDTreeNode;
        this->BuildTree(pNode, pNode->LeftChildren, leftDataIndexList);
    }

    // 构建右子树
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

        // 到达叶子节点, 结束搜索
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