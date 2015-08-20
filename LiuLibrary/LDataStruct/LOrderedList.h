/// @file LOrderedList.h
/// @brief 有序链表模板
/// 
/// Detail: 使用该容器需要容器中的类型支持小于操作符
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

/// @brief 有序链表结点
LTEMPLATE
struct LOrderedListNode
{
public:
    Type Data; //< 数据
    LOrderedListNode<Type>* PPrevious; ///< 上一个节点
    LOrderedListNode<Type>* PNext; ///< 下一个节点
};

/// @brief 有序链表模板
///
/// 需要元素类型支持 < , = 操作符(该链表从小到大排序)
LTEMPLATE
class LOrderedList
{
public:
    LOrderedList();
    ~LOrderedList();

    /// @brief 增加值到链表中
    ///  
    /// @param[in] value 需要增加的值
    void Insert(IN const Type& value);

    /// @brief 获取最小值
    ///  
    /// @return 返回最小值的指针, 链表为空返回0
    const Type* GetMinimun() const;

    /// @brief 获取链表头节点指针
    ///  
    /// @return 链表头节点指针, 如果返回0则表示链表为空
    const LOrderedListNode<Type>* Begin() const;

    /// @brief 获取最大值
    ///  
    /// @return 返回最大值的指针, 链表为空返回0
    const Type* GetMaximun() const;

    /// @brief 获取链表尾节点指针
    ///  
    /// @return 链表尾节点指针, 如果返回0则表示链表为空
    const LOrderedListNode<Type>* End() const;

    /// @brief 获取链表长度
    ///  
    /// @return 链表长度
    unsigned int Size() const;

    /// @brief 移除尾元素
    ///  
    /// @return 成功返回true, 失败返回false(链表中不存在元素)
    bool PopBack();

    /// @brief 移除首元素
    ///  
    /// @return 成功返回true, 失败返回false(链表中不存在元素)
    bool PopFront();


    /// @brief 清除链表
    void Clear();

private:
    unsigned int m_size; ///< 链表长度
    LOrderedListNode<Type>* m_pHeadNode; ///< 头节点
    LOrderedListNode<Type>* m_pTailNode;  ///< 尾节点
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

    // 当前链表为空
    if (0 == this->m_size)
    {
        this->m_pHeadNode = pNewNode;
        this->m_pTailNode = pNewNode;
    }

    // 当前链表大小为1
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

    // 当前链表大小大于等于2
    if (this->m_size >= 2)
    {
        LOrderedListNode<Type>* pCurrentNode = this->m_pHeadNode;
        while (true)
        {
            // 到达链表尾部
            if (0 == pCurrentNode)
            {
                this->m_pTailNode->PNext = pNewNode;
                pNewNode->PPrevious = this->m_pTailNode;
                this->m_pTailNode = pNewNode;
                break;
            }

            // 当前不是新节点需要插入的位置
            if (pCurrentNode->Data < pNewNode->Data)
            {
                LOrderedListNode<Type>* pTemp = pCurrentNode->PNext;
                pCurrentNode = pTemp;
                continue;
            }

            // 找到新节点需要插入的位置
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