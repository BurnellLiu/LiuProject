
/// Author:Burnell_Liu Email:674288799@qq.com Date: 2014/08/07
/// Description: ���ݾ���
/// 
/// �����㷨��Ŀ���ǲɼ�����, Ȼ������ҵ���ͬ��Ⱥ��
/// Others: 
/// Function List: 
///
/// History: 
///  1. Date, Author 
///  Modification
///

#ifndef _LDATACLUSTER_H_
#define _LDATACLUSTER_H_

#include "LDataStruct/include/LArray.h"

#include "LMachineLearning/LDataCorretation.h"

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

typedef LArray<float> LDCDataList; // ���������б�
typedef LArray<LDCDataList> LDCDataMatrix; // �������ݾ���


/// @�������ƶ����۷���
enum LDATA_SIMILAR_METHOD
{
    EUCLIDEAN_DISTANCE = 0, ///< ŷ����þ���ֵ����
    PEARSON_CORRETATIO ///< Ƥ��ѷ��ض�����
};

/// @brief ������������
struct LBiClusterTNode
{
    LBiClusterTNode()
    {
        Id = 0;
        Distance = 0.0;
        PLChild = 0;
        PRChild = 0;
    }

    int Id; ///< ��ʶ��
    float Distance; ///< �������ӵ����ݾ���ֵ(��ض�Խ��, ����ֵԽС)
    LDCDataList DataList; ///< �����б�

    LBiClusterTNode* PLChild; ///< ����
    LBiClusterTNode* PRChild; ///< �Һ���
};

/// @brief ��������������߽ӿ�
class LBiClustarTreeVisitor
{
public:
    /// @brief ���ʾ����������
    /// @param[in] pNode �����
    virtual void Visit(IN const LBiClusterTNode* pNode) const = 0;
};

/// @brief ���������(�ּ�����)
///
/// �ּ�����ͨ�����ϵĽ���Ϊ���Ƶ�Ⱥ�������ϲ�,
/// �������һ��Ⱥ��Ĳ�νṹ(���νṹ, Ҷ�ӽ��ΪԭʼȺ��)
class LBiClusterTree
{
public:
    LBiClusterTree();
    ~LBiClusterTree();

    /// @brief �����������ƶ����۷���
    ///
    /// ����Init()����ǰʹ�ø÷���, Ĭ��ʹ��Ƥ��ѷ��ض�����
    void SetDataSimilerMethod(IN LDATA_SIMILAR_METHOD similarMethod);

    /// @brief ��ʼ��
    void Init();

    /// @brief �����ݽ��о���
    ///
    /// ������ʹ��Receive()���������ʾ����Ľ��
    /// @param[in] dataMatrix ���ݾ���(Ҫ������ÿ�г������)
    void Cluster(IN const LDCDataMatrix& dataMatrix);

    /// @brief ����һ��������
    /// @param[in] visitor ������
    void Receive(IN const LBiClustarTreeVisitor& visitor);

private:
    /// @brief ������������
    /// @param[in] pNode �������ָ��
    void Clear(IN LBiClusterTNode*& pNode);

private:
    LDATA_SIMILAR_METHOD m_dataSimilarMethod; ///< �������ƶ����۷���
    LDataSimilar* m_pDataSimilar; ///< �������ƶȽӿ�ָ��
    LBiClusterTNode* m_pRootNode; ///< �����
};


typedef LArray<int> LDCResultList; // �������б�
typedef LArray<LDCResultList> LDCResultMatrix; // ����������

/// @brief K��ֵ����
class LKMeansCluster
{
public:
    LKMeansCluster();
    ~LKMeansCluster();

    /// @brief �����������ƶ����۷���
    ///
    /// ����Init()����ǰʹ�ø÷���, Ĭ��ʹ��Ƥ��ѷ��ض�����
    void SetDataSimilerMethod(IN LDATA_SIMILAR_METHOD similarMethod);

    /// @brief ����Kֵ
    ///
    /// ����Init()����ǰʹ�ø÷���, Ĭ��KֵΪ2
    void SetK(IN int k);

    /// @brief ��ʼ��
    void Init();

    /// @brief �����ݽ��о���
    ///
    /// ������ʹ��Receive()���������ʾ����Ľ��
    /// @param[in] dataMatrix ���ݾ���(Ҫ������ÿ�г������)
    void Cluster(IN const LDCDataMatrix& dataMatrix, OUT LDCResultMatrix& resultMatrix);

private:
    float RandFloat();

private:
    int m_k; ///< Kֵ
    LDCResultMatrix m_resultMatrix; ///< ����������
    LDataSimilar* m_pDataSimilar; ///< �������ƶȽӿ�ָ��
    LDATA_SIMILAR_METHOD m_dataSimilarMethod; ///< �������ƶ����۷���
};



#endif

