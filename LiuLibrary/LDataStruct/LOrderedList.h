/// @file LOrderedList.h
/// @brief ��������ģ��
/// 
/// Detail: ʹ�ø�������Ҫ�����е�����֧��С�ڲ�����
/// @author Burnell_Liu  
/// @version   
/// @date 14:8:2015

#ifndef _LDATASTRUCT_LORDEREDLIST_H_
#define _LDATASTRUCT_LORDEREDLIST_H_

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef INOUT
#define INOUT
#endif


#ifndef LTEMPLATE
#define LTEMPLATE template<typename Type>
#endif

/// @brief ����������
LTEMPLATE
struct LOrderedListNode
{
public:
    Type Data; //< ����
    LOrderedListNode<Type>* PPrevious; ///< ��һ���ڵ�
    LOrderedListNode<Type>* PNext; ///< ��һ���ڵ�
};

/// @brief ��������ģ��
///
/// ��ҪԪ������֧�� < , = ������(�������С��������)
LTEMPLATE
class LOrderedList
{
public:
    LOrderedList();
    ~LOrderedList();

    /// @brief ����ֵ��������
    ///  
    /// @param[in] value ��Ҫ���ӵ�ֵ
    void Insert(IN const Type& value);

    /// @brief ��ȡ��Сֵ
    ///  
    /// @return ������Сֵ��ָ��, ����Ϊ�շ���0
    const Type* GetMinimun() const;

    /// @brief ��ȡ����ͷ�ڵ�ָ��
    ///  
    /// @return ����ͷ�ڵ�ָ��, �������0���ʾ����Ϊ��
    const LOrderedListNode<Type>* Begin() const;

    /// @brief ��ȡ���ֵ
    ///  
    /// @return �������ֵ��ָ��, ����Ϊ�շ���0
    const Type* GetMaximun() const;

    /// @brief ��ȡ����β�ڵ�ָ��
    ///  
    /// @return ����β�ڵ�ָ��, �������0���ʾ����Ϊ��
    const LOrderedListNode<Type>* End() const;

    /// @brief ��ȡ������
    ///  
    /// @return ������
    unsigned int Size() const;

    /// @brief �Ƴ�βԪ��
    ///  
    /// @return �ɹ�����true, ʧ�ܷ���false(�����в�����Ԫ��)
    bool PopBack();

    /// @brief �Ƴ���Ԫ��
    ///  
    /// @return �ɹ�����true, ʧ�ܷ���false(�����в�����Ԫ��)
    bool PopFront();


    /// @brief �������
    void Clear();

private:
    unsigned int m_size; ///< ������
    LOrderedListNode<Type>* m_pHeadNode; ///< ͷ�ڵ�
    LOrderedListNode<Type>* m_pTailNode;  ///< β�ڵ�
};



LTEMPLATE
LOrderedList<Type>::LOrderedList()
{
    this->m_size = 0;
    this->m_pHeadNode = 0;
    this->m_pTailNode = 0;
}

LTEMPLATE
LOrderedList<Type>::~LOrderedList()
{
   this->Clear();
}

LTEMPLATE
void LOrderedList<Type>::Insert(IN const Type& value)
{
    LOrderedListNode<Type>* pNewNode = new LOrderedListNode<Type>;
    pNewNode->Data = value;
    pNewNode->PNext = 0;
    pNewNode->PPrevious = 0;

    // ��ǰ����Ϊ��
    if (0 == this->m_size)
    {
        this->m_pHeadNode = pNewNode;
        this->m_pTailNode = pNewNode;
    }

    // ��ǰ�����СΪ1
    if (1 == this->m_size)
    {
        if (this->m_pHeadNode->Data < pNewNode->Data)
        {
            this->m_pHeadNode->PNext = pNewNode;
            this->m_pTailNode = pNewNode;
            this->m_pTailNode->PPrevious = this->m_pHeadNode;
        }
        else
        {
            this->m_pTailNode = this->m_pHeadNode;
            this->m_pTailNode->PPrevious = pNewNode;
            this->m_pTailNode->PNext = 0;

            this->m_pHeadNode = pNewNode;
            this->m_pHeadNode->PNext = this->m_pTailNode;
            this->m_pHeadNode->PPrevious = 0;
        }
    }

    // ��ǰ�����С���ڵ���2
    if (this->m_size >= 2)
    {
        LOrderedListNode<Type>* pCurrentNode = this->m_pHeadNode;
        while (true)
        {
            // ��������β��
            if (0 == pCurrentNode)
            {
                this->m_pTailNode->PNext = pNewNode;
                pNewNode->PPrevious = this->m_pTailNode;
                this->m_pTailNode = pNewNode;
                break;
            }

            // ��ǰ�����½ڵ���Ҫ�����λ��
            if (pCurrentNode->Data < pNewNode->Data)
            {
                LOrderedListNode<Type>* pTemp = pCurrentNode->PNext;
                pCurrentNode = pTemp;
                continue;
            }

            // �ҵ��½ڵ���Ҫ�����λ��
            LOrderedListNode<Type>* pLeftNode = pCurrentNode->PPrevious;
            LOrderedListNode<Type>* pRightNode = pCurrentNode;
            pNewNode->PNext = pRightNode;
            pRightNode->PPrevious = pNewNode;
            if (0 == pLeftNode)
            {
                this->m_pHeadNode = pNewNode;
            }
            else
            {
                pLeftNode->PNext = pNewNode;
                pNewNode->PPrevious = pLeftNode;
            }

            break;
        }
    }

    this->m_size += 1;
}

LTEMPLATE
const Type* LOrderedList<Type>::GetMinimun() const
{
    if (0 == this->m_pHeadNode)
        return 0;

    return &this->m_pHeadNode->Data;
}

LTEMPLATE
const LOrderedListNode<Type>* LOrderedList<Type>::Begin() const
{
    return this->m_pHeadNode;
}

LTEMPLATE
const Type* LOrderedList<Type>::GetMaximun() const
{
    if (0 == this->m_pTailNode)
        return 0;
    return &this->m_pTailNode->Data;
}

LTEMPLATE
const LOrderedListNode<Type>* LOrderedList<Type>::End() const
{
    return this->m_pTailNode;
}

LTEMPLATE
unsigned int LOrderedList<Type>::Size() const
{
    return this->m_size;
}

LTEMPLATE
bool LOrderedList<Type>::PopFront()
{
    if (0 == this->m_size)
        return false;

    if (1 == this->m_size)
    {
        delete this->m_pHeadNode;
        this->m_pHeadNode = 0;
        this->m_pTailNode = 0;
        this->m_size = 0;
        return true;
    }

    if (2 == this->m_size)
    {
        delete this->m_pHeadNode;
        this->m_pHeadNode = this->m_pTailNode;
        this->m_pHeadNode->PPrevious = 0;
        this->m_size = 1;
        return true;
    }

    LOrderedListNode<Type>* pTempNode = this->m_pHeadNode;
    this->m_pHeadNode = pTempNode->PNext;
    this->m_pHeadNode->PPrevious = 0;
    delete pTempNode;
    pTempNode = 0;
    this->m_size -= 1;

    return true;
}

LTEMPLATE
bool LOrderedList<Type>::PopBack()
{
    if (0 == this->m_size)
        return false;

    if (1 == this->m_size)
    {
        delete this->m_pTailNode;
        this->m_pHeadNode = 0;
        this->m_pTailNode = 0;
        this->m_size = 0;
        return true;
    }

    if (2 == this->m_size)
    {
        delete this->m_pTailNode;
        this->m_pTailNode = this->m_pHeadNode;
        this->m_pTailNode->PNext = 0;
        this->m_size = 1;
        return true;
    }

    LOrderedListNode<Type>* pTempNode = this->m_pTailNode;
    this->m_pTailNode = pTempNode->PPrevious;
    this->m_pTailNode->PNext = 0;
    delete pTempNode;
    pTempNode = 0;
    this->m_size -= 1;

    return true;

}

LTEMPLATE
void LOrderedList<Type>::Clear()
{
    LOrderedListNode<Type>* pCurrent = m_pHeadNode;
    while (pCurrent)
    {
        LOrderedListNode<Type>* pTemp = pCurrent->PNext;
        delete pCurrent;
        pCurrent = pTemp;
    }

    m_pHeadNode = 0;
    m_pTailNode = 0;
    m_size = 0;
}
#endif