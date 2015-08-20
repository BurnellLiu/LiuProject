/// @file LKDTree.h
/// @brief k-d��
/// 
/// Detail: k-d��(k-dimensional���ļ��), ��һ�ַָ�kά���ݿռ�����ݽṹ
/// ��ҪӦ���ڶ�ά�ռ�ؼ����ݵ��������磺��Χ�������������������
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

/// @brief KD���ڵ�
struct LKDTreeNode;

typedef LMatrix<float> LKDTreeDataSet; ///< ���ݼ�(���ж���)
typedef LMatrix<float> LKDTreeData; ///< ����(һ�ж���)
typedef vector<unsigned int> LKDTreeDataIndexList; ///< ���������б�
typedef vector<LKDTreeNode*> LKDTreeNodeList; ///< ���ڵ��б�

/// @brief KD��
class LKDTree
{
public:
    LKDTree();
    ~LKDTree();

    /// @brief ����KD��
    ///  
    /// @param[in] dataSet ���ݼ�, Ҫ�����ݼ����ж���
    void BuildTree(IN const LKDTreeDataSet& dataSet);

    /// @brief �����ݼ���������ָ���������ڽ�����������
    ///  
    /// @param[in] data Դ����
    /// @return �ɹ��������ڽ�����������, ʧ�ܷ���-1
    int SearchNearestNeighbor(IN const LKDTreeData& data);

    /// @brief �����ݼ���������ָ���������ڽ���K����������
    ///  
    /// @param[in] data Դ����
    /// @param[in] k ��Ҫ���������ڽ��ĸ���(kҪ�����0������)
    /// @return �ɹ����ػ�ȡ�������ڽ������ݸ���(k), ʧ�ܷ���-1(��������)
    int SearchKNearestNeighbors(
        IN const LKDTreeData& data, 
        IN unsigned int k, 
        OUT LKDTreeDataIndexList& indexList);

private:
    /// @brief ����KD��
    ///  
    /// @param[in] pParent ���ڵ�
    /// @param[in] pNode ��ǰ�ڵ�
    /// @param[in] dataIndexList ���������б�
    void BuildTree(
        IN LKDTreeNode* pParent, 
        IN LKDTreeNode* pNode,
        IN const LKDTreeDataIndexList& dataIndexList);

    /// @brief ������
    ///  
    /// @param[in] pNode ���ڵ�
    /// @param[out] nodeList ���������Ľڵ��б�
    void TraverseTree(IN LKDTreeNode* pNode, OUT LKDTreeNodeList& nodeList);

    /// @brief ������
    ///  
    /// @param[in] data Դ����
    /// @param[out] searchPath ��������·��
    void SearchTree(IN const LKDTreeData& data, OUT LKDTreeNodeList& searchPath);

    /// @brief ����ָ�����������ݼ���һ�����ݵľ���ֵ
    ///  
    /// @param[in] data ָ��������
    /// @param[in] index ���ݼ��е���������
    /// @return ���ؾ���ֵ(ŷ����þ���), ʹ��ǰ�뱣֤������ȷ
    float CalculateDistance(IN const LKDTreeData& data, IN unsigned int index);

    /// @brief ������
    ///  
    /// @param[in] pNode ��Ҫ����Ľڵ�
    void ClearTree(IN LKDTreeNode*& pNode);
private:
    LKDTreeNode* m_pRootNode; ///< ���ڵ�
    LKDTreeDataSet m_dataSet; ///< ���ݼ�
};


#endif