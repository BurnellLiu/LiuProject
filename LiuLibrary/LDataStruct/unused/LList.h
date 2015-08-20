
#ifndef _DATASTRUCT_LLIST_H_
#define _DATASTRUCT_LLIST_H_

#ifndef LTEMPLATE
#define LTEMPLATE template<typename Type>
#endif


/// @brief 链表结点
LTEMPLATE
struct LListNode
{
    Type Data; //< 数据
    struct LListNode<Type>* PNext; ///< 下一个结点
};

/// @brief 链表
LTEMPLATE
class LList
{
public:
	LList();
	~LList();
public:
    /// @brief 在连接尾部添加元素
    void PushBack(const Type& elem);

    /// @brief 获取链表第一个结点指针
    LListNode<Type>* Begin();

    /// @brief 下标操作符
    ///
    /// 注意该方法不会对下标索引进行越界检查
    /// @param[in] index 下标索引
    /// @return 返回下标对应的数据引用
    Type& operator[](int index);

	/// @brief 移除索引对应的结点
    ///
    /// 注意该方法不会对下标索引进行越界检查
	/// @param index 下标索引
	void Remove(int index);

    /// @brief 清除链表
    void Clear();

private:
	LList(const LList<Type>&);
	LList<Type>& operator = (const LList<Type>&);

public:
    int Length; ///< 链表长度

private:
	LListNode<Type> m_heardNode; ///< 头结点
    LListNode<Type>* m_pTailNode; ///< 尾结点指针
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