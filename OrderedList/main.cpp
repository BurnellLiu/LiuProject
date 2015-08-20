
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cassert>

#include <list>
using std::list;

#include <Windows.h>

#include "LDataStruct/LOrderedList.h"


/// @brief ʹ��ʱ����Ϊ���������
static void SRandTime()
{
    srand((int)time(0));
}

/// @brief �����������
/// @param[in] min �����������Сֵ
/// @param[in] max ������������ֵ
/// @return �������
static int RandInt(int min, int max) 
{
    return rand()%(max - min + 1) + min;
}

int main()
{
    list<int> vec;
    int testArray[10] = {0};
    LOrderedList<int> list;
    SRandTime();

    while (true)
    {
        for (int i = 0; i < 10; i++)
        {
            testArray[i] = RandInt(-10, 10);
            printf("%3d  ", testArray[i]);
            list.Insert(testArray[i]);
        }
        printf("\n");

        const LOrderedListNode<int>* pNode = list.End();
        while (pNode)
        {
            if (pNode->PPrevious)
            {
                assert(pNode->PPrevious->Data <= pNode->Data);
            }
            printf("%3d  ", pNode->Data);
            pNode = pNode->PPrevious;
        }
        printf("\n\n");

        while (list.Size() > 0)
        {
            list.PopBack();
            pNode = list.End();
            while (pNode)
            {
                printf("%3d  ", pNode->Data);
                pNode = pNode->PPrevious;
            }
            printf("\n\n");

            list.PopFront();
            pNode = list.End();
            while (pNode)
            {
                printf("%3d  ", pNode->Data);
                pNode = pNode->PPrevious;
            }
            printf("\n\n");
        }

        Sleep(1000);
        break;

    }

  
    

    system("pause");
    return 0;
}