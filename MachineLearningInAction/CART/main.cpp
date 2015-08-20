
#include "LMachineLearning/LDecisionTree.h"

#include <cstdio>
#include <cstdlib>

int main()
{
    LDecisionTree tree;

    int dataRows = 16;
    int dataCols = 5;

    LDTDataSet dataSet(dataRows);
    for (int i = 0; i < dataRows; i++)
    {
        dataSet[i].Reset(dataCols);
    }

    dataSet[0  ][0].SetStringValue("slashdot");
    dataSet[1  ][0].SetStringValue("google");
    dataSet[2  ][0].SetStringValue("digg");     
    dataSet[3  ][0].SetStringValue("kiwitobes");
    dataSet[4  ][0].SetStringValue("google");   
    dataSet[5  ][0].SetStringValue("(direct)"); 
    dataSet[6  ][0].SetStringValue("(direct)"); 
    dataSet[7  ][0].SetStringValue("google");   
    dataSet[8  ][0].SetStringValue("slashdot");
    dataSet[9  ][0].SetStringValue("digg");    
    dataSet[10][0].SetStringValue("google");  
    dataSet[11][0].SetStringValue("kiwitobes");
    dataSet[12][0].SetStringValue("digg");  
    dataSet[13][0].SetStringValue("slashdot");
    dataSet[14][0].SetStringValue("google"); 
    dataSet[15][0].SetStringValue("kiwitobes");

    dataSet[0  ][1].SetStringValue("USA");
    dataSet[1  ][1].SetStringValue("France");
    dataSet[2  ][1].SetStringValue("USA");
    dataSet[3  ][1].SetStringValue("France");
    dataSet[4  ][1].SetStringValue("UK");
    dataSet[5  ][1].SetStringValue("New Zealand");
    dataSet[6  ][1].SetStringValue("UK");
    dataSet[7  ][1].SetStringValue("USA");
    dataSet[8  ][1].SetStringValue("France");
    dataSet[9  ][1].SetStringValue("USA");
    dataSet[10][1].SetStringValue("UK");
    dataSet[11][1].SetStringValue("UK");
    dataSet[12][1].SetStringValue("New Zealand");
    dataSet[13][1].SetStringValue("UK");
    dataSet[14][1].SetStringValue("UK");
    dataSet[15][1].SetStringValue("France");

    dataSet[0  ][2].SetStringValue("yes");
    dataSet[1  ][2].SetStringValue("yes");
    dataSet[2  ][2].SetStringValue("yes");
    dataSet[3  ][2].SetStringValue("yes");
    dataSet[4  ][2].SetStringValue("no");
    dataSet[5  ][2].SetStringValue("no");
    dataSet[6  ][2].SetStringValue("no");
    dataSet[7  ][2].SetStringValue("no");
    dataSet[8  ][2].SetStringValue("yes");
    dataSet[9  ][2].SetStringValue("no");
    dataSet[10][2].SetStringValue("no");
    dataSet[11][2].SetStringValue("no");
    dataSet[12][2].SetStringValue("yes");
    dataSet[13][2].SetStringValue("no");
    dataSet[14][2].SetStringValue("yes");
    dataSet[15][2].SetStringValue("yes");

    dataSet[0  ][3].SetIntValue(18);
    dataSet[1  ][3].SetIntValue(23);
    dataSet[2  ][3].SetIntValue(24);
    dataSet[3  ][3].SetIntValue(23);
    dataSet[4  ][3].SetIntValue(21);
    dataSet[5  ][3].SetIntValue(12);
    dataSet[6  ][3].SetIntValue(21);
    dataSet[7  ][3].SetIntValue(24);
    dataSet[8  ][3].SetIntValue(19);
    dataSet[9  ][3].SetIntValue(18);
    dataSet[10][3].SetIntValue(18);
    dataSet[11][3].SetIntValue(19);
    dataSet[12][3].SetIntValue(12);
    dataSet[13][3].SetIntValue(21);
    dataSet[14][3].SetIntValue(18);
    dataSet[15][3].SetIntValue(19);

    dataSet[0  ][4].SetStringValue("None");
    dataSet[1  ][4].SetStringValue("Premium");
    dataSet[2  ][4].SetStringValue("Basic");
    dataSet[3  ][4].SetStringValue("Basic");
    dataSet[4  ][4].SetStringValue("Premium");
    dataSet[5  ][4].SetStringValue("None");
    dataSet[6  ][4].SetStringValue("Basic");
    dataSet[7  ][4].SetStringValue("Premium");
    dataSet[8  ][4].SetStringValue("None");
    dataSet[9  ][4].SetStringValue("None");
    dataSet[10][4].SetStringValue("None");
    dataSet[11][4].SetStringValue("None");
    dataSet[12][4].SetStringValue("Basic");
    dataSet[13][4].SetStringValue("None");
    dataSet[14][4].SetStringValue("Basic");
    dataSet[15][4].SetStringValue("Basic");

    tree.BuildTree(dataSet);
    tree.Prune(0.5f);
    tree.PrintTree();

    LDTDataList testData(4);
    testData[0].SetStringValue("(direct)");
    testData[1].SetStringValue("USA");
    testData[2].SetStringValue("yes");
    testData[3].SetIntValue(5);

    map<LVariant, float> resultMap;
    tree.Classify(testData, resultMap);

    for (auto iter = resultMap.begin(); iter != resultMap.end(); iter++)
    {
        LVariant result = iter->first;
        float prob = iter->second;
        if (result.GetType() == LVariant::INT)
            printf("Result: %d, Prob: %f\n", result.GetIntValue(), prob);
        if (result.GetType() == LVariant::STRING)
            printf("Result: %s, Prob: %f\n", result.GetStringValue().c_str(), prob);
    }
    

    system("pause");
    return 0;
}