

/// Author:Burnell_Liu Email:674288799@qq.com Date: 2014/09/04
/// Description: ���õĺ���
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
    /// @brief ʹ��ʱ����Ϊ���������
    inline void SRandTime()
    {
        srand((int)time(0));
    }

    /// @brief �����������
    /// @param[in] min �����������Сֵ
    /// @param[in] max ������������ֵ
    /// @return �������
    inline int RandInt(int min, int max) 
    {
        return rand()%(max - min + 1) + min;
    }

    /// @brief �����������ֵ
    /// @return �������ֵ
    inline bool RandBool()
    {
        if (RandInt(0, 1)) 
            return true;
        else 
            return false;
    }

    /// @brief �������С��, ��Χ0~1
    /// @return ���С��
    inline float RandFloat()		   
    {
        return (rand())/(RAND_MAX + 1.0f);
    }

    /// @brief �������С��
    /// @param[in] min ���С������Сֵ
    /// @param[in] max ���С�������ֵ
    /// @return ���С��
    inline float RandFloat(float min, float max)
    {
        return min + (max - min) * rand() / (RAND_MAX + 1.0f);
    }

    /// @brief �������С��, ��Χ-1~1
    /// @return ���С��
    inline float RandClamped()	   
    {
        return RandFloat() - RandFloat();
    }

};

namespace LDestroy
{
    /// @brief ��ȫɾ��ָ�����
    template<typename T>
    inline void SafeDelete(T*& pObject)
    {
        if (pObject != 0)
        {
            delete pObject;
            pObject = 0;
        }
    }

    /// @brief ��ȫɾ���������
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
    /// @brief ��ȡ�޷������͵����ֵ
    /// @return �޷��������͵����ֵ
    inline unsigned int GetUnsignedIntMax()
    {
        unsigned int temp = ~0;
        return temp;
    }

    /// @brief ��ȡ���͵����ֵ
    /// @return ���͵����ֵ
    inline int GetIntMax()
    {
        unsigned int temp = ~0;
        return temp >> 1;
    }
};

#endif