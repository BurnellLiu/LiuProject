
#include <cstdio>
#include <cstdlib>
#include <vector>
using std::vector;

#include <Windows.h>

#include "LMachineLearning/LSVM.h"
#include "LDataStruct/LMatrix.h"
#include "LFile/LFile.h"
#include "LDataStruct/LString.h"

int main()
{
    
    vector<string> lineList;
    LFileReader fileReader(".\\testSetRBF2.txt");
    fileReader.Open();
    while (!fileReader.AtEnd())
    {
        string line;
        fileReader.ReadLine(line);
        if (line.empty())
            continue;

        lineList.push_back(line);
      
    }
    fileReader.Close();


    
    LSVMMatrix sampleMatrix(lineList.size(), 2);
    LSVMMatrix classVector(lineList.size(), 1);

    for (unsigned int i = 0; i < lineList.size(); i++)
    {
        LStringList strList;
        StringSplit('\t', lineList[i].c_str(), strList);
        sampleMatrix[i][0] = ConvertFromString<float>(strList[0]);
        sampleMatrix[i][1] = ConvertFromString<float>(strList[1]);
        classVector[i][0] = ConvertFromString<float>(strList[2]);
    }
    

	LSVMKRBF rbf(0.8f);
	LSVMParam param;
	param.C = 50;
	param.MaxIterCount = 40;
	param.PKernelFunc = &rbf;

	LSVM svm(param);
    LSVMProblem problem(sampleMatrix, classVector);
	LSVMResult result;

    svm.TrainModel(problem, result);

    printf("Support Vector Number: %u\n", result.SupportVevtorNum);
	printf("Iter Count: %u\n", result.IterCount);

	LSVMMatrix yVector;
	svm.Predict(sampleMatrix, yVector);

    int errorCount = 0;
    for (unsigned int i = 0; i < sampleMatrix.RowLen; i++)
    {
		if (yVector[i][0] != classVector[i][0])
			errorCount++;
    }

    printf("ErrorRate: %f\n", (float)errorCount/(float)sampleMatrix.RowLen);
   
    //system("pause");
	return 0;
}