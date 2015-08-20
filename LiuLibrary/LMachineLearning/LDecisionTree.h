/// @file LDecision.h
/// @brief  ������ͷ�ļ�
/// 
/// Detail:���ļ�������ʵ�־�������ʵ���㷨, �������ع����㷨(CART, Classification And Regression Tree)
/// @author Burnell_Liu  
/// @version   
/// @date 5:6:2015

#ifndef _LDECISION_H_
#define _LDECISION_H_

#include <string>
using std::string;
#include <map>
using std::map;

#include "LDataStruct/LArray.h"


#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

/// @brief ����
class LVariant
{
public:
    enum VALUE_TYPE
    {
        UNKNOWN = 0, // δ֪
        INT, ///< ��ֵ����
        STRING ///< �ַ�����
    };
public:
    LVariant();
    LVariant(IN int value);
    LVariant(IN const string& value);
    LVariant(IN const LVariant& rhs);
    ~LVariant();

    LVariant& operator = (IN const LVariant& rhs);

    bool operator < (IN const LVariant& rhs) const;

    /// @brief ��ȡ�����д洢����������
    ///  
    /// @return ��������
    VALUE_TYPE GetType() const;

    /// @brief ����������ֵ
    ///  
    /// @param[in] value ������ֵ
    void SetIntValue(IN int value);

    /// @brief ��ȡ������ֵ
    ///  
    /// @return ����ֵ
    int GetIntValue() const;

    /// @brief �����ַ���ֵ
    ///  
    /// @param[in] value �ַ���ֵ
    void SetStringValue(IN const string& value);

    /// @brief ��ȡ�ַ���ֵ
    ///  
    /// @return �ַ���ֵ
    string GetStringValue() const;

private:
    int* m_pValueInt; ///< ������ֵ
    string* m_pValueStr; ///< �ַ���ֵ
    VALUE_TYPE m_type; ///< ��ֵ����
};


typedef LArray<LVariant> LDTDataList;
typedef LArray<LDTDataList> LDTDataSet;

struct LDecisionTreeNode;

/// @brief ������
class LDecisionTree
{
public:
    LDecisionTree();
    ~LDecisionTree();

    /// @brief ���������
    ///  
    /// ���ݼ��ɶ����������, ÿһ�����ݶ�����һ��۲������һ�����ֵ
    /// �۲�����ͽ��ֵ������string��int
    /// ͬһ�е�ֵҪôȫΪstringҪôȫΪint, ��������unknown����
    /// @param[in] dataSet ѵ�����ݼ�
    /// @return ����ʧ�ܷ���false, ѵ�����ݼ���ʽ����ȷ��ʧ��
    bool BuildTree(IN const LDTDataSet& dataSet);

    /// @brief ���м�֦����(���ϲ�Ҷ�ӽڵ�)
    ///
    /// �������Ҷ�ӽڵ�ϲ������ӵ���С��ָ������Сֵ, ����кϲ�����
    /// @param[in] minGain ��С����ֵ
    void Prune(IN float minGain);

    /// @brief �������ݽ��з���
    ///  
    /// @param[in] dataList �����б�
    /// @param[out] result ������
    /// @return �ɹ����෵��true, ʧ�ܷ���false
    bool Classify(IN const LDTDataList& dataList, OUT map<LVariant, float>& resultMap);

    /// @brief ��ӡ������(���ڵ���)
    void PrintTree();

private:
    /// @brief �ݹ鹹�������
    ///  
    /// @param[in] dataSet ���ݼ�
    /// @return �������ڵ�
    LDecisionTreeNode* RecursionBuildTree(IN const LDTDataSet& dataSet);

    /// @brief �ݹ���м�֦����(���ϲ�Ҷ�ӽڵ�)
    ///
    /// �������Ҷ�ӽڵ�ϲ������ӵ���С��ָ������Сֵ, ����кϲ�����
    /// @param[in] pNode �������ڵ�
    /// @param[in] minGain ��С����ֵ
    void RecursionPrune(IN LDecisionTreeNode* pNode, IN float minGain);

    /// @brief �������ݽ��еݹ����
    ///  
    /// @param[in] pNode �������ڵ�
    /// @param[in] dataList �����б�
    /// @param[out] result ������
    /// @return �ɹ����෵��true, ʧ�ܷ���false
    bool RecursionClassify(IN LDecisionTreeNode* pNode, IN const LDTDataList& dataList, OUT map<LVariant, int>& resultMap);

    /// @brief �ݹ�ɾ��������
    ///  
    /// @param[in] pNode �������ڵ�
    void RecursionDeleteTree(IN LDecisionTreeNode* pNode);

    /// @brief �ݹ��ӡ��
    ///  
    /// @param[in] pNode �������ڵ�
    /// @param[in] space �ո�
    void RecursionPrintTree(IN const LDecisionTreeNode* pNode, IN string space);

    /// @brief ������ݼ��Ƿ����Ҫ��
    ///  
    /// @param[in] dataSet ���ݼ�
    /// @return ����Ҫ�󷵻�true, ������Ҫ�󷵻�false
    bool CheckDataSet(IN const LDTDataSet& dataSet);

    /// @brief ������ݼ�
    ///  
    /// @param[in] dataSet ��Ҫ��ֵ����ݼ�
    /// @param[in] column ������ݵ���
    /// @param[in] checkValue ������ݵ��еļ��ֵ
    /// @param[out] trueSet �����Ϊtrue�����ݼ�
    /// @param[out] falseSet �����Ϊfalse�����ݼ�
    void DevideSet(
        IN const LDTDataSet& dataSet, 
        IN int column, 
        IN LVariant& checkValue, 
        OUT LDTDataSet& trueSet, 
        OUT LDTDataSet& falseSet);

    /// @brief ͳ�ƽ����
    ///  
    /// @param[in] dataSet ���ݼ���
    /// @param[out] resultMap ����ֵ�
    void CountResult(IN const LDTDataSet& dataSet, OUT map<LVariant, int>& resultMap);

    /// @brief �������ݼ��������
    ///  
    /// ÿ�������е����һ��Ϊ����ֵ
    /// @param[in] dataSet ���ݼ�
    /// @return ���ݼ�������
    float Entropy(IN const LDTDataSet& dataSet);

private:
    LDecisionTreeNode* m_pRootNode; ///< ���������ڵ�
};





#endif