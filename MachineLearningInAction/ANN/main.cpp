
#include <cstdio>

#include <Windows.h>


#include "LMachineLearning/LNeuralNetwork.h"


void PrintMatrix(const LNNMatrix& m)
{
    for (unsigned int row = 0; row < m.RowLen; row++)
    {
        for (unsigned int col = 0; col < m.ColumnLen; col++)
        {
            printf("%.3f  ", m[row][col]);
        }
        printf("\n");
    }
}


int main()
{
    // 定义神经网络结构
    LBPNetworkPogology pogology;
    pogology.InputNumber = 2;
    pogology.HiddenLayerNumber = 1;
    pogology.OutputNumber = 1;
    pogology.NeuronsOfHiddenLayer = 2;

    // 初始化神经网络并且设置学习速率
    LBPNetwork bpNetwork;
    bpNetwork.Init(pogology);
    bpNetwork.SetLearnRate(2.1f);

    // 定义输入矩阵
    float inputList[8] = 
    {
        1.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
    };

    LNNMatrix input(4, 2, inputList);

    // 定义目标输出矩阵
    LNNMatrix targetOutput(4, 1);
    targetOutput[0][0] = 1.0f;
    targetOutput[1][0] = 1.0f;
    targetOutput[2][0] = 0.0f;
    targetOutput[3][0] = 0.0f;

    // 训练1000
    for (int i = 0; i < 1000; i++)
    {
        bpNetwork.Train(input, targetOutput); 
    }

    // 观察训练后的结果
    LNNMatrix output;
    bpNetwork.Active(input, &output);
 
    PrintMatrix(output);

    system("pause");
    return 0;
}