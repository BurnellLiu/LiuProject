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

typedef LMatrix<float> LKDTreeMatrix; ///< KD������
typedef LMatrix<int> LKDTreeList; ///< �б�(������)

class CKDTree;

/// @brief KD��
class LKDTree
{
public:
    /// @brief ���캯��
    LKDTree();

    /// @brief ��������
    ~LKDTree();

    /// @brief ����KD��
    /// @param[in] dataSet ���ݼ�, Ҫ�����ݼ����ж���
    void BuildTree(IN const LKDTreeMatrix& dataSet);

    /// @brief �����ݼ���������ָ���������ڽ�����������
    /// @param[in] data Դ����(������)
    /// @return �ɹ��������ڽ�����������, ʧ�ܷ���-1
    int SearchNearestNeighbor(IN const LKDTreeMatrix& data);

    /// @brief �����ݼ���������ָ���������ڽ���K����������
    /// @param[in] data Դ����(������)
    /// @param[in] k ��Ҫ���������ڽ��ĸ���(kҪ�����0������)
    /// @param[out] indexList �洢���ڽ������������б�(������, 1 * k), �ӽ���Զ
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool SearchKNearestNeighbors(IN const LKDTreeMatrix& data, IN unsigned int k, OUT LKDTreeList& indexList);

private:
    CKDTree* m_pKDTree; ///< KD��ʵ�ֶ���
};


#endif