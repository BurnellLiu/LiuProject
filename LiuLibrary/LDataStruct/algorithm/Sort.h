#ifndef _SORT_H_
#define _SORT_H_

#include "LDefine.h"

#include <vector>
using std::vector;


/// @brief ð������ģ�巽��
///
/// �������ʱ�临�Ӷ�O(n), ����ʱ�临�Ӷ�O(n2)[ƽ��]
/// ȱ�㣺�������ݴ����Ƚ϶�
/// @param[in] rVec ��Ҫ����Ķ�̬����
LTEMPLATE
void BubbleSort(vector<Type>& rVec)
{
	int totalLen= (int)rVec.size();
	
	bool bDisorder = true; // ��������

	for (int i = 0; i < totalLen && bDisorder; i++)
	{
		bDisorder = false; 

		for (int k = totalLen-1; k > i; k--)
		{
			if (rVec[k] < rVec[k-1])
			{
				bDisorder = true; // ��������

				int temp = rVec[k];
				rVec[k] = rVec[k-1];
				rVec[k-1] = temp;
			}
		}
	}
}

/// @brief ��ѡ������
///
/// ʱ�临�Ӷ�ΪO(n2)[ƽ��]���ŵ㽻��������
/// @param[in] rVec ��Ҫ����Ķ�̬����
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

/// @brief ֱ�Ӳ�������ģ��
///
/// �㷨˼�룺��һ����¼���뵽�Ѿ�����õ�������У��Ӷ��õ�һ���µġ���¼����һ�������
/// ʱ�临�Ӷ�O(n2)
/// @param[in] rVec ��Ҫ����Ķ�̬����
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