/*
 * Author: Burnell Liu  Date:  Email: burnell_liu@outlook.com 
 * Description:
 * ��ͷ�ļ���������ʹ��GPU�������²����ؼ��ķ���
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
/// ���²�����ͼ��ṹ
/// </SUMMARY>
struct MandelbrotImage
{
    unsigned int Width; // ͼ���
    unsigned int Height; // ͼ���
    unsigned int* PData; // �洢ͼ������, �����з�����ͷŸ��ڴ滺����, Ӧ�÷���Width*Height��С���ڴ�
};

/// <SUMMARY>
/// ���²����ز����ṹ
/// </SUMMARY>
struct MandelbrotParam
{
    double RealMin; // ʵ����Сֵ
    double ImgMin; // �鲿��Сֵ
    double RealMax; // ʵ�����ֵ
    double ImgMax; // �鲿���ֵ
    unsigned int MaxIter; // ����������
};

/// <SUMMARY>
/// �������²����ؼ�
/// </SUMMARY>
/// <PARAM name="param" dir="IN">
/// ���²����ؼ������
/// </PARAM>
/// <PARAM name="image" dir="INOUT">
/// ���²�����ͼ��
/// �������������PData
/// </PARAM>
/// <RETURN>
/// �������󷵻�false
/// </RETURN>
bool GPUGenerateMandelbrot(IN MandelbrotParam& param, INOUT MandelbrotImage& image);
    
