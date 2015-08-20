
#ifndef _DATASTRUCT_LLIST_H_
#define _DATASTRUCT_LLIST_H_

#include "LDefine.h"

/// @brief 列表抽象基类
///
/// 线性表下标索引从0开始，该线性表的使用要求元素类型支持赋值操作符
LTEMPLATE 
class LList
{
public:
	/// @brief 清空
	virtual void Clear() = 0;

	/// @brief 返回线性表中的元素个数
	/// @return 线性表中的元素个数
	virtual int Length() const = 0;

	/// @brief 获取索引下标为index的元素指针
	/// @param[in] index 索引下标，index范围0~Length()-1
	/// @return 成功返回元素指针，失败返回NULL
	virtual Type* GetElem(LIndex index) = 0;

	/// @brief 查找指定的元素
	/// @param[in] elem 需要查找的元素
	/// @return 成功返回元素的索引下标，失败返回 -1
	//virtual int LocateElem(const Type& elem) const = 0;

	/// @brief 在指定的索引下标index前插入元素
	/// 
	/// 该操作将复制指定的元素到线性表中，要求元素的类型支持赋值操作符
	/// @param[in] index 索引下标，index范围0~Length()
	/// @param[in] elem 需要插入的元素
	/// @return 成功返回LOK，失败返回LFAIL
	virtual LStatus InsertElem(LIndex index, const Type& elem) = 0;

	/// @brief 在线性表尾部添加元素
	/// @param[in] elem 需要添加的元素
	/// @return 成功返回LOK，失败返回LFAIL
	virtual LStatus PushBack(const Type& elem) = 0;

	/// @brief 删除指定的索引下标index所对应的元素
	/// @param[in] index 索引下标，index范围0~Length()-1
	/// @return 成功返回0，失败返回-1
	virtual LStatus DeleteElem(LIndex index) =0;
};

/// @brief 顺序列表
///
/// 该类禁止拷贝构造函数和赋值操作符的使用
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

/// @brief 链列表
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