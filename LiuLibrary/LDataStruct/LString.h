
#ifndef _LSTRING_H_
#define _LSTRING_H_

#include <string>
using std::string;
#include <sstream>
using std::stringstream;
#include <algorithm>  
using std::transform;


#include "LDataStruct/LArray.h"

typedef LArray<string> LStringList;

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

/// <SUMMARY>
/// 将字符串转换为大写模式
/// </SUMMARY>
inline void ToUpperString(INOUT string& str)  
{  
	transform(str.begin(), str.end(), str.begin(), toupper);  
}

/// <SUMMARY>
/// 将字符串转换为小写模式
/// </SUMMARY>
inline void ToLowerString(INOUT string& str)  
{  
	transform(str.begin(), str.end(), str.begin(), tolower);  
}  

/// <SUMMARY>
/// 字符串裁剪
/// 去除字符串开始和结束的空白字符, 如: 空格, 换行, 回车, 制表符
/// </SUMMARY>
inline string& StringTrimmed(string& str) 
{
	if (str.empty()) 
	{
		return str;
	}

	unsigned int i = 0;
	while (isspace(str[i]) != 0 )
	{
		i++;
	}
	str.erase(0, i);

	if (str.empty()) 
	{
		return str;
	}

	unsigned int j = str.length() - 1;
	while (isspace(str[j]) != 0)
	{
		if (j <= 0)
			break;

		j--;
	}

	str.erase(j + 1);

	return str;
}


/// @brief 字符串分割
/// @param[in] ch 分割字符
/// @param[in] srcStr 源字符串
/// @param[in] strList 分割后的字符串列表
inline void StringSplit(IN const char ch, IN const char* srcStr, OUT LStringList& strList)
{
    int srcLen = strlen(srcStr);
    char* srcBuffer = new char[srcLen + 1];
    for (int i = 0; i < srcLen; i++)
    {
        srcBuffer[i] = srcStr[i];
    }
    srcBuffer[srcLen] = '\0';

    // 获取分割后的字符串列表长度
    int strListLen = 1;
    for (int i = 0; i < srcLen; i++)
    {
        if (srcStr[i] == ch)
        {
            srcBuffer[i] = '\0';
            strListLen++;
        }
    }
    strList.Reset(strListLen);

    int listIndex = 0;
    int begin = 0;
    for (int i = 0; i < srcLen; i++)
    {
        if (srcStr[i] == ch)
        {
            strList.Data[listIndex++] = &srcBuffer[begin];
            begin = i + 1;
        }
    }
    strList.Data[listIndex++] = &srcBuffer[begin];

    delete[] srcBuffer;
}


template<typename T>
void ConvertToString(IN T value, OUT string& str)
{
    stringstream strStream;
    strStream << value;
    str = strStream.str();
};

template<typename T>
string ConvertToString(IN T value)
{
    stringstream strStream;
    strStream << value;
    return strStream.str();
}

template<typename T>
T ConvertFromString(IN const string& str)
{
    T value;
    stringstream strStream(str);
    strStream >> value;
    return value;
}

#endif

