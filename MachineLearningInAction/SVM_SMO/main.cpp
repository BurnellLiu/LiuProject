
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


    LSVM svm;
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
    

    LSVMProblem problem(sampleMatrix, classVector);
    LSVMKRBF rbf;
    rbf.SetGamma(0.9f);
    svm.TrainModel(problem, &rbf);

    printf("Support Vector Number: %u\n", svm.GetSupportVectorNumber());

    int errorCount = 0;
    LSVMMatrix sample;
    for (unsigned int i = 0; i < sampleMatrix.RowLen; i++)
    {
        sample = sampleMatrix.GetRow(i);
        if (svm.Predict(sample) != classVector[i][0])
        {
            errorCount++;
        }
    }

    printf("ErrorRate: %f\n", (float)errorCount/(float)sampleMatrix.RowLen);
   
    system("pause");
	return 0;
}