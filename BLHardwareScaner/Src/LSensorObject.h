/// @file LSensorObject.h
/// @brief ���������� 
/// 
/// Detail: ���ļ�����������LSensorObject���ڼ���SensorObject.dll�ļ�����ȡ����������
/// @author Burnell Liu Email:burnell_liu@outlook.com
/// @version   
/// @date 2016:3:4

#ifndef _LSENSOROBJECT_H_
#define _LSENSOROBJECT_H_

#include <string>
using std::string;
using std::wstring;

// ����
#ifndef IN
#define IN 
#endif

// ���
#ifndef OUT
#define OUT 
#endif

// ��󴫸�����Ŀ
#define MAX_SENSOR_NUMBER 8

// ����ַ�������
#define MAX_STRING_LEN 256

extern "C"
{
    /// <SUMMARY>
    /// �������ٶȴ�����(3D)����
    /// </SUMMARY>
    struct SAccelerometer3DSensorData
    {
        double X_G; // X����������ٶ�(G)
        double Y_G; // Y����������ٶ�(G)
        double Z_G; // Z����������ٶ�(G)
    };

    /// <SUMMARY>
    /// �������ٶȴ�����(3D)��Ϣ��
    /// </SUMMARY>
    struct SAccelerometer3DInforArray
    {
        unsigned int Count; // ����������
        wchar_t FriendlyName[MAX_SENSOR_NUMBER][MAX_STRING_LEN]; // �Ѻ�����
        SAccelerometer3DSensorData Data[MAX_SENSOR_NUMBER]; // ����������
    };

    /// <SUMMARY>
    /// �����Ǵ�����(3D)����
    /// </SUMMARY>
    struct SGyrometer3DSensorData
    {
        double X_DEGREES_PER_SECOND; // X����ٶ�(��ÿ��)
        double Y_DEGREES_PER_SECOND; // Y����ٶ�(��ÿ��)
        double Z_DEGREES_PER_SECOND; // Z����ٶ�(��ÿ��)
    };

    /// <SUMMARY>
    /// �����Ǵ�����(3D)��Ϣ��
    /// </SUMMARY>
    struct SGyrometer3DInforArray
    {
        unsigned int Count; // ����������
        wchar_t FriendlyName[MAX_SENSOR_NUMBER][MAX_STRING_LEN]; // �Ѻ�����
        SGyrometer3DSensorData Data[MAX_SENSOR_NUMBER]; // ����������
    };

    /// <SUMMARY>
    /// ָ���봫����(3D)����
    /// </SUMMARY>
    struct SCompass3DSensorData
    {
        double X_MILLIGAUSS; // X��Ĵų�ǿ��(����˹)
        double Y_MILLIGAUSS; // Y��Ĵų�ǿ��(����˹)
        double Z_MILLIGAUSS; // Z��Ĵų�ǿ��(����˹)
    };

    /// <SUMMARY>
    /// ָ���봫����(3D)��Ϣ��
    /// </SUMMARY>
    struct SCompass3DInforArray
    {
        unsigned int Count; // ����������
        wchar_t FriendlyName[MAX_SENSOR_NUMBER][MAX_STRING_LEN]; // �Ѻ�����
        SCompass3DSensorData Data[MAX_SENSOR_NUMBER]; // ����������
    };

    /// <SUMMARY>
    /// �⴫��������
    /// </SUMMARY>
    struct SAmbientLightSensorData
    {
        float LIGHT_LEVEL; // ����ǿ��
    };

    /// <SUMMARY>
    /// �⴫������Ϣ��
    /// </SUMMARY>
    struct SAmbientLightInforArray
    {
        unsigned int Count; // ����������
        wchar_t FriendlyName[MAX_SENSOR_NUMBER][MAX_STRING_LEN]; // �Ѻ�����
        SAmbientLightSensorData Data[MAX_SENSOR_NUMBER]; // ����������
    };

    /// <SUMMARY>
    /// GPS����������
    /// </SUMMARY>
    struct SGpsSensorData
    {
        double LAYITUDE_DEGREES; // γ��ֵ
        double LONGITUDE_GERREES; // ����ֵ
    };

    /// <SUMMARY>
    /// GPS��������Ϣ��
    /// </SUMMARY>
    struct SGpsInforArray
    {
        unsigned int Count; // ����������
        wchar_t FriendlyName[MAX_SENSOR_NUMBER][MAX_STRING_LEN]; // �Ѻ�����
        SGpsSensorData Data[MAX_SENSOR_NUMBER]; // ����������
    };
};


class CSensorObject;

/// @brief ����������
class LSensorObject
{
public:
    /// @brief ���캯��
    LSensorObject();

    /// @brief ��������
    ~LSensorObject();

    /// <SUMMARY>
    /// ��ȡ�������ٶȴ�����(3D)��Ϣ
    /// </SUMMARY>
    /// <PARAM name = "pInforArray" dir = "OUT">
    /// �洢��������Ϣ
    /// </PARAM>
    /// <RETURNS>
    /// true(��ȡ�ɹ�), false(��ȡʧ��)
    /// </RETURNS>
    bool GetAccelerometer3DInfor(OUT SAccelerometer3DInforArray* pInforArray);

    /// <SUMMARY>
    /// ��ȡ�����Ǵ�����(3D)��Ϣ
    /// </SUMMARY>
    /// <PARAM name = "pInforArray" dir = "OUT">
    /// �洢��������Ϣ
    /// </PARAM>
    /// <RETURNS>
    /// true(��ȡ�ɹ�), false(��ȡʧ��)
    /// </RETURNS>
    bool GetGyrometer3DInfor(OUT SGyrometer3DInforArray* pInforArray);

    /// <SUMMARY>
    /// ��ȡָ���봫����(3D)��Ϣ
    /// </SUMMARY>
    /// <PARAM name = "pInforArray" dir = "OUT">
    /// �洢��������Ϣ
    /// </PARAM>
    /// <RETURNS>
    /// true(��ȡ�ɹ�), false(��ȡʧ��)
    /// </RETURNS>
    bool GetCompass3DInfor(OUT SCompass3DInforArray* pInforArray);

    /// <SUMMARY>
    /// ��ȡ�⴫������Ϣ
    /// </SUMMARY>
    /// <PARAM name = "pInforArray" dir = "OUT">
    /// �洢��������Ϣ
    /// </PARAM>
    /// <RETURNS>
    /// true(��ȡ�ɹ�), false(��ȡʧ��)
    /// </RETURNS>
    bool GetAmbientLightInfor(OUT SAmbientLightInforArray* pInforArray);

    /// <SUMMARY>
    /// ��ȡGPS��������Ϣ
    /// </SUMMARY>
    /// <PARAM name = "pInforArray" dir = "OUT">
    /// �洢��������Ϣ
    /// </PARAM>
    /// <RETURNS>
    /// true(��ȡ�ɹ�), false(��ȡʧ��)
    /// </RETURNS>
    bool GetGpsInfor(OUT SGpsInforArray* pInforArray);

private:
    CSensorObject* m_pSensorObject; ///< ������ʵ�ֶ���
};

#endif