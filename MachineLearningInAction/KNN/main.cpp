
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>

#include <Windows.h>

#include "LMachineLearning/LKDTree.h"

/// @brief �������С��
/// @param[in] min ���С������Сֵ
/// @param[in] max ���С�������ֵ
/// @return ���С��
float RandFloat(float min, float max)
{
    return min + (max - min) * rand() / (RAND_MAX + 1.0f);
}

/// @brief �Ծ�����������ֵ, ��Χ(min, max)
/// @param[in] matrix
/// @param[in] min ��Сֵ
/// @param[in] max ���ֵ
void RandMatrix(LKDTreeMatrix& matrix, float min, float max)
{
	for (unsigned int row = 0; row < matrix.RowLen; row++)
	{
		for (unsigned int col = 0; col < matrix.ColumnLen; col++)
		{
			matrix[row][col] = RandFloat(min, max);
		}
	}
}

/// @brief �����ݼ���Ѱ�Һ�ָ���������ڽ�����������
/// @param[in] dataSet ���ݼ�
/// @param[in] data ��ҪѰ�����ڽ�������
/// @return ���ڽ�����
unsigned int FindNearestNeighbor(LKDTreeMatrix& dataSet, LKDTreeMatrix& data)
{
	float sqrSum = 0.0f;
	for (unsigned int col = 0; col < dataSet.ColumnLen; col++)
	{
		float dif = dataSet[0][col] - data[0][col];
		sqrSum += dif * dif;
	}
	sqrSum = sqrtf(sqrSum);

	unsigned int nnIndex = 0;
	float nearestDistance = sqrSum;

	for (unsigned int row = 0; row < dataSet.RowLen; row++)
	{
		float sqrSum = 0.0f;
		for (unsigned int col = 0; col < dataSet.ColumnLen; col++)
		{
			float dif = dataSet[row][col] - data[0][col];
			sqrSum += dif * dif;
		}
		sqrSum = sqrtf(sqrSum);
		if (sqrSum < nearestDistance)
		{
			nearestDistance = sqrSum;
			nnIndex = (int)row;
		}
	}

	return nnIndex;
}

int main()
{
    //srand((int)time(0));
    LKDTreeMatrix dataSet(10, 3, 0.0f);
    LKDTreeMatrix data(1, 3);
    data[0][0] = 3.0f;
    data[0][1] = 2.5f;
    data[0][2] = -5.0f;


    LKDTree tree;

	LKDTreeList indexList;

	for (unsigned int i = 0; i < 700; i++)
	{
		printf("Test Index: %u\n", i);

		RandMatrix(dataSet, -10.0f, 10.0f);

		tree.BuildTree(dataSet);

		
		tree.SearchKNearestNeighbors(data, 3, indexList);
		int predictNN = indexList[0][0];

		int actualNN = FindNearestNeighbor(dataSet, data);

		if (actualNN != predictNN)
		{
			printf("Fail\n");
			break;
		}

	}
   
    system("pause");
    return 0;
}