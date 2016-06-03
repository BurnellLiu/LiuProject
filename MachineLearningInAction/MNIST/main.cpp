
#include <cstdio>
#include <windows.h>

#include "LMNIST/LMNISTParser.h"
#include "LMachineLearning/LRegression.h"
#include "LMachineLearning/LNeuralNetwork.h"

void PrintMatrix(const LMatrix<float>& matrix)
{
    for (unsigned row = 0; row < matrix.RowLen; row++)
    {
        for (unsigned col = 0; col < matrix.ColumnLen; col++)
        {
            printf("%.2f ", matrix[row][col]);
        }
        printf("\n");
    }
}


int TrainMNISTWithSoftmaxRegression()
{
    // ����ѵ�����ݼ�
    LMNISTImageFile trainImageFile("./Data/train-images.idx3-ubyte");
    LMNISTLabelFile trainLabelFile("./Data/train-labels.idx1-ubyte");

    unsigned int trainImageNum = 0;
    unsigned int imageRow = 0;
    unsigned int imageCol = 0;
    unsigned int imageSize = 0;
    unsigned int trainLabelNum = 0;

    trainImageFile.GetImageNum(trainImageNum);
    trainImageFile.GetImageSize(imageRow, imageCol);
    trainLabelFile.GetLabelNum(trainLabelNum);
    imageSize = imageRow * imageCol;

    printf("Train Image Total Number: %u\n", trainImageNum);
    printf("Train Image Size: Row(%u) Col(%u)\n", imageRow, imageCol);
    printf("Train Label Total Number: %u\n", trainLabelNum);
    if (0 == trainImageNum || trainLabelNum != trainImageNum)
    {
        printf("MNIST Train Data Error!!\n");
        return 0;
    }
    printf("\n");

    // ���ز������ݼ�
    LMNISTImageFile testImageFile("./Data/t10k-images.idx3-ubyte");
    LMNISTLabelFile testLabelFile("./Data/t10k-labels.idx1-ubyte");

    unsigned int testImageNum = 0;
    unsigned int testLabelNum = 0;
    unsigned int testImageRow = 0;
    unsigned int testImageCol = 0;

    testImageFile.GetImageNum(testImageNum);
    testImageFile.GetImageSize(testImageRow, testImageCol);
    testLabelFile.GetLabelNum(testLabelNum);

    printf("Tset Image Total Number: %u\n", testImageNum);
    printf("Test Image Size: Row(%u) Col(%u)\n", testImageRow, testImageCol);
    printf("Test Label Total Number: %u\n", testLabelNum);
    if (0 == testImageNum ||
        testLabelNum != testImageNum ||
        testImageRow != imageRow ||
        testImageCol != imageCol)
    {
        printf("MNIST Test Data Error!!\n");
        return 0;
    }
    printf("\n");

    // ����Softmax�ع�
    unsigned int trainTimes = 5; // ѵ������
    float learnRate = 0.001f; // ѧϰ�ٶ�
    unsigned int totalClass = 10; // �������ķ���
    unsigned int step = 1000; // ÿ��ѵ��ʹ�õ�������

    LRegressionMatrix xMatrix(step, imageSize, 0.0f);
    LRegressionMatrix yMatrix(step, totalClass, 0.0f);
    LSoftmaxRegression softmaxReg(trainImageNum, imageSize, totalClass);

    // ѵ��ģ��
    unsigned char* pImageBuffer = new unsigned char[imageSize * step];
    unsigned char* pLabelBuffer = new unsigned char[step];
    for (unsigned int i = 0; i < trainTimes; i++)
    {
        unsigned int iterCount = trainImageNum / step;
        for (unsigned int j = 0; j < iterCount; j++)
        {
            trainImageFile.GetImage(j * step, step, pImageBuffer);
            trainLabelFile.GetLabel(j * step, step, pLabelBuffer);

            yMatrix.Reset(step, totalClass, 0.0f);
            for (unsigned int row = 0; row < xMatrix.RowLen; row++)
            {
                for (unsigned int col = 0; col < xMatrix.ColumnLen; col++)
                {
                    xMatrix[row][col] = (float)pImageBuffer[row * xMatrix.ColumnLen + col];
                }

                yMatrix[row][pLabelBuffer[row]] = 1.0f;
            }

            softmaxReg.TrainModel(xMatrix, yMatrix, learnRate);

            unsigned int completedIter = i * iterCount + j + 1;
            printf("Train Complete:%3d%%\r", completedIter * 100 / (trainTimes * iterCount));
        }
    }
    printf("\n\n");


    // ʹ�ò��Լ�����ģ��
    xMatrix.Reset(1, imageSize, 0.0f);
    yMatrix.Reset(1, totalClass, 0.0f);
    unsigned int errorCount = 0;
    unsigned char label = 0;
    for (unsigned int i = 0; i < testImageNum; i++)
    {
        testImageFile.GetImage(i, pImageBuffer);
        testLabelFile.GetLabel(i, label);

        for (unsigned int k = 0; k < imageSize; k++)
        {
            xMatrix[0][k] = (float)pImageBuffer[k];
        }

        softmaxReg.Predict(xMatrix, yMatrix);

        float maxProb = 0.0f;
        unsigned int classIndex = 0;
        for (unsigned int col = 0; col < yMatrix.ColumnLen; col++)
        {
            if (yMatrix[0][col] > maxProb)
            {
                maxProb = yMatrix[0][col];
                classIndex = col;
            }
        }

        if (classIndex != (unsigned int)label)
            errorCount++;

        printf("Test Complete:%3d%%\r", (i + 1) * 100 / testImageNum);
    }
    printf("\n\n");

    printf("Eror Rate:%.2f%%\n", (float)errorCount * 100 / (float)testImageNum);

    if (0 != pImageBuffer)
    {
        delete[] pImageBuffer;
        pImageBuffer = 0;
    }

    if (0 != pLabelBuffer)
    {
        delete[] pLabelBuffer;
        pLabelBuffer = 0;
    }

    return 0;
}

int TrainMNISTWithBPNetwork()
{
    // ����ѵ�����ݼ�
    LMNISTImageFile trainImageFile("./Data/train-images.idx3-ubyte");
    LMNISTLabelFile trainLabelFile("./Data/train-labels.idx1-ubyte");

    unsigned int trainImageNum = 0;
    unsigned int imageRow = 0;
    unsigned int imageCol = 0;
    unsigned int imageSize = 0;
    unsigned int trainLabelNum = 0;

    trainImageFile.GetImageNum(trainImageNum);
    trainImageFile.GetImageSize(imageRow, imageCol);
    trainLabelFile.GetLabelNum(trainLabelNum);
    imageSize = imageRow * imageCol;

    printf("Train Image Total Number: %u\n", trainImageNum);
    printf("Train Image Size: Row(%u) Col(%u)\n", imageRow, imageCol);
    printf("Train Label Total Number: %u\n", trainLabelNum);
    if (0 == trainImageNum || trainLabelNum != trainImageNum)
    {
        printf("MNIST Train Data Error!!\n");
        return 0;
    }
    printf("\n");

    // ���ز������ݼ�
    LMNISTImageFile testImageFile("./Data/t10k-images.idx3-ubyte");
    LMNISTLabelFile testLabelFile("./Data/t10k-labels.idx1-ubyte");

    unsigned int testImageNum = 0;
    unsigned int testLabelNum = 0;
    unsigned int testImageRow = 0;
    unsigned int testImageCol = 0;

    testImageFile.GetImageNum(testImageNum);
    testImageFile.GetImageSize(testImageRow, testImageCol);
    testLabelFile.GetLabelNum(testLabelNum);

    printf("Tset Image Total Number: %u\n", testImageNum);
    printf("Test Image Size: Row(%u) Col(%u)\n", testImageRow, testImageCol);
    printf("Test Label Total Number: %u\n", testLabelNum);
    if (0 == testImageNum ||
        testLabelNum != testImageNum ||
        testImageRow != imageRow ||
        testImageCol != imageCol)
    {
        printf("MNIST Test Data Error!!\n");
        return 0;
    }
    printf("\n");

    unsigned int trainTimes = 3; // ѵ������
    float learnRate = 0.1f; // ѧϰ�ٶ�
    unsigned int totalClass = 10; // �������ķ���
    unsigned int step = 1000; // ÿ��ѵ��ʹ�õ�������

    // ���巴�򴫲�����
    LBPNetworkPogology pogolpgy;
    pogolpgy.InputNumber = imageSize;
    pogolpgy.HiddenLayerNumber = 1; // ���ز���Ŀ
    pogolpgy.NeuronsOfHiddenLayer = 40; // ���ز���Ԫ����
    pogolpgy.OutputNumber = totalClass; // �������Ŀ

    LBPNetwork bpnetwork(pogolpgy);

    LRegressionMatrix xMatrix(step, imageSize, 0.0f);
    LRegressionMatrix yMatrix(step, totalClass, 0.0f);

    // ѵ��ģ��
    unsigned char* pImageBuffer = new unsigned char[imageSize * step];
    unsigned char* pLabelBuffer = new unsigned char[step];
    for (unsigned int i = 0; i < trainTimes; i++)
    {
        unsigned int iterCount = trainImageNum / step;
        for (unsigned int j = 0; j < iterCount; j++)
        {
            trainImageFile.GetImage(j * step, step, pImageBuffer);
            trainLabelFile.GetLabel(j * step, step, pLabelBuffer);

            yMatrix.Reset(step, totalClass, 0.0f);
            for (unsigned int row = 0; row < xMatrix.RowLen; row++)
            {
                for (unsigned int col = 0; col < xMatrix.ColumnLen; col++)
                {
                    xMatrix[row][col] = (float)pImageBuffer[row * xMatrix.ColumnLen + col]/255.0f;
                }
                yMatrix[row][pLabelBuffer[row]] = 1.0f;
            }

            
            bpnetwork.Train(xMatrix, yMatrix, learnRate);

            unsigned int completedIter = i * iterCount + j + 1;
            printf("Train Complete:%3d%%\r", completedIter * 100 / (trainTimes * iterCount));
        }
    }
    printf("\n\n");

    
    
    // ʹ�ò��Լ�����ģ��
    xMatrix.Reset(1, imageSize, 0.0f);
    yMatrix.Reset(1, totalClass, 0.0f);
    unsigned int errorCount = 0;
    unsigned char label = 0;
    for (unsigned int i = 0; i < testImageNum; i++)
    {
        testImageFile.GetImage(i, pImageBuffer);
        testLabelFile.GetLabel(i, label);

        for (unsigned int k = 0; k < imageSize; k++)
        {
            xMatrix[0][k] = (float)pImageBuffer[k]/255.0f;
        }

        bpnetwork.Active(xMatrix, &yMatrix);

        float maxProb = 0.0f;
        unsigned int classIndex = 0;
        for (unsigned int col = 0; col < yMatrix.ColumnLen; col++)
        {
            if (yMatrix[0][col] > maxProb)
            {
                maxProb = yMatrix[0][col];
                classIndex = col;
            }
        }

        if (classIndex != (unsigned int)label)
            errorCount++;

        printf("Test Complete:%3d%%\r", (i + 1) * 100 / testImageNum);
    }
    printf("\n\n");
    printf("Eror Rate:%.2f%%\n", (float)errorCount * 100 / (float)testImageNum);

    if (0 != pImageBuffer)
    {
        delete[] pImageBuffer;
        pImageBuffer = 0;
    }

    if (0 != pLabelBuffer)
    {
        delete[] pLabelBuffer;
        pLabelBuffer = 0;
    }

    return 0;
}

int main()
{
    
    TrainMNISTWithBPNetwork();
    system("pause");

    return 0;
}