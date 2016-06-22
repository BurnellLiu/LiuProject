
#include <cstdio>
#include <windows.h>

#include "LMNIST/LMNISTParser.h"
#include "LMachineLearning/LRegression.h"
#include "LMachineLearning/LNeuralNetwork.h"
#include "LMachineLearning/LSVM.h"

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
    // 加载训练数据集
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

    // 加载测试数据集
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

    // 定义Softmax回归
    unsigned int trainTimes = 5; // 训练次数
    float learnRate = 0.001f; // 学习速度
    unsigned int totalClass = 10; // 样本集的分类
    unsigned int step = 1000; // 每次训练使用的样本数

    LRegressionMatrix xMatrix(step, imageSize, 0.0f);
    LRegressionMatrix yMatrix(step, totalClass, 0.0f);
    LSoftmaxRegression softmaxReg(trainImageNum, imageSize, totalClass);

    // 训练模型
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


    // 使用测试集测试模型
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
    // 加载训练数据集
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

    // 加载测试数据集
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

    unsigned int trainTimes = 3; // 训练次数
    float learnRate = 0.1f; // 学习速度
    unsigned int totalClass = 10; // 样本集的分类
    unsigned int step = 1000; // 每次训练使用的样本数

    // 定义反向传播网络
    LBPNetworkPogology pogolpgy;
    pogolpgy.InputNumber = imageSize;
    pogolpgy.HiddenLayerNumber = 1; // 隐藏层数目
    pogolpgy.NeuronsOfHiddenLayer = 40; // 隐藏层神经元数量
    pogolpgy.OutputNumber = totalClass; // 输出层数目

    LBPNetwork bpnetwork(pogolpgy);

    LRegressionMatrix xMatrix(step, imageSize, 0.0f);
    LRegressionMatrix yMatrix(step, totalClass, 0.0f);

    // 训练模型
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

    
    
    // 使用测试集测试模型
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

int TrainMNISTWithSVM()
{
	// 加载训练数据集
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

	// 加载测试数据集
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

	// 只对0和1做二分类
	unsigned int trainSampleNum = 400;
	printf("Train 0 And 1 Num: %u\n", trainSampleNum);

	// 加载训练样本集
	LSVMMatrix trainSampleSet(trainSampleNum, imageSize);
	LSVMMatrix trainLabelVector(trainSampleNum, 1);

	unsigned int iter = 0;
	unsigned char* pImageBuffer = new unsigned char[imageSize];
	for (unsigned int i = 0; i < trainLabelNum; i++)
	{
		unsigned char label = 2;
		trainLabelFile.GetLabel(i, label);
		if (label != 0 && label != 1)
			continue;

		if (label == 1)
		{
			trainLabelVector[iter][0] = 1.0f;
		}

		if (label == 0)
		{
			trainLabelVector[iter][0] = -1.0f;
		}

		trainImageFile.GetImage(i, pImageBuffer);
		for (unsigned int col = 0; col < trainSampleSet.ColumnLen; col++)
		{
			trainSampleSet[iter][col] = (float)pImageBuffer[col] / 255.0f;
		}

		iter++;

		if (iter >= trainSampleNum)
			break;
	}

	// 定义核函数
	LSVMKRBF svmRBF(20.0f);

	// 定义SVM参数
	LSVMParam svmParam;
	svmParam.C = 40.0f;
	svmParam.MaxIterCount = 40;
	svmParam.PKernelFunc = &svmRBF;
	LSVMProblem svmProblem(trainSampleSet, trainLabelVector);

	// 定义SVM
	LSVM svm(svmParam);

	// 训练SVM
	LSVMResult svmResult;
	svm.TrainModel(svmProblem, svmResult);
	printf("Train Result:\n");
	printf("\tSupport Vector Num: %u\n", svmResult.SupportVectorNum);
	printf("\tIter Count: %u\n", svmResult.IterCount);

	LSVMMatrix trainYVector;
	svm.Predict(trainSampleSet, trainYVector);

	unsigned int errorCount = 0;
	for (unsigned int i = 0; i < trainSampleSet.RowLen; i++)
	{
		if (trainLabelVector[i][0] != trainYVector[i][0])
			errorCount++;
	}
	printf("Train Error Rate: %.2f%%\n", errorCount * 100.0f / trainSampleSet.RowLen);


	// 使用测试集测试模型
	LSVMMatrix testSample(1, imageSize);
	LSVMMatrix testLabel(1, 1);
	errorCount = 0;
	unsigned int totalCount = 0;
	for (unsigned int i = 0; i < testLabelNum; i++)
	{
		unsigned char label = 2;
		testLabelFile.GetLabel(i, label);
		if (label != 0 && label != 1)
			continue;

		totalCount++;
		testImageFile.GetImage(i, pImageBuffer);
		for (unsigned int col = 0; col < testSample.ColumnLen; col++)
		{
			testSample[0][col] = (float)pImageBuffer[col]/255.0f;
		}

		svm.Predict(testSample, testLabel);
		if (label == 0)
		{
			if (testLabel[0][0] == 1.0f)
				errorCount++;
		}
		if (label == 1)
		{
			if (testLabel[0][0] == -1.0f)
				errorCount++;
		}
	}

	printf("Test 0 And 1 Num: %u\n", totalCount);
	printf("Test Error Rate: %.2f%%\n", errorCount * 100.0f / totalCount);

	return 0;
}

int main()
{
    
    TrainMNISTWithSVM();
    system("pause");

    return 0;
}