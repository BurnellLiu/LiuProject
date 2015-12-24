
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
    // ����������ṹ
    LBPNetworkPogology pogology;
    pogology.InputNumber = 2;
    pogology.HiddenLayerNumber = 1;
    pogology.OutputNumber = 1;
    pogology.NeuronsOfHiddenLayer = 2;

    // ��ʼ�������粢������ѧϰ����
    LBPNetwork bpNetwork;
    bpNetwork.Init(pogology);
    bpNetwork.SetLearnRate(2.1f);

    // �����������
    float inputList[8] = 
    {
        1.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
    };

    LNNMatrix input(4, 2, inputList);

    // ����Ŀ���������
    LNNMatrix targetOutput(4, 1);
    targetOutput[0][0] = 1.0f;
    targetOutput[1][0] = 1.0f;
    targetOutput[2][0] = 0.0f;
    targetOutput[3][0] = 0.0f;

    // ѵ��1000
    for (int i = 0; i < 1000; i++)
    {
        bpNetwork.Train(input, targetOutput); 
    }

    // �۲�ѵ����Ľ��
    LNNMatrix output;
    bpNetwork.Active(input, &output);
 
    PrintMatrix(output);

    system("pause");
    return 0;
}