/// @file LKDTree.h
/// @brief k-d树
/// 
/// Detail: k-d树(k-dimensional树的简称), 是一种分割k维数据空间的数据结构
/// 主要应用于多维空间关键数据的搜索（如：范围搜索和最近邻搜索）。
/// @author Burnell_Liu  
/// @version   
/// @date 31:7:2015

#ifndef _LKDTREE_H_
#define _LKDTREE_H_

#include <vector>
using std::vector;

#include "LDataStruct/LMatrix.h"

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

/// @brief KD树节点
struct LKDTreeNode;

typedef LMatrix<float> LKDTreeDataSet; ///< 数据集(多行多列)
typedef LMatrix<float> LKDTreeData; ///< 数据(一行多列)
typedef vector<unsigned int> LKDTreeDataIndexList; ///< 数据索引列表
typedef vector<LKDTreeNode*> LKDTreeNodeList; ///< 树节点列表

/// @brief KD树
class LKDTree
{
public:
    LKDTree();
    ~LKDTree();

    /// @brief 构造KD树
    ///  
    /// @param[in] dataSet 数据集, 要求数据集多行多列
    void BuildTree(IN const LKDTreeDataSet& dataSet);

    /// @brief 在数据集中搜索与指定数据最邻近的数据索引
    ///  
    /// @param[in] data 源数据
    /// @return 成功返回最邻近的数据索引, 失败返回-1
    int SearchNearestNeighbor(IN const LKDTreeData& data);

    /// @brief 在数据集中搜索与指定数据最邻近的K个数据索引
    ///  
    /// @param[in] data 源数据
    /// @param[in] k 需要搜索的最邻近的个数(k要求大于0的整数)
    /// @return 成功返回获取到的最邻近的数据个数(k), 失败返回-1(参数错误)
    int SearchKNearestNeighbors(
        IN const LKDTreeData& data, 
        IN unsigned int k, 
        OUT LKDTreeDataIndexList& indexList);

private:
    /// @brief 构造KD树
    ///  
    /// @param[in] pParent 父节点
    /// @param[in] pNode 当前节点
    /// @param[in] dataIndexList 数据索引列表
    void BuildTree(
        IN LKDTreeNode* pParent, 
        IN LKDTreeNode* pNode,
        IN const LKDTreeDataIndexList& dataIndexList);

    /// @brief 遍历树
    ///  
    /// @param[in] pNode 树节点
    /// @param[out] nodeList 遍历出来的节点列表
    void TraverseTree(IN LKDTreeNode* pNode, OUT LKDTreeNodeList& nodeList);

    /// @brief 搜索树
    ///  
    /// @param[in] data 源数据
    /// @param[out] searchPath 搜索出的路径
    void SearchTree(IN const LKDTreeData& data, OUT LKDTreeNodeList& searchPath);

    /// @brief 计算指定数据与数据集中一个数据的距离值
    ///  
    /// @param[in] data 指定的数据
    /// @param[in] index 数据集中的数据索引
    /// @return 返回距离值(欧几里得距离), 使用前请保证参数正确
    float CalculateDistance(IN const LKDTreeData& data, IN unsigned int index);

    /// @brief 清理树
    ///  
    /// @param[in] pNode 需要清理的节点
    void ClearTree(IN LKDTreeNode*& pNode);
private:
    LKDTreeNode* m_pRootNode; ///< 根节点
    LKDTreeDataSet m_dataSet; ///< 数据集
};


#endif