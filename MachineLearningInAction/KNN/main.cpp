
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

int main()
{
    srand((int)time(0));
    LKDTreeMatrix dataSet(10, 3);
    LKDTreeMatrix data(1, 3);
    data[0][0] = 3.0f;
    data[0][1] = 2.5f;
    data[0][2] = -5.0f;
    LKDTree tree;

    int testTimes = 1000;
    while (testTimes > 0)
    {
        testTimes--;

        for (unsigned int m = 0; m < dataSet.RowLen; m++)
        {
            for (unsigned int n = 0; n < dataSet.ColumnLen; n++)
            {
                dataSet[m][n] = RandFloat(-10.0f, 10.0f);
            }
        }

        tree.BuildTree(dataSet);

        LKDTreeList indexList;
        tree.SearchKNearestNeighbors(data, 3, indexList);
        int forecastNearestDataIndex = indexList[0][0];
        //int forecastNearestDataIndex = tree.SearchNearestNeighbor(data);
        printf("Forecast Nearest Index: %d\n", forecastNearestDataIndex);
        float forecastNearestDistance = 0.0f;
        for (unsigned int i = 0; i < dataSet.ColumnLen; i++)
        {
            printf("%f  ", dataSet[(unsigned int)forecastNearestDataIndex][i]);
            float dif = dataSet[forecastNearestDataIndex][i] - data[0][i];
            forecastNearestDistance += dif * dif;
        }
        forecastNearestDistance = sqrtf(forecastNearestDistance);
        printf("\n");
        printf("Forecast Nearest Distance: %f\n", forecastNearestDistance);


        printf("\n");

        int actualNearestNodeIndex = -1;
        float actualNearestDistance = 10000.0f;

        for (unsigned int m = 0; m < dataSet.RowLen; m++)
        {
            float sqrSum = 0.0f;
            for (unsigned int n = 0; n < dataSet.ColumnLen; n++)
            {
                float dif = dataSet[m][n] - data[0][n];
                sqrSum += dif * dif;
            }
            sqrSum = sqrtf(sqrSum);
            if (sqrSum < actualNearestDistance)
            {
                actualNearestDistance = sqrSum;
                actualNearestNodeIndex = (int)m;
            }
        }

        printf("Actual Nearest Index: %d\n", actualNearestNodeIndex);
        for (unsigned int i = 0; i < dataSet.ColumnLen; i++)
        {
            printf("%f  ", dataSet[(unsigned int)actualNearestNodeIndex][i]);
        }
        printf("\n");
        printf("Actual Nearest Distance: %f\n", actualNearestDistance);

        printf("\n............................................................\n\n");

        if (actualNearestNodeIndex == forecastNearestDataIndex)
            continue;

        printf("Data: \n");
        for (unsigned int m = 0; m < dataSet.RowLen; m++)
        {
            printf("Index: %u  ", m);
            for (unsigned int n = 0; n < dataSet.ColumnLen; n++)
            {
                printf("%f  ", dataSet[m][n]);
            }
            printf("\n");
        }

        break;
    }
   
    system("pause");
    return 0;
}