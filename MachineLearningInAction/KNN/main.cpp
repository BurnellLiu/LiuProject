
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>

#include <Windows.h>

#include "LMachineLearning/LKDTree.h"

/// @brief 产生随机小数
/// @param[in] min 随机小数的最小值
/// @param[in] max 随机小数的最大值
/// @return 随机小数
float RandFloat(float min, float max)
{
    return min + (max - min) * rand() / (RAND_MAX + 1.0f);
}

/// @brief 对矩阵进行随机赋值, 范围(min, max)
/// @param[in] matrix
/// @param[in] min 最小值
/// @param[in] max 最大值
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

/// @brief 从数据集中寻找和指定数据最邻近的数据索引
/// @param[in] dataSet 数据集
/// @param[in] data 需要寻找最邻近的数据
/// @return 最邻近索引
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