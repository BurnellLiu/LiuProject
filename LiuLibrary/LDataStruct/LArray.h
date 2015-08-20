
#ifndef _DATASTRUCT_LARRAY_H_
#define _DATASTRUCT_LARRAY_H_

#ifndef LTEMPLATE
#define LTEMPLATE template<typename Type>
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef INOUT
#define INOUT
#endif

/// @brief 动态数组
LTEMPLATE
class LArray
{
public:
	/// @brief 默认构造函数
	///
	/// 默认数组长度为0, 即没有数据可被访问
	LArray();
	~LArray();

	/// @brief 构造函数
	/// @param[in] length 数组长度, 如果 length <= 0 则数组长度为0
	explicit LArray(int length);

	/// @brief 拷贝构造函数
	LArray(const LArray<Type>& rhs);

	/// @brief 赋值操作符
	LArray<Type>& operator = (const LArray<Type>& rhs);

	/// @brief 重置数组
	/// @param[in] length 新的数组长度, 如果 length <= 0则数组长度为0
	void Reset(int length);

	/// @brief []操作符
	///
	/// 如果想使用高效率访问请直接使用Data
	/// @param[in] index 数组下标索引
	/// @return 数组元素引用
	Type& operator[](int index);

    /// @brief []操作符
    ///
    /// 如果想使用高效率访问请直接使用Data
    /// @param[in] index 数组下标索引
    /// @return 数组元素引用
    const Type& operator[](int index) const;

public:
	Type* Data; ///< 数据列表
	int Length; ///< 数据长度
};

LTEMPLATE
LArray<Type>::LArray()
{
	Length = 0;
	Data = 0;
}

LTEMPLATE
LArray<Type>::LArray(int length)
	: Length(0), Data(0)
{
	if (length <= 0)
		return;

	Length = length;
	Data = new Type[length];
}

LTEMPLATE
LArray<Type>::~LArray()
{
	if (this->Data)
	{
		delete[] this->Data;
	}
}

LTEMPLATE
LArray<Type>::LArray(const LArray<Type>& rhs)
	: Length(0), Data(0)
{
	if (rhs.Length <= 0)
		return;

	Length = rhs.Length;
	Data = new Type[Length];

	for (int i = 0; i < this->Length; i++)
	{
		this->Data[i] = rhs.Data[i];
	}
}

LTEMPLATE
LArray<Type>& LArray<Type>::operator = (const LArray<Type>& rhs)
{
	if (this->Length != rhs.Length)
	{
		if (this->Data)
		{
			delete[] this->Data;
			this->Data = 0;
		}
		this->Length = 0;

		if (rhs.Length > 0)
		{
			this->Length = rhs.Length;
			this->Data = new Type[Length];
		}
	}

	for (int i = 0; i < this->Length; i++)
	{
		this->Data[i] = rhs.Data[i];
	}

	return *this;
}

LTEMPLATE
void LArray<Type>::Reset(int length)
{
	if (this->Length != length)
	{
		if (this->Data)
		{
			delete[] this->Data;
			this->Data = 0;
		}
		this->Length = 0;

		if (length > 0)
		{
			this->Data = new Type[length];
			this->Length = length;
		}
	}
}

LTEMPLATE
Type& LArray<Type>::operator[](int index)
{
	return *(this->Data + index);
}

LTEMPLATE
const Type& LArray<Type>::operator[](int index) const
{
    return *(this->Data + index);
}

#endif
