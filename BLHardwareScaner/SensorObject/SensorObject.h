


#ifndef _SENSOROBJECT_H_
#define _SENSOROBJECT_H_

#ifdef _EXPORTING_SENSOR
#define SENSOR_API_DECLSPEC __declspec(dllexport)
#else
#define SENSOR_API_DECLSPEC __declspec(dllimport)
#endif

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

/// <SUMMARY>
/// �������ٶȴ�����(3D)����
/// </SUMMARY>
struct SENSOR_API_DECLSPEC SAccelerometer3DSensorData
{
    double X_G; // X����������ٶ�(G)
    double Y_G; // Y����������ٶ�(G)
    double Z_G; // Z����������ٶ�(G)
};

/// <SUMMARY>
/// �������ٶȴ�����(3D)��Ϣ��
/// </SUMMARY>
struct SENSOR_API_DECLSPEC SAccelerometer3DInforArray
{
    unsigned int Count; // ����������
    SAccelerometer3DSensorData Data[MAX_SENSOR_NUMBER];
};

/// <SUMMARY>
/// �����Ǵ�����(3D)����
/// </SUMMARY>
struct SENSOR_API_DECLSPEC SGyrometer3DSensorData
{
    double X_DEGREES_PER_SECOND; // X����ٶ�(��ÿ��)
    double Y_DEGREES_PER_SECOND; // Y����ٶ�(��ÿ��)
    double Z_DEGREES_PER_SECOND; // Z����ٶ�(��ÿ��)
};

/// <SUMMARY>
/// �����Ǵ�����(3D)��Ϣ��
/// </SUMMARY>
struct SENSOR_API_DECLSPEC SGyrometer3DInforArray
{
    unsigned int Count; // ����������
    SGyrometer3DSensorData Data[MAX_SENSOR_NUMBER];
};

/// <SUMMARY>
/// ָ���봫����(3D)����
/// </SUMMARY>
struct SENSOR_API_DECLSPEC SCompass3DSensorData
{
    double X_MILLIGAUSS; // X��Ĵų�ǿ��(����˹)
    double Y_MILLIGAUSS; // Y��Ĵų�ǿ��(����˹)
    double Z_MILLIGAUSS; // Z��Ĵų�ǿ��(����˹)
};

/// <SUMMARY>
/// ָ���봫����(3D)��Ϣ��
/// </SUMMARY>
struct SENSOR_API_DECLSPEC SCompass3DInforArray
{
    unsigned int Count; // ����������
    SCompass3DSensorData Data[MAX_SENSOR_NUMBER];
};

/// <SUMMARY>
/// �⴫��������
/// </SUMMARY>
struct SENSOR_API_DECLSPEC SAmbientLightSensorData
{
    float LIGHT_LEVEL; // ����ǿ��
};

/// <SUMMARY>
/// �⴫������Ϣ��
/// </SUMMARY>
struct SENSOR_API_DECLSPEC SAmbientLightInforArray
{
    unsigned int Count; // ����������
    SAmbientLightSensorData Data[MAX_SENSOR_NUMBER];
};

/// <SUMMARY>
/// GPS����������
/// </SUMMARY>
struct SENSOR_API_DECLSPEC SGpsSensorData
{
    double LAYITUDE_DEGREES; // γ��ֵ
    double LONGITUDE_GERREES; // ����ֵ
};

/// <SUMMARY>
/// GPS��������Ϣ��
/// </SUMMARY>
struct SENSOR_API_DECLSPEC SGpsInforArray
{
    unsigned int Count; // ����������
    SGpsSensorData Data[MAX_SENSOR_NUMBER];
};

extern "C"
{
    /// <SUMMARY>
    /// ��ȡ�������ٶȴ�����(3D)��Ϣ
    /// </SUMMARY>
    /// <PARAM name = "pInforArray" dir = "OUT">
    /// �洢��������Ϣ
    /// </PARAM>
    /// <RETURNS>
    /// true(��ȡ�ɹ�), false(��ȡʧ��)
    /// </RETURNS>
    SENSOR_API_DECLSPEC bool GetAccelerometer3DSensorInfor(OUT SAccelerometer3DInforArray* pInforArray);

    /// <SUMMARY>
    /// ��ȡ�����Ǵ�����(3D)��Ϣ
    /// </SUMMARY>
    /// <PARAM name = "pInforArray" dir = "OUT">
    /// �洢��������Ϣ
    /// </PARAM>
    /// <RETURNS>
    /// true(��ȡ�ɹ�), false(��ȡʧ��)
    /// </RETURNS>
    SENSOR_API_DECLSPEC bool GetGyrometer3DSensorInfor(OUT SGyrometer3DInforArray* pInforArray);

    /// <SUMMARY>
    /// ��ȡָ���봫����(3D)��Ϣ
    /// </SUMMARY>
    /// <PARAM name = "pInforArray" dir = "OUT">
    /// �洢��������Ϣ
    /// </PARAM>
    /// <RETURNS>
    /// true(��ȡ�ɹ�), false(��ȡʧ��)
    /// </RETURNS>
    SENSOR_API_DECLSPEC bool GetCompass3DSensorInfor(OUT SCompass3DInforArray* pInforArray);

    /// <SUMMARY>
    /// ��ȡ�⴫������Ϣ
    /// </SUMMARY>
    /// <PARAM name = "pInforArray" dir = "OUT">
    /// �洢��������Ϣ
    /// </PARAM>
    /// <RETURNS>
    /// true(��ȡ�ɹ�), false(��ȡʧ��)
    /// </RETURNS>
    SENSOR_API_DECLSPEC bool GetAmbientLightSensorInfor(OUT SAmbientLightInforArray* pInforArray);

    /// <SUMMARY>
    /// ��ȡGPS��������Ϣ
    /// </SUMMARY>
    /// <PARAM name = "pInforArray" dir = "OUT">
    /// �洢��������Ϣ
    /// </PARAM>
    /// <RETURNS>
    /// true(��ȡ�ɹ�), false(��ȡʧ��)
    /// </RETURNS>
    SENSOR_API_DECLSPEC bool GetGpsSensorInfor(OUT SGpsInforArray* pInforArray);
}


#endif