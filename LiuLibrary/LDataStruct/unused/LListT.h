
#ifndef _DATASTRUCT_LLIST_H_
#define _DATASTRUCT_LLIST_H_

#include "LDefine.h"

/// @brief �б�������
///
/// ���Ա��±�������0��ʼ�������Ա��ʹ��Ҫ��Ԫ������֧�ָ�ֵ������
LTEMPLATE 
class LList
{
public:
	/// @brief ���
	virtual void Clear() = 0;

	/// @brief �������Ա��е�Ԫ�ظ���
	/// @return ���Ա��е�Ԫ�ظ���
	virtual int Length() const = 0;

	/// @brief ��ȡ�����±�Ϊindex��Ԫ��ָ��
	/// @param[in] index �����±꣬index��Χ0~Length()-1
	/// @return �ɹ�����Ԫ��ָ�룬ʧ�ܷ���NULL
	virtual Type* GetElem(LIndex index) = 0;

	/// @brief ����ָ����Ԫ��
	/// @param[in] elem ��Ҫ���ҵ�Ԫ��
	/// @return �ɹ�����Ԫ�ص������±꣬ʧ�ܷ��� -1
	//virtual int LocateElem(const Type& elem) const = 0;

	/// @brief ��ָ���������±�indexǰ����Ԫ��
	/// 
	/// �ò���������ָ����Ԫ�ص����Ա��У�Ҫ��Ԫ�ص�����֧�ָ�ֵ������
	/// @param[in] index �����±꣬index��Χ0~Length()
	/// @param[in] elem ��Ҫ�����Ԫ��
	/// @return �ɹ�����LOK��ʧ�ܷ���LFAIL
	virtual LStatus InsertElem(LIndex index, const Type& elem) = 0;

	/// @brief �����Ա�β�����Ԫ��
	/// @param[in] elem ��Ҫ��ӵ�Ԫ��
	/// @return �ɹ�����LOK��ʧ�ܷ���LFAIL
	virtual LStatus PushBack(const Type& elem) = 0;

	/// @brief ɾ��ָ���������±�index����Ӧ��Ԫ��
	/// @param[in] index �����±꣬index��Χ0~Length()-1
	/// @return �ɹ�����0��ʧ�ܷ���-1
	virtual LStatus DeleteElem(LIndex index) =0;
};

/// @brief ˳���б�
///
/// �����ֹ�������캯���͸�ֵ��������ʹ��
LTEMPLATE
class LSqList : public LList<Type>
{
public:
	LSqList();
	virtual ~LSqList();

public:
	virtual void Clear();
	virtual int Length() const;

	virtual Type* GetElem(LIndex index);
	virtual LStatus InsertElem(LIndex index, const Type& elem);
	virtual LStatus DeleteElem(LIndex index);
	virtual LStatus PushBack(const Type& elem);

private:
	LSqList(const LSqList<Type>&);
	LSqList<Type>& operator = (const LSqList<Type>&);
	
private:
	Type m_data[LSQLIST_MAX_LENGTH];
	int m_currentLength;
};

LTEMPLATE
LSqList<Type>::LSqList()
{
	m_currentLength= 0;
}

LTEMPLATE
LSqList<Type>::~LSqList()
{

}

LTEMPLATE
void LSqList<Type>::Clear()
{
	m_currentLength = 0;
}

LTEMPLATE
int LSqList<Type>::Length() const
{
	return m_currentLength;
}

LTEMPLATE
Type* LSqList<Type>::GetElem(LIndex index)
{
	if (index < 0|| index >= Length())
		return NULL;

	return &m_data[index];
}

LTEMPLATE
LStatus LSqList<Type>::InsertElem(LIndex index, const Type& elem)
{
	if (index < 0 || index > Length())
		return LFAIL;

	if (Length() >= LSQLIST_MAX_LENGTH)
		return LFAIL;

	for (LIndex i = Length()-1; i >= index; i--)
	{
		m_data[i + 1] = m_data[i];
	}
	m_currentLength += 1;
	m_data[index] = elem;

	return LOK;
}

LTEMPLATE
LStatus LSqList<Type>::PushBack(const Type& elem)
{
	return InsertElem(Length(), elem);
}

LTEMPLATE
LStatus LSqList<Type>::DeleteElem(LIndex index)
{
	if (index < 0 || index >= Length())
		return LFAIL;

	for (LIndex i = index; i < Length()-1; i++)
	{
		m_data[i] = m_data[i + 1];
	}

	m_currentLength -= 1;

	return LOK;
}

/// @brief ���б�
LTEMPLATE
class LLinkList : public LList<Type>
{
public:
	LLinkList();
	virtual ~LLinkList();
public:
	virtual void Clear();
	virtual int Length() const;

	virtual Type* GetElem(LIndex index);
	virtual LStatus InsertElem(LIndex index, const Type& elem);
	virtual LStatus DeleteElem(LIndex index);
	virtual LStatus PushBack(const Type& elem);

private:
	LLinkList(const LLinkList<Type>&);
	LLinkList<Type>& operator = (const LLinkList<Type>&);

	LTEMPLATE_T
	struct LNode
	{
		T Data;
		struct LNode<T>* PNext;
	};

private:
	int m_currentLength;
	LNode<Type> m_heardNode;
};

LTEMPLATE
LLinkList<Type>::LLinkList()
{
	m_currentLength = 0;
	m_heardNode.PNext = NULL;
}

LTEMPLATE
LLinkList<Type>::~LLinkList()
{
	Clear();
}

LTEMPLATE
void LLinkList<Type>::Clear()
{
	LNode<Type>* pNode = m_heardNode.PNext;
	LNode<Type>* pNodeNext = NULL;

	while (pNode)
	{
		pNodeNext = pNode->PNext;
		SAFE_DELETE(pNode);
		pNode = pNodeNext;
	}

	m_heardNode.PNext = NULL;
	m_currentLength = 0;
}

LTEMPLATE
int LLinkList<Type>::Length() const
{
	return m_currentLength;
}

LTEMPLATE
Type* LLinkList<Type>::GetElem(LIndex index)
{
	if (index < 0 || index >= Length())
		return NULL;

	LNode<Type>* pNode = m_heardNode.PNext;
	LIndex i = 0;
	while (i < index)
	{
		pNode = pNode->PNext;
		i++;
	}

	return &(pNode->Data);
}

LTEMPLATE
LStatus LLinkList<Type>::InsertElem(LIndex index, const Type& elem)
{
	if (index < 0 || index > Length())
		return LFAIL;

	LNode<Type>* pNode = &m_heardNode;
	LIndex i = 0;
	while (i < index)
	{
		pNode = pNode->PNext;
		i++;
	}

	LNode<Type>* pNewNode = new LNode<Type>;
	if (pNewNode == NULL)
		return LFAIL;

	pNewNode->Data = elem;
	pNewNode->PNext = pNode->PNext;
	pNode->PNext = pNewNode;
	m_currentLength++;

	return LOK;
}

LTEMPLATE
LStatus LLinkList<Type>::PushBack(const Type& elem)
{
	return InsertElem(Length(), elem);
}

LTEMPLATE
LStatus LLinkList<Type>::DeleteElem(LIndex index)
{
	if (index < 0 || index >= Length())
		return LFAIL;

	int i = 0;
	LNode<Type>* pNode = &m_heardNode;
	while (i < index)
	{
		pNode = pNode->PNext;
		i++;
	}

	LNode<Type>*pNodeDelete = pNode->PNext;
	pNode->PNext = pNodeDelete->PNext;
	delete pNodeDelete;
	pNodeDelete = NULL;
	m_currentLength--;

	return LOK;
}

#endif