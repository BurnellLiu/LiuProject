#ifndef _LDATASTRUCT_LTREE_H_
#define _LDATASTRUCT_LTREE_H_

#include "LDefine.h"



/// @brief 二叉查找树模板
///
/// 需要元素类型支持 < , = 操作符
LTEMPLATE
class LBiSTree
{
public:
	LBiSTree();
	virtual ~LBiSTree();

	/// @brief 是否包含指定的元素
	/// @param[in] elem 需要查找的元素
	/// @return 查找成功返回true，查找失败返回false
	bool Contains(const Type& elem) const;

	/// @brief 插入元素到二叉查找树中, 重复元素将被忽略
	/// @param elem 需要插入的元素
	virtual void Insert(const Type& elem);

	/// @brief 移除二叉查找树中指定的元素
	/// @param elem 需要移除的元素
	virtual void Remove(const Type& elem);

	/// @brief 中序遍历二叉查找树
	/// @param[in] VisitTreeNode 对数据元素操作的应用函数
	void InOrderTraverse(void (*VisitTreeNode)(const Type& elem));

	/// @brief 清空二叉查找树
	void Clear();

	/// @brief 查找最小元素
	/// @return 返回最小元素的指针，当该树为空时返回NULL
	const Type* FindMin() const;

	/// @brief 查找最大元素
	/// @return 返回最大元素的指针，当该树为空时返回NULL
	const Type* FindMax() const;

	/// @brief 获取树的深度
	/// @return 树的深度，空树返回0
	virtual int Depth() const;

	/// @brief 二叉查找树是否为空
	/// @return 为空返回 true，不为空返回false
	bool Empty() const;

protected:
	/// @brief 二叉查找树节点
	struct LBiTNode
	{
		Type Elem; ///< 元素
		LBiTNode* LChild; ///< 左孩子节点
		LBiTNode* RChild; ///< 右孩子节点
		int TreeDepth; ///< 树的深度

		LBiTNode(const Type& elem, LBiTNode* pLChild, LBiTNode* pRChild)
		{
			Elem = elem;
			LChild = pLChild;
			RChild = pRChild;
			TreeDepth = 1;
		}
	};

protected:
	LBiTNode* m_pRootNode; ///< 二叉查找树根结点

private:
	/// @brief 清空指定根节点的二叉查找树
	/// @param[in] pTreeNode 根节点
	void clear(LBiTNode*& pTreeNode);

	/// @brief 指定根节点的二叉查找树中是否包含指定的元素
	/// @param[in] elem 需要查找的元素
	/// @param[in] pTreeNode 二叉查找树的根节点
	/// @return 包含返回true, 不包含返回false
	bool contains(const Type& elem, LBiTNode* pTreeNode) const;

	/// @brief 指定根节点的二叉查找树中插入指定的元素
	/// @param[in] elem 需要插入的元素
	/// @param[in] pTreeNode 二叉查找树的根节点
	void insert(const Type& elem, LBiTNode*& pTreeNode);

	/// @brief 指定根节点的二叉查找树中移除指定的元素
	/// @param[in] elem 需要移除的元素
	/// @param[in] pTreeNode 二叉查找树的根节点
	void remove(const Type& elem, LBiTNode*& pTreeNode);

	
	/// @brief 求以pRootNode为根节点的二叉树的深度
	/// @return 二叉查找树深度
	int depth(LBiTNode* pRootNode) const;

	/// @brief 中序遍历二叉查找树
	/// @param[in] pTreeNode 二叉查找树根节点 
	/// @param[in] VisitTreeNode 访问二叉查找树节点的回调函数
	void inOrderTraverseBiSTree(LBiTNode*& pTreeNode, void (*VisitTreeNode)(const Type& pElem));

};

LTEMPLATE
LBiSTree<Type>::LBiSTree()
{
	m_pRootNode = NULL;
}

LTEMPLATE
LBiSTree<Type>::~LBiSTree()
{
	clear(m_pRootNode);
}

LTEMPLATE
int LBiSTree<Type>::depth(LBiTNode* pRootNode) const
{
	int deep = 0;
	int ldeep = 0;
	int rdeep = 0;
	if (pRootNode == NULL)
		return deep;

	ldeep = depth(pRootNode->LChild);
	rdeep = depth(pRootNode->RChild);
	deep = (ldeep > rdeep) ? ldeep : rdeep;
	deep++;
	pRootNode->TreeDepth = deep;
	return deep;
}

LTEMPLATE
int LBiSTree<Type>::Depth() const
{
	return depth(m_pRootNode);
}

LTEMPLATE
bool LBiSTree<Type>::contains(const Type& elem, LBiTNode* pTreeNode) const
{
	if (pTreeNode == NULL)
		return false;

	if (elem < pTreeNode->Elem)
		return contains(elem, pTreeNode->LChild);
	else if (pTreeNode->Elem < elem)
		return contains(elem, pTreeNode->RChild);
	else
		return true;
}

LTEMPLATE
bool LBiSTree<Type>::Contains(const Type& elem) const
{
	return contains(elem, m_pRootNode);
}

LTEMPLATE
void LBiSTree<Type>::insert(const Type& elem, LBiTNode*& pTreeNode)
{
	if (pTreeNode == NULL)
	{
		pTreeNode = new LBiTNode(elem, NULL, NULL);
		assert(pTreeNode != NULL);
		return;
	}

	if (elem < pTreeNode->Elem)
		insert(elem, pTreeNode->LChild);
	else if (pTreeNode->Elem < elem)
		insert(elem, pTreeNode->RChild);
	else
		;
}

LTEMPLATE
void LBiSTree<Type>::Insert(const Type& elem)
{
	insert(elem, m_pRootNode);
}

LTEMPLATE
const Type* LBiSTree<Type>::FindMin() const
{
	LBiTNode* pTreeNode = m_pRootNode;
	if (pTreeNode == NULL)
		return NULL;

	while (pTreeNode->LChild)
	{
		pTreeNode = pTreeNode->LChild;
	}

	return &(pTreeNode->Elem);
}

LTEMPLATE
const Type* LBiSTree<Type>::FindMax() const
{
	LBiTNode* pTreeNode = m_pRootNode;
	if (pTreeNode == NULL)
		return NULL;

	while (pTreeNode->RChild)
	{
		pTreeNode = pTreeNode->RChild;
	}

	return &(pTreeNode->Elem);
}

LTEMPLATE
void LBiSTree<Type>::remove(const Type& elem, LBiTNode*& pTreeNode)
{
	if (pTreeNode == NULL)
		return;

	if (elem < pTreeNode->Elem)
	{
		remove(elem, pTreeNode->LChild);
	}
	else if (pTreeNode->Elem < elem)
	{
		remove(elem, pTreeNode->RChild);
	} 
	else
	{
		if (pTreeNode->LChild != NULL && pTreeNode->RChild != NULL)
		{
			LBiTNode* pPartMax = pTreeNode->LChild;
			while (pPartMax->RChild)
			{
				pPartMax = pPartMax->RChild;
			}

			pTreeNode->Elem = pPartMax->Elem;
			remove(pTreeNode->Elem, pTreeNode->LChild);
		}
		else
		{
			LBiTNode* oldNode = pTreeNode;
			pTreeNode = (pTreeNode->LChild != NULL) ? pTreeNode->LChild : pTreeNode->RChild;
			SAFE_DELETE(oldNode);
		}
	}
}

LTEMPLATE
void LBiSTree<Type>::Remove(const Type& elem)
{
	remove(elem, m_pRootNode);
}

LTEMPLATE
void LBiSTree<Type>::clear(LBiTNode*& pTreeNode)
{
	if (pTreeNode == NULL )
		return;

	clear(pTreeNode->LChild);
	clear(pTreeNode->RChild);
	SAFE_DELETE(pTreeNode);
}

LTEMPLATE
void LBiSTree<Type>::Clear()
{
	clear(m_pRootNode);
}

LTEMPLATE
void LBiSTree<Type>::inOrderTraverseBiSTree(LBiTNode*& pTreeNode, void (*VisitTreeNode)(const Type& pElem))
{
	if (VisitTreeNode == NULL)
		return;

	if (pTreeNode == NULL)
		return;

	inOrderTraverseBiSTree(pTreeNode->LChild, VisitTreeNode);
	VisitTreeNode(pTreeNode->Elem);
	inOrderTraverseBiSTree(pTreeNode->RChild, VisitTreeNode);
}

LTEMPLATE
void LBiSTree<Type>::InOrderTraverse(void (*VisitTreeNode)(const Type& elem))
{
	inOrderTraverseBiSTree(m_pRootNode, VisitTreeNode);
}

LTEMPLATE
bool LBiSTree<Type>::Empty()
{
	if (m_pRootNode == NULL)
		return true;
	else
		return false;
}

/// @brief AVL树模板 
///
/// 需要元素类型支持 < , = 操作符
LTEMPLATE
class LAVLTree : public LBiSTree<Type>
{
public:
	/// @brief 插入元素到AVL树中, 重复元素将被忽略
	/// @param[in] elem 需要插入的元素
	virtual void Insert(const Type& elem);

	/// @brief 获取AVL树的深度
	/// @return AVL树的深度
	virtual int Depth() const;

	/// @brief 移除AVL树中指定的元素
	/// @param[in] elem 需要移除的元素
	virtual void Remove(const Type& elem);

private:
	/// @brief 对以pRootNode为根节点的AVL树作右旋
	/// @param[in] pRootNode AVL树根节点
	void rightRotate(LBiTNode*& pRootNode);

	/// @brief 对以pNode为根节点的AVL树作右旋
	/// @param[in] pNode AVL树根节点
	void leftRotate(LBiTNode*& pRootNode);

	/// @brief 指定根节点的AVL树中插入指定的元素
	/// @param[in] elem 需要插入的元素
	/// @param[in] pTreeNode AVL树的根节点
	void avlInsert(const Type& elem, LBiTNode*& pTreeNode);

	/// @brief 指定根节点的AVL树中移除指定的元素
	/// @param[in] elem 需要移除的元素
	/// @param[in] pTreeNode AVL树的根节点
	void avlRemove(const Type& elem, LBiTNode*& pTreeNode);

	/// @brief 获取指定AVL树的深度
	/// @param pTreeNode AVL树根节点
	/// @return AVL树的深度，空树返回0
	int getDepth(LBiTNode* pTreeNode) const;
};

LTEMPLATE
void LAVLTree<Type>::rightRotate(LBiTNode*& pRootNode)
{
	LBiTNode* pNewRootNode = pRootNode->LChild;
	pRootNode->LChild = pNewRootNode->RChild;
	pNewRootNode->RChild = pRootNode;

	int lDepth = 0;
	int rDepth = 0;

	lDepth = getDepth(pRootNode->LChild);
	rDepth = getDepth(pRootNode->RChild);
	pRootNode->TreeDepth = lDepth > rDepth ? lDepth + 1 : rDepth + 1;

	lDepth = getDepth(pNewRootNode->LChild);
	rDepth = getDepth(pNewRootNode->RChild);
	pNewRootNode->TreeDepth =  lDepth > rDepth ? lDepth + 1 : rDepth + 1;

	pRootNode = pNewRootNode;
}

LTEMPLATE
void LAVLTree<Type>::leftRotate(LBiTNode*& pRootNode)
{
	LBiTNode* pNewRootNode = pRootNode->RChild;
	pRootNode->RChild = pNewRootNode->LChild;
	pNewRootNode->LChild = pRootNode;

	int lDepth = 0;
	int rDepth = 0;

	lDepth = getDepth(pRootNode->LChild);
	rDepth = getDepth(pRootNode->RChild);
	pRootNode->TreeDepth = lDepth > rDepth ? lDepth + 1 : rDepth + 1;

	lDepth = getDepth(pNewRootNode->LChild);
	rDepth = getDepth(pNewRootNode->RChild);
	pNewRootNode->TreeDepth =  lDepth > rDepth ? lDepth + 1 : rDepth + 1;


	pRootNode = pNewRootNode;
}

LTEMPLATE
void LAVLTree<Type>::avlInsert(const Type& elem, LBiTNode*& pTreeNode)
{
	if (pTreeNode == NULL)
	{
		pTreeNode = new LBiTNode(elem, NULL, NULL);
		assert(pTreeNode != NULL);
		return;
	}

	if (elem < pTreeNode->Elem)
	{
		avlInsert(elem, pTreeNode->LChild);

		int lDepth = getDepth(pTreeNode->LChild);
		int rDepth = getDepth(pTreeNode->RChild);
		pTreeNode->TreeDepth = ( lDepth > rDepth) ? lDepth + 1 : rDepth + 1;

		if ((lDepth-rDepth) == 2)
		{
			if (elem < pTreeNode->LChild->Elem)
			{
				rightRotate(pTreeNode);
			}
			else
			{
				leftRotate(pTreeNode->LChild);
				rightRotate(pTreeNode);
			}

		}
		
	}
	else if (pTreeNode->Elem < elem)
	{
		avlInsert(elem, pTreeNode->RChild);

		int lDepth = getDepth(pTreeNode->LChild);
		int rDepth = getDepth(pTreeNode->RChild);
		pTreeNode->TreeDepth = ( lDepth > rDepth) ? lDepth + 1 : rDepth + 1;

		if ((rDepth-lDepth) == 2)
		{
			if (pTreeNode->RChild->Elem < elem)
			{
				leftRotate(pTreeNode);
			}
			else
			{
				rightRotate(pTreeNode->RChild);
				leftRotate(pTreeNode);
			}
		}

	}
	else
	{
		;
	}
}

LTEMPLATE
void LAVLTree<Type>::Insert(const Type& elem)
{
	avlInsert(elem, m_pRootNode);
}

LTEMPLATE
int LAVLTree<Type>::getDepth(LBiTNode* pTreeNode) const
{
	if (pTreeNode)
		return pTreeNode->TreeDepth;
	else
		return 0;
}

LTEMPLATE
int LAVLTree<Type>::Depth() const
{
	return getDepth(m_pRootNode);
}

LTEMPLATE
void LAVLTree<Type>::avlRemove(const Type& elem, LBiTNode*& pTreeNode)
{
	if (pTreeNode == NULL)
		return;

	if (elem < pTreeNode->Elem)
	{
		avlRemove(elem, pTreeNode->LChild);

		int lDepth = getDepth(pTreeNode->LChild);
		int rDepth = getDepth(pTreeNode->RChild);
		pTreeNode->TreeDepth = lDepth > rDepth ? lDepth + 1 : rDepth + 1;
		if ((rDepth-lDepth) == 2)
		{
			if (getDepth(pTreeNode->RChild->RChild) < getDepth(pTreeNode->RChild->LChild))
			{
				rightRotate(pTreeNode->RChild);
				leftRotate(pTreeNode);
			}
			else
			{
				leftRotate(pTreeNode);
			}

		}
	}
	else if (pTreeNode->Elem < elem)
	{
		avlRemove(elem, pTreeNode->RChild);

		int lDepth = getDepth(pTreeNode->LChild);
		int rDepth = getDepth(pTreeNode->RChild);
		pTreeNode->TreeDepth = lDepth > rDepth ? lDepth + 1 : rDepth + 1;
		if ((lDepth-rDepth) == 2)
		{
			if (getDepth(pTreeNode->LChild->LChild) < getDepth(pTreeNode->LChild->RChild))
			{
				leftRotate(pTreeNode->LChild);
				rightRotate(pTreeNode);
			}
			else
			{
				rightRotate(pTreeNode);
			}

		}
	} 
	else
	{
		if (pTreeNode->LChild != NULL && pTreeNode->RChild != NULL)
		{
			LBiTNode* pPartMax = pTreeNode->LChild;
			while (pPartMax->RChild)
			{
				pPartMax = pPartMax->RChild;
			}

			pTreeNode->Elem = pPartMax->Elem;
			avlRemove(pTreeNode->Elem, pTreeNode->LChild);

			int lDepth = getDepth(pTreeNode->LChild);
			int rDepth = getDepth(pTreeNode->RChild);
			pTreeNode->TreeDepth = lDepth > rDepth ? lDepth + 1 : rDepth + 1;
			if ((rDepth-lDepth) == 2)
			{
				if (getDepth(pTreeNode->RChild->RChild) < getDepth(pTreeNode->RChild->LChild))
				{
					rightRotate(pTreeNode->RChild);
					leftRotate(pTreeNode);
				}
				else
				{
					leftRotate(pTreeNode);
				}

			}


		}
		else
		{
			LBiTNode* oldNode = pTreeNode;
			pTreeNode = (pTreeNode->LChild != NULL) ? pTreeNode->LChild : pTreeNode->RChild;
			SAFE_DELETE(oldNode);
		}
	}
}

LTEMPLATE
void LAVLTree<Type>::Remove(const Type& elem)
{
	avlRemove(elem, m_pRootNode);
}


#endif