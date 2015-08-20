
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

/// @brief ��̬����
LTEMPLATE
class LArray
{
public:
	/// @brief Ĭ�Ϲ��캯��
	///
	/// Ĭ�����鳤��Ϊ0, ��û�����ݿɱ�����
	LArray();
	~LArray();

	/// @brief ���캯��
	/// @param[in] length ���鳤��, ��� length <= 0 �����鳤��Ϊ0
	explicit LArray(int length);

	/// @brief �������캯��
	LArray(const LArray<Type>& rhs);

	/// @brief ��ֵ������
	LArray<Type>& operator = (const LArray<Type>& rhs);

	/// @brief ��������
	/// @param[in] length �µ����鳤��, ��� length <= 0�����鳤��Ϊ0
	void Reset(int length);

	/// @brief []������
	///
	/// �����ʹ�ø�Ч�ʷ�����ֱ��ʹ��Data
	/// @param[in] index �����±�����
	/// @return ����Ԫ������
	Type& operator[](int index);

    /// @brief []������
    ///
    /// �����ʹ�ø�Ч�ʷ�����ֱ��ʹ��Data
    /// @param[in] index �����±�����
    /// @return ����Ԫ������
    const Type& operator[](int index) const;

public:
	Type* Data; ///< �����б�
	int Length; ///< ���ݳ���
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
