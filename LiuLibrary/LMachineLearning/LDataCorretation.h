
/// Author:Burnell_Liu Email:674288799@qq.com Date: 2014/08/07
/// Description: 数据相关性
/// 
/// 欧几里得距离评价
/// 皮尔逊相关系数评价
/// Others: 
/// Function List: 
///
/// History: 
///  1. Date, Author 
///  Modification
///




#ifndef _LDATACORRETATION_H_
#define _LDATACORRETATION_H_

#include "LDataStruct/include/LArray.h"

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

typedef LArray<float> LDSDataList;

/// @brief 数据相似度
class LDataSimilar
{
public:
    /// @brief 计算数据列表A和B的相似度
    ///
    /// 要求A列表和B列表长度相同, 并且每列数据对应的含义相同
    virtual float Calculate(IN const LDSDataList& dataListA, IN const LDSDataList& dataListB) = 0;
};

/// @brief 欧几里得距离评价
class LEuclideanDistance : public LDataSimilar
{
public:
    /// @brief 计算数据列表A和B的相似度
    ///
    /// 要求A列表和B列表长度相同, 并且每列数据对应的含义相同
    /// @param[in] dataListA
    /// @param[in] dataListB
    /// @return 0~1 数据距离越近返回值越大, 返回值为1时数据相同, 返回值为0时表示数据长度不同
    virtual float Calculate(IN const LDSDataList& dataListA, IN const LDSDataList& dataListB);
};

/// @brief 皮尔逊相关度评价
///
/// 皮尔逊相关系数是一种度量两个变量间相关程度的方法,
/// 它是一个介于 1 和 -1 之间的值，其中，1 表示变量完全正相关， 0 表示无关，-1 表示完全负相关
/// 相比于欧几里得距离评价, 该算法计算复杂, 但该算法可以修正"夸大分值的情况"
class LPearsonCorretation : public LDataSimilar
{
public:
    /// @brief 计算数据列表A和B的相关度
    ///
    /// 要求A列表和B列表长度相同, 并且每列数据对应的含义相同
    /// @param[in] dataListA
    /// @param[in] dataListB
    /// @return -1~1, 1表示变量完全正相关, 0表示无关, -1 表示完全负相关(数据长度不同返回0)
    virtual float Calculate(IN const LDSDataList& dataListA, IN const LDSDataList& dataListB);
};

/// @brief Tanimoto系数度量方法
///
/// 对两列数据的指定项的重叠情况的度量
class LTanimotoCoefficient : public LDataSimilar
{
public:
    LTanimotoCoefficient();
    ~LTanimotoCoefficient();

    /// @brief 设置重叠值(默认1.0)
    /// @param[in] sameValue 重叠值
    void SetSameValue(IN float sameValue);

    /// @brief 计算重叠情况
    ///
    /// 计算前请先设置重叠值
    /// @param[in] dataListA
    /// @param[in] dataListB
    /// @return 0~1, 1表示两列数据完全重叠, 0表示两列数据没有指定的重叠项
    virtual float Calculate(IN const LDSDataList& dataListA, IN const LDSDataList& dataListB);

private:
    float m_sameValue;
};

#endif