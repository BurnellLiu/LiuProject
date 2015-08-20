

#ifndef _DATASTRUCT_LSTACK_H_ 
#define _DATASTRUCT_LSTACK_H_

#include "LDefine.h"

/// @brief ջ�������
///
/// �����ʹ��Ҫ������Ԫ��֧�ָ�ֵ������
LTEMPLATE
class LStack
{
public:
	/// @brief ��ջ
	/// @param[in] elem ��Ҫѹ�������Ԫ��
	/// @return �ɹ�����LOK��ʧ�ܷ���LFAIL
	virtual LStatus Push(const Type& elem) = 0;

	/// @brief ��ջ
	/// @return �ɹ�����LOK��ʧ�ܷ���LFAIL
	virtual LStatus Pop() = 0;

	/// @brief ��ȡջ��Ԫ��
	/// @return �ɹ�����ջ��Ԫ��ָ�룬ʧ�ܷ���NULL
	virtual Type* GetTop() = 0;

	/// @brief ��ȡ��ǰջ����
	/// @return ���ص�ǰջ����
	virtual int Length() const = 0;

	/// @brief �Ƿ�Ϊ��ջ
	/// @return ��ջ����true, �ǿ�ջ����false
	virtual bool Empty() const =0;

	/// @brief ���ջ
	virtual void Clear() = 0;
};

/// @brief ˳��ջ
/// 
/// ��ջ����󳤶�ΪLSQSTACK_MAX_LENGTH
LTEMPLATE
class LSqStack : public LStack<Type>
{
public:
	LSqStack();
	virtual ~LSqStack();

public:
	virtual LStatus Push(const Type& elem);
	virtual LStatus Pop();
	virtual Type* GetTop();

	virtual int Length() const;
	virtual bool Empty() const;

	virtual void Clear();

private:
	LSqStack(const LSqStack<Type>&);
	LSqStack<Type>& operator = (const LSqStack<Type>&);

private:
	Type m_data[LSQSTACK_MAX_LENGTH];
	LIndex m_top;
};

LTEMPLATE
LSqStack<Type>::LSqStack()
{
	m_top= 0;
}

LTEMPLATE
LSqStack<Type>::~LSqStack()
{

};

LTEMPLATE
LStatus LSqStack<Type>::Push(const Type& elem)
{
	if (m_top == LSQSTACK_MAX_LENGTH)
		return LFAIL;

	m_data[m_top] = elem;
	m_top++;
	return LOK;
}

LTEMPLATE
LStatus LSqStack<Type>::Pop()
{
	if (m_top == 0)
		return LFAIL;

	m_top--;

	return LOK;
}

LTEMPLATE
Type* LSqStack<Type>::GetTop()
{
	if (m_top == 0)
		return NULL;

	return &m_data[m_top-1];
}

LTEMPLATE
int LSqStack<Type>::Length() const
{
	return m_top;
}

LTEMPLATE
bool LSqStack<Type>::Empty() const
{
	if (m_top == 0)
		return true;
	else
		return false;
}

LTEMPLATE
void LSqStack<Type>::Clear()
{
	m_top = 0;
}

/// @brief ��ջ
LTEMPLATE
class LLinkStack : public LStack<Type>
{
public:
	LLinkStack();
	virtual ~LLinkStack();

public:
	virtual LStatus Push(const Type& elem);
	virtual LStatus Pop();
	virtual Type* GetTop();

	virtual int Length() const;
	virtual bool Empty() const;

	virtual void Clear();

private:
	LLinkStack(const LLinkStack<Type>&);
	LLinkStack<Type>& operator = (const LLinkStack<Type>&);

	LTEMPLATE_T
	struct LStackNode
	{
		T Data;
		struct LStackNode<T>* PNext;
	};
private:
	int m_length;
	LStackNode<Type> m_top;
};

LTEMPLATE
LLinkStack<Type>::LLinkStack()
{
	m_length = 0;
	m_top.PNext = NULL;
}

LTEMPLATE
LLinkStack<Type>::~LLinkStack()
{
	Clear();
}

LTEMPLATE
LStatus LLinkStack<Type>::Push(const Type& elem)
{
	LStackNode<Type>* pNewNode = new LStackNode<Type>;
	if (pNewNode == NULL)
		return LFAIL;

	pNewNode->Data = elem;
	pNewNode->PNext = m_top.PNext;
	m_top.PNext = pNewNode;
	m_length++;

	return LOK;
}

LTEMPLATE
LStatus LLinkStack<Type>::Pop()
{
	if (m_top.PNext == NULL)
		return LFAIL;

	LStackNode<Type>* pDeleteNode = m_top.PNext;
	m_top.PNext = pDeleteNode->PNext;
	SAFE_DELETE(pDeleteNode);
	m_length--;

	return LOK;
}

LTEMPLATE
Type* LLinkStack<Type>::GetTop()
{
	if (m_top.PNext == NULL)
		return NULL;

	return &(m_top.PNext->Data);
}

LTEMPLATE
int LLinkStack<Type>::Length() const
{
	return m_length;
}

LTEMPLATE
bool LLinkStack<Type>::Empty() const
{
	if (m_top.PNext == NULL)
		return true;
	else
		return false;
}

LTEMPLATE
void LLinkStack<Type>::Clear()
{
	LStackNode<Type>* pDeleteNode = m_top.PNext;
	while (pDeleteNode)
	{
		m_top.PNext = pDeleteNode->PNext;
		SAFE_DELETE(pDeleteNode);
		pDeleteNode = m_top.PNext;
	}
	m_length = 0;
}
#endif