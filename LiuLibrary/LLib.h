

/// Author:Burnell_Liu Email:674288799@qq.com Date: 2014/09/04
/// Description: 常用的函数
/// 
/// Others: 
/// Function List: 
///
/// History: 
///  1. Date, Author 
///  Modification
///


#ifndef _LLIB_H_
#define _LLIB_H_

#include <cstdlib>
#include <time.h>

namespace LRandom
{
    /// @brief 使用时间作为随机数种子
    inline void SRandTime()
    {
        srand((int)time(0));
    }

    /// @brief 产生随机整数
    /// @param[in] min 随机整数的最小值
    /// @param[in] max 随机整数的最大值
    /// @return 随机整数
    inline int RandInt(int min, int max) 
    {
        return rand()%(max - min + 1) + min;
    }

    /// @brief 产生随机布尔值
    /// @return 随机布尔值
    inline bool RandBool()
    {
        if (RandInt(0, 1)) 
            return true;
        else 
            return false;
    }

    /// @brief 产生随机小数, 范围0~1
    /// @return 随机小数
    inline float RandFloat()		   
    {
        return (rand())/(RAND_MAX + 1.0f);
    }

    /// @brief 产生随机小数
    /// @param[in] min 随机小数的最小值
    /// @param[in] max 随机小数的最大值
    /// @return 随机小数
    inline float RandFloat(float min, float max)
    {
        return min + (max - min) * rand() / (RAND_MAX + 1.0f);
    }

    /// @brief 产生随机小数, 范围-1~1
    /// @return 随机小数
    inline float RandClamped()	   
    {
        return RandFloat() - RandFloat();
    }

};

namespace LDestroy
{
    /// @brief 安全删除指针对象
    template<typename T>
    inline void SafeDelete(T*& pObject)
    {
        if (pObject != 0)
        {
            delete pObject;
            pObject = 0;
        }
    }

    /// @brief 安全删除数组对象
    template<typename T>
    inline void SafeDeleteArray(T*& pArray)
    {
        if (pArray != 0)
        {
            delete [] pArray;
            pArray = 0;
        }
    }
    
};

namespace LTypeValue
{
    /// @brief 获取无符号整型的最大值
    /// @return 无符号整形型的最大值
    inline unsigned int GetUnsignedIntMax()
    {
        unsigned int temp = ~0;
        return temp;
    }

    /// @brief 获取整型的最大值
    /// @return 整型的最大值
    inline int GetIntMax()
    {
        unsigned int temp = ~0;
        return temp >> 1;
    }
};

#endif