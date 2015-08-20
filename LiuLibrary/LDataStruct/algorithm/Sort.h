#ifndef _SORT_H_
#define _SORT_H_

#include "LDefine.h"

#include <vector>
using std::vector;


/// @brief 冒泡排序模板方法
///
/// 最优情况时间复杂度O(n), 最坏情况时间复杂度O(n2)[平方]
/// 缺点：交换数据次数比较多
/// @param[in] rVec 需要排序的动态数组
LTEMPLATE
void BubbleSort(vector<Type>& rVec)
{
	int totalLen= (int)rVec.size();
	
	bool bDisorder = true; // 数组无序

	for (int i = 0; i < totalLen && bDisorder; i++)
	{
		bDisorder = false; 

		for (int k = totalLen-1; k > i; k--)
		{
			if (rVec[k] < rVec[k-1])
			{
				bDisorder = true; // 数组无序

				int temp = rVec[k];
				rVec[k] = rVec[k-1];
				rVec[k-1] = temp;
			}
		}
	}
}

/// @brief 简单选择排序
///
/// 时间复杂度为O(n2)[平方]，优点交换次数少
/// @param[in] rVec 需要排序的动态数组
LTEMPLATE
void SimpleSelectSort(vector<Type>& rVec)
{
	int totalLen = (int)rVec.size();

	for (int i = 0; i < totalLen; i ++)
	{
		int minValueIndex = i;

		for (int k = i + 1; k < totalLen; k++)
		{
			if (rVec[minValueIndex] > rVec[k])
				minValueIndex = k;
		}

		if (minValueIndex != i)
		{
			int temp = rVec[minValueIndex];
			rVec[minValueIndex] = rVec[i];
			rVec[i] = temp;
		}
	}
}

/// @brief 直接插入排序模板
///
/// 算法思想：将一个记录插入到已经排序好的有序表中，从而得到一个新的、记录数增一的有序表
/// 时间复杂度O(n2)
/// @param[in] rVec 需要排序的动态数组
LTEMPLATE
void StraightInsertSort(vector<Type>& rVec)
{
	int totalLen = rVec.size();

	for (int i = 1; i < totalLen; i++)
	{
		if (rVec[i-1] > rVec[i])
		{
			int temp = rVec[i];
			int k = i-1;
			for ( ; k >= 0 && rVec[k] > temp; k--)
			{
				rVec[k + 1] = rVec[k];
			}

			rVec[k + 1] = temp;
		}
	}
}

#endif