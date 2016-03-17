
#ifndef _LVECTOR2D_H_
#define _LVECTOR2D_H_

#include "LMacroDefine.h"

/// @brief ��άʸ���ṹ
struct LVector2D
{
	float X; ///< X����ֵ
	float Y; ///< Y����ֵ

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

	/// @brief ��ȡʸ������
	/// @return ʸ������
	float Length()
	{
		return sqrt(X * X + Y * Y);
	}

	/// @brief  ��׼��ʸ��, ������Ϊ1��ʸ��
	void Normalize()
	{
		float length = this->Length();

		if (Length() != 0.0f)
		{
			this->X /= length;
			this->Y /= length;
		}
		
	}

	/// @brief ����ʸ���ĵ��
	/// @param[in] v ʸ��
	/// @return ���
	float Dot(IN const LVector2D& v)
	{
		return this->X * v.X + this->Y * v.Y;
	}

	/// @brief ���������ķ���
	///
	/// ����v��������˳ʱ�뷽�򷵻�1������v����������ʱ�뷽�򷵻�-1
    /// �����������෴��������ʱ�뷽��
	/// @param[in] IN const LVector2D & v
	/// @return 1 or -1
	int Sign(IN const LVector2D& v)
	{
		if (this->Y * v.X > this->X * v.Y)
			return 1;
		else
			return -1;
	}

	/// @brief ���� += ������
	LVector2D& operator += (IN const LVector2D& rhs)
	{
		this->X += rhs.X;
		this->Y += rhs.Y;

		return *this;
	}

	/// @brief ���� -= ������
	LVector2D& operator -= (IN const LVector2D& rhs)
	{
		this->X -= rhs.X;
		this->Y -= rhs.Y;

		return *this;
	}

	/// @brief ���� *= ������
	LVector2D& operator *= (IN const float& rhs)
	{
		this->X *= rhs;
		this->Y *= rhs;

		return *this;
	}

	/// @brief ���� /= ������
	LVector2D& operator /= (IN const float& rhs)
	{
		this->X /= rhs;
		this->Y /= rhs;

		return *this;
	}


};
#endif