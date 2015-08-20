
#ifndef _DATASTRUCT_LLIST_H_
#define _DATASTRUCT_LLIST_H_

#ifndef LTEMPLATE
#define LTEMPLATE template<typename Type>
#endif


/// @brief ������
LTEMPLATE
struct LListNode
{
    Type Data; //< ����
    struct LListNode<Type>* PNext; ///< ��һ�����
};

/// @brief ����
LTEMPLATE
class LList
{
public:
	LList();
	~LList();
public:
    /// @brief ������β�����Ԫ��
    void PushBack(const Type& elem);

    /// @brief ��ȡ�����һ�����ָ��
    LListNode<Type>* Begin();

    /// @brief �±������
    ///
    /// ע��÷���������±���������Խ����
    /// @param[in] index �±�����
    /// @return �����±��Ӧ����������
    Type& operator[](int index);

	/// @brief �Ƴ�������Ӧ�Ľ��
    ///
    /// ע��÷���������±���������Խ����
	/// @param index �±�����
	void Remove(int index);

    /// @brief �������
    void Clear();

private:
	LList(const LList<Type>&);
	LList<Type>& operator = (const LList<Type>&);

public:
    int Length; ///< ������

private:
	LListNode<Type> m_heardNode; ///< ͷ���
    LListNode<Type>* m_pTailNode; ///< β���ָ��
};

LTEMPLATE
LList<Type>::LList()
{
	Length = 0;
	m_heardNode.PNext = 0;
    m_pTailNode = 0;
}

LTEMPLATE
LList<Type>::~LList()
{
	Clear();
}

LTEMPLATE
LListNode<Type>* LList<Type>::Begin()
{
    return m_heardNode.PNext;
}

LTEMPLATE
void LList<Type>::Clear()
{
	LListNode<Type>* pNode = m_heardNode.PNext;
	LListNode<Type>* pNodeNext = 0;

	while (pNode)
	{
		pNodeNext = pNode->PNext;
		if (pNode)
            delete pNode;
		pNode = pNodeNext;
	}

	m_heardNode.PNext = NULL;
	Length = 0;
    m_pTailNode = 0;
}

LTEMPLATE
Type& LList<Type>::operator[](int index)
{
    LListNode<Type>* pNode = m_heardNode.PNext;
    for (int i = 0; i < index; i++)
    {
        pNode = pNode->PNext;
    }
    return pNode->Data;
}

LTEMPLATE
void LList<Type>::PushBack(const Type& elem)
{
	LListNode<Type>* pNewNode = new LListNode<Type>;
    pNewNode->Data = elem;
    pNewNode->PNext = 0;
    if (m_pTailNode == 0)
        m_heardNode.PNext = pNewNode;
    else 
        m_pTailNode->PNext = pNewNode;

    m_pTailNode = pNewNode;
    Length += 1;
}

LTEMPLATE
void LList<Type>::Remove(int index)
{
	int i = 0;
	LListNode<Type>* pNode = &m_heardNode;
	while (i < index)
	{
		pNode = pNode->PNext;
		i++;
	}

	LListNode<Type>* pNodeDelete = pNode->PNext;
	pNode->PNext = pNodeDelete->PNext;
	delete pNodeDelete;
	pNodeDelete = NULL;
	Length -= 1;
}

#endif