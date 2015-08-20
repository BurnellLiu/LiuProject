

#ifndef _LDATASTRUCT_LQUEUE_H_
#define _LDATASTRUCT_LQUEUE_H_


#include "LDefine.h"


/// @brief ���г������
LTEMPLATE
class LQueue
{
public:
	/// @brief ����Ԫ�ص�����β��
	/// @param[in] elem ��Ҫ������е�Ԫ��
	/// @return �ɹ�����LOK��ʧ�ܷ���LFAIL
	virtual LStatus Entry(const Type& elem) = 0;

	/// @brief ��ȡ����ͷ��Ԫ��
	/// @return �ɹ����ض���ͷԪ��ָ�룬ʧ�ܷ���NULL
	virtual Type* Get() = 0;

	/// @brief ����ͷԪ�س�����
	/// @return �ɹ�����LOK��ʧ�ܷ���LFAIL
	virtual LStatus Out() = 0;

	/// @brief ��ȡ���еĳ���
	/// @return ����ĳ���
	virtual int Length() const = 0;

	/// @brief �����Ƿ�Ϊ��
	/// @return ����Ϊ�շ���true�����򷵻�false
	virtual bool Empty() const = 0;

	/// @brief ��������е�Ԫ��
	virtual void Clear() = 0;
};

/// @brief ѭ��˳�����
LTEMPLATE
class LSqQueue : public LQueue<Type>
{
public:
	LSqQueue();
	virtual ~LSqQueue();

public:
	virtual LStatus Entry(const Type& elem);
	virtual Type* Get();
	virtual LStatus Out();

	virtual int Length() const;
	virtual bool Empty() const;
	virtual void Clear();

private:
	LSqQueue(const LSqQueue<Type>&);
	LSqQueue<Type>& operator = (const LSqQueue<Type>&);

private:
	Type m_data[LSQQUEUE_MAX_LENGTH];
	LIndex m_front;
	LIndex m_rear;
	int m_length;
};

LTEMPLATE
LSqQueue<Type>::LSqQueue()
{
	m_front = 0;
	m_rear = 0;
	m_length = 0;
};

LTEMPLATE
LSqQueue<Type>::~LSqQueue()
{

}

LTEMPLATE
LStatus LSqQueue<Type>::Entry(const Type& elem)
{
	if (m_rear == m_front && m_length != 0)
		return LFAIL;

	m_data[m_rear] = elem;

	m_rear++;
	if (m_rear == LSQQUEUE_MAX_LENGTH)
		m_rear = 0;

	m_length++;

	return LOK;
}

LTEMPLATE
LStatus LSqQueue<Type>::Out()
{
	if (m_front == m_rear && m_length == 0)
		return LFAIL;

	m_front++;
	if (m_front == LSQQUEUE_MAX_LENGTH)
		m_front = 0;

	m_length--;

	return LOK;
}

LTEMPLATE
Type* LSqQueue<Type>::Get()
{
	if (m_front == m_rear && m_length == 0)
		return NULL;

	return &m_data[m_front];
}

LTEMPLATE
int LSqQueue<Type>::Length() const
{
	return m_length;
}

LTEMPLATE
bool LSqQueue<Type>::Empty() const
{
	if (m_length == 0)
		return true;
	else
		return false;
}

LTEMPLATE
void LSqQueue<Type>::Clear()
{
	m_front = 0;
	m_rear = 0;
	m_length = 0;
}

LTEMPLATE
class LLinkQueue : public LQueue<Type>
{
public:
	LLinkQueue();
	virtual ~LLinkQueue();

public:
	virtual LStatus Entry(const Type& elem);
	virtual Type* Get();
	virtual LStatus Out();

	virtual int Length() const;
	virtual bool Empty() const;
	virtual void Clear();

private:
	LLinkQueue(const LLinkQueue<Type>&);
	LLinkQueue<Type>& operator = (const LLinkQueue<Type>&);

	LTEMPLATE_T
	struct LQueueNode
	{
		T Data;
		struct LQueueNode<T>* PNext;
	};

private:
	int m_length;
	LQueueNode<Type> m_headNode;
	LQueueNode<Type>* m_pFront;
	LQueueNode<Type>* m_pRear;
};

LTEMPLATE
LLinkQueue<Type>::LLinkQueue()
{
	m_length = 0;
	m_headNode.PNext = NULL;
	m_pFront = &m_headNode;
	m_pRear = &m_headNode;
}

LTEMPLATE
LLinkQueue<Type>::~LLinkQueue()
{
	Clear();
}

LTEMPLATE
LStatus LLinkQueue<Type>::Entry(const Type& elem)
{
	LQueueNode<Type>* pNewNode = new LQueueNode<Type>;
	if (pNewNode == NULL)
		return LFAIL;
	
	pNewNode->Data = elem;
	pNewNode->PNext = NULL;
	
	m_pRear->PNext = pNewNode;

	m_pRear = pNewNode;
	m_length++;
	return LOK;
}

LTEMPLATE
LStatus LLinkQueue<Type>::Out()
{
	if (m_length == 0)
		return LFAIL;

	LQueueNode<Type>* pDeleteNode = m_pFront->PNext;
	m_pFront->PNext = pDeleteNode->PNext;

	if (pDeleteNode == m_pRear)
		m_pRear = m_pFront;

	SAFE_DELETE(pDeleteNode);
	m_length--;
	return LOK;
}

LTEMPLATE
Type* LLinkQueue<Type>::Get()
{
	if (m_pFront->PNext == NULL)
		return NULL;

	return &(m_pFront->PNext->Data);
}

LTEMPLATE
int LLinkQueue<Type>::Length() const 
{
	return m_length;
}

LTEMPLATE
bool LLinkQueue<Type>::Empty() const 
{
	if (m_length == 0)
		return true;
	else
		return false;
}
LTEMPLATE
void LLinkQueue<Type>::Clear()
{
	while (m_length > 0)
	{
		Out();
	}
}

#endif