

#include <cstdio>

#include <Windows.h>

#include "LDataStruct/LMatrix.h"

template <typename T>
void Swap(T& a, T& b)
{
    T t = a;
    a = b;
    b = t;
}


void FullArray(char* array, unsigned int arraySize, unsigned int index)
{
    if (index >= arraySize)
    {
        printf("%s\n", array);
        return;
    }

    for (unsigned int i = index; i< arraySize; i++)
    {
        Swap(array[i], array[index]);
        FullArray(array, arraySize, index + 1);
        Swap(array[i], array[index]);
    }
}

int main()
{
    char str[10] = {0};
    str[0] = 'a';
    str[1] = 'b';
    str[2] = 'c';
    str[3] = 'd';

    FullArray(str, 4, 0);

    system("pause");
    return 0;
}