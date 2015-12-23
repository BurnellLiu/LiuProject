
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
    LBPNetworkPogology pogology;
    pogology.InputNumber = 2;
    pogology.HiddenLayerNumber = 1;
    pogology.OutputNumber = 1;
    pogology.NeuronsOfHiddenLayer = 1;

    LBPNetwork bpNetwork;
    bpNetwork.Init(pogology);

    float inputList[4] = 
    {
        1.0f, 0.0f,
        0.0f, 1.0f
    };
    LNNMatrix input(2, 2, inputList);

    LNNMatrix output;

    bpNetwork.Active(input, &output);
    PrintMatrix(output);

    system("pause");
    return 0;
}