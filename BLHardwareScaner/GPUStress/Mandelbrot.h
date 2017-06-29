/*
 * Author: Burnell Liu  Date:  Email: burnell_liu@outlook.com 
 * Description:
 * 该头文件中声明了使用GPU计算曼德勃罗特集的方法
 */

#pragma once

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
/// 曼德勃罗特图像结构
/// </SUMMARY>
struct MandelbrotImage
{
    unsigned int Width; // 图像宽
    unsigned int Height; // 图像高
    unsigned int* PData; // 存储图像数据, 请自行分配和释放该内存缓冲区, 应该分配Width*Height大小的内存
};

/// <SUMMARY>
/// 曼德勃罗特参数结构
/// </SUMMARY>
struct MandelbrotParam
{
    double RealMin; // 实部最小值
    double ImgMin; // 虚部最小值
    double RealMax; // 实部最大值
    double ImgMax; // 虚部最大值
    unsigned int MaxIter; // 最大迭代次数
};

/// <SUMMARY>
/// 生成曼德勃罗特集
/// </SUMMARY>
/// <PARAM name="param" dir="IN">
/// 曼德勃罗特计算参数
/// </PARAM>
/// <PARAM name="image" dir="INOUT">
/// 曼德勃罗特图像
/// 计算结束后会填充PData
/// </PARAM>
/// <RETURN>
/// 参数有误返回false
/// </RETURN>
bool GPUGenerateMandelbrot(IN MandelbrotParam& param, INOUT MandelbrotImage& image);
    
