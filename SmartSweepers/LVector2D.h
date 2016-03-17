
#ifndef _LVECTOR2D_H_
#define _LVECTOR2D_H_

#include "LMacroDefine.h"

/// @brief 二维矢量结构
struct LVector2D
{
	float X; ///< X坐标值
	float Y; ///< Y坐标值

	LVector2D()
	{
		X = 0.0f; 
		Y = 0.0f;
	}
	LVector2D(IN float x, IN float y)
	{
		X = x;
		Y = y;
	}

	/// @brief 获取矢量长度
	/// @return 矢量长度
	float Length()
	{
		return sqrt(X * X + Y * Y);
	}

	/// @brief  标准化矢量, 即长度为1的矢量
	void Normalize()
	{
		float length = this->Length();

		if (Length() != 0.0f)
		{
			this->X /= length;
			this->Y /= length;
		}
		
	}

	/// @brief 两个矢量的点积
	/// @param[in] v 矢量
	/// @return 点积
	float Dot(IN const LVector2D& v)
	{
		return this->X * v.X + this->Y * v.Y;
	}

	/// @brief 两个向量的符号
	///
	/// 向量v在向量的顺时针方向返回1，向量v在向量的逆时针方向返回-1
    /// 两向量方向相反算作在逆时针方向
	/// @param[in] IN const LVector2D & v
	/// @return 1 or -1
	int Sign(IN const LVector2D& v)
	{
		if (this->Y * v.X > this->X * v.Y)
			return 1;
		else
			return -1;
	}

	/// @brief 重载 += 操作符
	LVector2D& operator += (IN const LVector2D& rhs)
	{
		this->X += rhs.X;
		this->Y += rhs.Y;

		return *this;
	}

	/// @brief 重载 -= 操作符
	LVector2D& operator -= (IN const LVector2D& rhs)
	{
		this->X -= rhs.X;
		this->Y -= rhs.Y;

		return *this;
	}

	/// @brief 重载 *= 操作符
	LVector2D& operator *= (IN const float& rhs)
	{
		this->X *= rhs;
		this->Y *= rhs;

		return *this;
	}

	/// @brief 重载 /= 操作符
	LVector2D& operator /= (IN const float& rhs)
	{
		this->X /= rhs;
		this->Y /= rhs;

		return *this;
	}


};
#endif