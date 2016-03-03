
#define _EXPORTING_SENSOR
#include "SensorObject.h"

#include <string>
using std::string;

#include <Sensorsapi.h>
#include <Sensors.h>

#include <objbase.h>


#pragma comment(lib, "Sensorsapi.lib")

#define SAFE_RELEASE(p) do {if (p){ (p)->Release(); (p) = 0;}}while(0)
#define SAFE_DEL_ARRAY(p) do {if (p){ delete[] (p); (p) = 0;}}while(0)

/// <SUMMARY>
/// ��ʼ��COM��(���߳��׼�)
/// </SUMMARY>
class CComInitS
{
public:
    CComInitS()
        : m_success(false)
    {
        // CoInitialize���ú󷵻�S_FALSE��S_OK�������CoUninitialize
        // CoInitialize���ú󷵻�RPC_E_CHANGE_MODE, ������ǰ�߳��ѱ���ʼ��(�Һ͵�ǰģʽ��ͬ),����Ҫ����CoUninitialize
        HRESULT hr = CoInitialize(0);
        if (hr == S_FALSE || hr == S_OK)
        {
            m_success = true;
        }
    }

    virtual ~CComInitS()
    {
        if (m_success)
            CoUninitialize();
    }
private:
    bool m_success; // ��ʶCOM���Ƿ��ʼ���ɹ�
    CComInitS(const CComInitS&);
    CComInitS& operator = (const CComInitS&);
};

/// <SUMMARY>
/// ��������������
/// </SUMMARY>
class CSensorObject
{
public:
    /// <SUMMARY>
    /// ���캯��
    /// </SUMMARY>
    CSensorObject()
    {
        m_sensorCount = 0;

        m_pSensorManager = NULL;
        m_pSensorCollection = NULL;
        m_pSensorArray = NULL;
    }

    /// <SUMMARY>
    /// ��������
    /// </SUMMARY>
    virtual ~CSensorObject()
    {
        SafeRelease();
    }

    /// <SUMMARY>
    /// ��ʼ��Sensor����
    /// </SUMMARY>
    /// <PARAM name = "sensorType" dir = "IN">
    /// ָ��Sensor��GUID
    /// </PARAM>
    /// <RETURNS>
    /// �ɹ�����true, ʧ�ܷ���false, m_errorMessage�д洢������Ϣ
    /// </RETURNS>
    bool TypeInit(IN REFSENSOR_TYPE_ID sensorType)
    {
        SafeRelease();

        HRESULT hr = CoCreateInstance(CLSID_SensorManager, NULL, CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&m_pSensorManager));
        if (hr == HRESULT_FROM_WIN32(ERROR_ACCESS_DISABLED_BY_POLICY))
        {
            SafeRelease();
            m_errorMessage = "CoCreateInstance: Unable to retrieve sensor manager due to group policy settings";
            return false;
        }

        // Get the sensor collection
        hr = m_pSensorManager->GetSensorsByType(sensorType, &m_pSensorCollection);
        if (hr == HRESULT_FROM_WIN32(ERROR_NOT_FOUND))
        {
            SafeRelease();
            m_errorMessage = "ISensorManager::GetSensorsByType: No sensors are available for the specified type";
            return false;
        }

        m_pSensorCollection->GetCount(&m_sensorCount);

        if (m_sensorCount == 0)
        {
            SafeRelease();
            m_errorMessage = "Sensors count is Zero, no sensors of the requested type";
            return false;
        }

        m_pSensorArray = new ISensor*[m_sensorCount];
        for (ULONG i = 0; i < m_sensorCount; i++)
            m_pSensorArray[i] = NULL;

        for (ULONG i = 0; i < m_sensorCount; i++)
        {
            m_pSensorCollection->GetAt(i, &m_pSensorArray[i]);
        }

        SensorState sensorState = SENSOR_STATE_NOT_AVAILABLE;
        for (ULONG i = 0; i < m_sensorCount; i++)
        {
            m_pSensorArray[i]->GetState(&sensorState);
            if (sensorState == SENSOR_STATE_ACCESS_DENIED)
            {
                hr = m_pSensorManager->RequestPermissions(0, m_pSensorCollection, TRUE);

                if (hr == HRESULT_FROM_WIN32(ERROR_ACCESS_DENIED)
                    || hr == HRESULT_FROM_WIN32(ERROR_CANCELLED))
                {
                    SafeRelease();
                    m_errorMessage = "ISensorManager::RequestPermissions: User have previously denied access to this sensor";
                    return false;
                }
            }
            sensorState = SENSOR_STATE_NOT_AVAILABLE;
        }

        return true;
    }

    /// <SUMMARY>
    /// ��ȡ����������
    /// </SUMMARY>
    /// <RETURNS>
    /// ����������
    /// </RETURNS>
    unsigned int GetSensorCount()
    {
        return m_sensorCount;
    }

    /// <SUMMARY>
    /// ��ȡ����������
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// �������豸����
    /// </PARAM>
    /// <PARAM name = "pDataReportAddr" dir = "OUT">
    /// �洢���������ݱ���
    /// </PARAM>
    /// <RETURNS>
    /// true(��ȡ�ɹ�), false(��ȡʧ��)
    /// </RETURNS>
    bool GetDataReport(IN unsigned int index, OUT ISensorDataReport*& pDataReport)
    {
        if (index >= m_sensorCount || index < 0)
        {
            m_errorMessage = "Index is out of range";
            return false;
        }

        ISensor* pSensor = m_pSensorArray[index];


        HRESULT hr = pSensor->GetData(&pDataReport);
        if (hr == E_INVALIDARG)
        {
            m_errorMessage = "ISensor::GetData: The sensor driver provided badly formed data";
            return false;
        }
        if (hr == HRESULT_FROM_WIN32(ERROR_NO_DATA))
        {
            m_errorMessage = "ISensor::GetData: The sensor has no data to report";
            return false;
        }

        return true;
    }

private:
    /// <SUMMARY>
    /// �ͷ���Դ
    /// </SUMMARY>
    void SafeRelease()
    {
        ISensor* pSensor = NULL;
        for (unsigned long i = 0; i < m_sensorCount; i++)
        {
            pSensor = m_pSensorArray[i];
            SAFE_RELEASE(pSensor);
            m_pSensorArray[i] = NULL;
        }
        SAFE_DEL_ARRAY(m_pSensorArray);

        m_sensorCount = 0;

        SAFE_RELEASE(m_pSensorCollection);
        SAFE_RELEASE(m_pSensorManager);
    }

private:
    unsigned long m_sensorCount; // Sensor�豸����
    string m_errorMessage; // ������Ϣ
    ISensor** m_pSensorArray; // ����Sensor�豸����ָ��
    CComInitS comInittS; // ���ڳ�ʼ��COM
    ISensorManager* m_pSensorManager;
    ISensorCollection* m_pSensorCollection;

private:
    CSensorObject(const CSensorObject&);
    CSensorObject& operator = (const CSensorObject&);
};

/// <SUMMARY>
/// �������ٶȴ�����
/// </SUMMARY>
class LAccelerometer3DSensor
{
public:
    /// <SUMMARY>
    /// ���캯��
    /// </SUMMARY>
    LAccelerometer3DSensor()
    {
        m_sensorObject.TypeInit(SENSOR_TYPE_ACCELEROMETER_3D);
    }

    /// <SUMMARY>
    /// ��������
    /// </SUMMARY>
    ~LAccelerometer3DSensor()
    {

    }

    /// <SUMMARY>
    /// ��ȡ����������
    /// </SUMMARY>
    /// <RETURNS>
    /// ����������
    /// </RETURNS>
    unsigned int GetCount()
    {
        return m_sensorObject.GetSensorCount();
    }

    /// <SUMMARY>
    /// ��ȡ�������ٶȴ���������
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// �������豸����
    /// </PARAM>
    /// <PARAM name = "data" dir = "OUT">
    /// �洢����������
    /// </PARAM>
    /// <RETURNS>
    /// true(��ȡ�ɹ�), false(��ȡʧ��)
    /// </RETURNS>
    bool GetData(IN unsigned int index, OUT SAccelerometer3DSensorData& data)
    {
        ISensorDataReport* pSensorDataReport = NULL;
        bool bRet = m_sensorObject.GetDataReport(index, pSensorDataReport);
        if (!bRet)
            return false;

        if (pSensorDataReport == NULL)
            return false;

        PROPVARIANT varX = {};
        PROPVARIANT varY = {};
        PROPVARIANT varZ = {};

        pSensorDataReport->GetSensorValue(SENSOR_DATA_TYPE_ACCELERATION_X_G, &varX);
        pSensorDataReport->GetSensorValue(SENSOR_DATA_TYPE_ACCELERATION_Y_G, &varY);
        pSensorDataReport->GetSensorValue(SENSOR_DATA_TYPE_ACCELERATION_Z_G, &varZ);

        data.X_G = varX.dblVal;
        data.Y_G = varY.dblVal;
        data.Z_G = varZ.dblVal;

        PropVariantClear(&varX);
        PropVariantClear(&varY);
        PropVariantClear(&varZ);

        SAFE_RELEASE(pSensorDataReport);

        return true;

    }

private:
    CSensorObject m_sensorObject; // ��������������

};

/// <SUMMARY>
/// �����Ǵ�����
/// </SUMMARY>
class LGyrometer3DSensor
{
public:
    /// <SUMMARY>
    /// ���캯��
    /// </SUMMARY>
    LGyrometer3DSensor()
    {
        m_sensorObject.TypeInit(SENSOR_TYPE_GYROMETER_3D);
    }

    /// <SUMMARY>
    /// ��������
    /// </SUMMARY>
    ~LGyrometer3DSensor()
    {

    }

    /// <SUMMARY>
    /// ��ȡ����������
    /// </SUMMARY>
    /// <RETURNS>
    /// ����������
    /// </RETURNS>
    unsigned int GetCount()
    {
        return m_sensorObject.GetSensorCount();
    }

    /// <SUMMARY>
    /// ��ȡ�����Ǵ���������
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// �������豸����
    /// </PARAM>
    /// <PARAM name = "data" dir = "OUT">
    /// �洢����������
    /// </PARAM>
    /// <RETURNS>
    /// true(��ȡ�ɹ�), false(��ȡʧ��)
    /// </RETURNS>
    bool GetData(IN unsigned int index, OUT SGyrometer3DSensorData& data)
    {
        ISensorDataReport* pSensorDataReport = NULL;
        bool bRet = m_sensorObject.GetDataReport(index, pSensorDataReport);
        if (!bRet)
            return false;

        if (pSensorDataReport == NULL)
            return false;

        PROPVARIANT varX = {};
        PROPVARIANT varY = {};
        PROPVARIANT varZ = {};

        pSensorDataReport->GetSensorValue(SENSOR_DATA_TYPE_ANGULAR_VELOCITY_X_DEGREES_PER_SECOND, &varX);
        pSensorDataReport->GetSensorValue(SENSOR_DATA_TYPE_ANGULAR_VELOCITY_Y_DEGREES_PER_SECOND, &varY);
        pSensorDataReport->GetSensorValue(SENSOR_DATA_TYPE_ANGULAR_VELOCITY_Z_DEGREES_PER_SECOND, &varZ);

        data.X_DEGREES_PER_SECOND = varX.dblVal;
        data.Y_DEGREES_PER_SECOND = varY.dblVal;
        data.Z_DEGREES_PER_SECOND = varZ.dblVal;

        PropVariantClear(&varX);
        PropVariantClear(&varY);
        PropVariantClear(&varZ);

        SAFE_RELEASE(pSensorDataReport);

        return true;
    }

private:
    CSensorObject m_sensorObject; // ��������������
};


/// <SUMMARY>
/// ָ���봫����
/// </SUMMARY>
class LCompass3DSensor
{
public:
    /// <SUMMARY>
    /// ���캯��
    /// </SUMMARY>
    LCompass3DSensor()
    {
        m_sensorObject.TypeInit(SENSOR_TYPE_COMPASS_3D);
    }

    /// <SUMMARY>
    /// ��������
    /// </SUMMARY>
    ~LCompass3DSensor()
    {

    }

    /// <SUMMARY>
    /// ��ȡ����������
    /// </SUMMARY>
    /// <RETURNS>
    /// ����������
    /// </RETURNS>
    unsigned int GetCount()
    {
        return m_sensorObject.GetSensorCount();
    }

    /// <SUMMARY>
    /// ��ȡָ���봫��������
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// �������豸����
    /// </PARAM>
    /// <PARAM name = "data" dir = "OUT">
    /// �洢����������
    /// </PARAM>
    /// <RETURNS>
    /// true(��ȡ�ɹ�), false(��ȡʧ��)
    /// </RETURNS>
    bool GetData(IN unsigned int index, OUT SCompass3DSensorData& data)
    {
        ISensorDataReport* pSensorDataReport = NULL;
        bool bRet = m_sensorObject.GetDataReport(index, pSensorDataReport);
        if (!bRet)
            return false;

        if (pSensorDataReport == NULL)
            return false;

        PROPVARIANT varX = {};
        PROPVARIANT varY = {};
        PROPVARIANT varZ = {};

        pSensorDataReport->GetSensorValue(SENSOR_DATA_TYPE_MAGNETIC_FIELD_STRENGTH_X_MILLIGAUSS, &varX);
        pSensorDataReport->GetSensorValue(SENSOR_DATA_TYPE_MAGNETIC_FIELD_STRENGTH_Y_MILLIGAUSS, &varY);
        pSensorDataReport->GetSensorValue(SENSOR_DATA_TYPE_MAGNETIC_FIELD_STRENGTH_Z_MILLIGAUSS, &varZ);

        data.X_MILLIGAUSS = varX.dblVal;
        data.Y_MILLIGAUSS = varY.dblVal;
        data.Z_MILLIGAUSS = varZ.dblVal;

        PropVariantClear(&varX);
        PropVariantClear(&varY);
        PropVariantClear(&varZ);

        SAFE_RELEASE(pSensorDataReport);

        return true;
    }

private:
    CSensorObject m_sensorObject; // ��������������
};

/// <SUMMARY>
/// GPS������
/// </SUMMARY>
class LLocationGpsSensor
{
public:
    /// <SUMMARY>
    /// ���캯��
    /// </SUMMARY>
    LLocationGpsSensor()
    {
        m_sensorObject.TypeInit(SENSOR_TYPE_LOCATION_GPS);
    }

    /// <SUMMARY>
    /// ��������
    /// </SUMMARY>
    ~LLocationGpsSensor()
    {

    }

    /// <SUMMARY>
    /// ��ȡ����������
    /// </SUMMARY>
    /// <RETURNS>
    /// ����������
    /// </RETURNS>
    unsigned int GetCount()
    {
        return m_sensorObject.GetSensorCount();
    }

    /// <SUMMARY>
    /// ��ȡGPS����������
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// �������豸����
    /// </PARAM>
    /// <PARAM name = "data" dir = "OUT">
    /// �洢����������
    /// </PARAM>
    /// <RETURNS>
    /// true(��ȡ�ɹ�), false(��ȡʧ��)
    /// </RETURNS>
    bool GetData(IN unsigned int index, OUT SGpsSensorData& data)
    {
        ISensorDataReport* pSensorDataReport = NULL;
        bool bRet = m_sensorObject.GetDataReport(index, pSensorDataReport);
        if (!bRet)
            return false;

        if (pSensorDataReport == NULL)
            return false;

        PROPVARIANT varLatitude = {};
        PROPVARIANT varLongitude = {};

        pSensorDataReport->GetSensorValue(SENSOR_DATA_TYPE_LATITUDE_DEGREES, &varLatitude);
        pSensorDataReport->GetSensorValue(SENSOR_DATA_TYPE_LONGITUDE_DEGREES, &varLongitude);

        data.LAYITUDE_DEGREES = varLatitude.dblVal;
        data.LONGITUDE_GERREES = varLongitude.dblVal;

        PropVariantClear(&varLatitude);
        PropVariantClear(&varLongitude);

        SAFE_RELEASE(pSensorDataReport);

        return true;
    }

private:
    CSensorObject m_sensorObject; // ��������������
};


/// <SUMMARY>
/// �⴫����
/// </SUMMARY>
class LAmbientLightSensor
{
public:
    /// <SUMMARY>
    /// ���캯��
    /// </SUMMARY>
    LAmbientLightSensor()
    {
        m_sensorObject.TypeInit(SENSOR_TYPE_AMBIENT_LIGHT);
    }

    /// <SUMMARY>
    /// ��������
    /// </SUMMARY>
    ~LAmbientLightSensor()
    {

    }

    /// <SUMMARY>
    /// ��ȡ����������
    /// </SUMMARY>
    /// <RETURNS>
    /// ����������
    /// </RETURNS>
    unsigned int GetCount()
    {
        return m_sensorObject.GetSensorCount();
    }

    /// <SUMMARY>
    /// ��ȡ�⴫��������
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// �������豸����
    /// </PARAM>
    /// <PARAM name = "data" dir = "OUT">
    /// �洢����������
    /// </PARAM>
    /// <RETURNS>
    /// true(��ȡ�ɹ�), false(��ȡʧ��)
    /// </RETURNS>
    bool GetData(IN unsigned int index, OUT SAmbientLightSensorData& data)
    {
        ISensorDataReport* pSensorDataReport = NULL;
        bool bRet = m_sensorObject.GetDataReport(index, pSensorDataReport);
        if (!bRet)
            return false;

        if (pSensorDataReport == NULL)
            return false;

        PROPVARIANT varLighLevel = {};
        pSensorDataReport->GetSensorValue(SENSOR_DATA_TYPE_LIGHT_LEVEL_LUX, &varLighLevel);

        data.LIGHT_LEVEL = varLighLevel.fltVal;

        PropVariantClear(&varLighLevel);
        SAFE_RELEASE(pSensorDataReport);

        return true;
    }

private:
    CSensorObject m_sensorObject; // ��������������

};

LAccelerometer3DSensor* gAccelerometer3D = NULL;
LGyrometer3DSensor* gGyrometer3D = NULL;
LCompass3DSensor* gCompass3D = NULL;
LAmbientLightSensor* gAmbientLight = NULL;
LLocationGpsSensor* gLocationGps = NULL;

/// @brief ����ж��DLL
static void DllProcessDetach()
{
    if (gAccelerometer3D != NULL)
    {
        delete gAccelerometer3D;
        gAccelerometer3D = NULL;
    }

    if (gGyrometer3D != NULL)
    {
        delete gGyrometer3D;
        gGyrometer3D = NULL;
    }

    if (gCompass3D != NULL)
    {
        delete gCompass3D;
        gCompass3D = NULL;
    }

    if (gLocationGps != NULL)
    {
        delete gLocationGps;
        gLocationGps = NULL;
    }

    if (gAmbientLight != NULL)
    {
        delete gAmbientLight;
        gAmbientLight = NULL;
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD canReason, LPVOID lpReserved)
{
    switch (canReason)
    {
    case DLL_PROCESS_ATTACH:
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        DllProcessDetach();
        break;
    }
    return TRUE;
}

bool GetAccelerometer3DSensorInfor(OUT SAccelerometer3DInforArray* pInforArray)
{
    if (pInforArray == NULL)
        return false;

    if (gAccelerometer3D == NULL)
    {
        gAccelerometer3D = new LAccelerometer3DSensor();
    }

    pInforArray->Count = gAccelerometer3D->GetCount();
    for (unsigned int i = 0; i < pInforArray->Count && i < MAX_SENSOR_NUMBER; i++)
    {
        gAccelerometer3D->GetData(i, pInforArray->Data[i]);
    }

    return true;
}

bool GetGyrometer3DSensorInfor(OUT SGyrometer3DInforArray* pInforArray)
{
    if (pInforArray == NULL)
        return false;

    if (gGyrometer3D == NULL)
    {
        gGyrometer3D = new LGyrometer3DSensor();
    }

    pInforArray->Count = gGyrometer3D->GetCount();
    for (unsigned int i = 0; i < pInforArray->Count && i < MAX_SENSOR_NUMBER; i++)
    {
        gGyrometer3D->GetData(i, pInforArray->Data[i]);
    }

    return true;
}

bool GetCompass3DSensorInfor(OUT SCompass3DInforArray* pInforArray)
{
    if (pInforArray == NULL)
        return false;

    if (gCompass3D == NULL)
    {
        gCompass3D = new LCompass3DSensor();
    }

    pInforArray->Count = gCompass3D->GetCount();
    for (unsigned int i = 0; i < pInforArray->Count && i < MAX_SENSOR_NUMBER; i++)
    {
        gCompass3D->GetData(i, pInforArray->Data[i]);
    }

    return true;
}

bool GetGpsSensorInfor(OUT SGpsInforArray* pInforArray)
{
    if (pInforArray == NULL)
        return false;

    if (gLocationGps == NULL)
    {
        gLocationGps = new LLocationGpsSensor();
    }

    pInforArray->Count = gLocationGps->GetCount();
    for (unsigned int i = 0; i < pInforArray->Count && i < MAX_SENSOR_NUMBER; i++)
    {
        gLocationGps->GetData(i, pInforArray->Data[i]);
    }

    return true;
}

bool GetAmbientLightSensorInfor(OUT SAmbientLightInforArray* pInforArray)
{
    if (pInforArray == NULL)
        return false;

    if (gAmbientLight == NULL)
    {
        gAmbientLight = new LAmbientLightSensor();
    }

    pInforArray->Count = gAmbientLight->GetCount();
    for (unsigned int i = 0; i < pInforArray->Count && i < MAX_SENSOR_NUMBER; i++)
    {
        gAmbientLight->GetData(i, pInforArray->Data[i]);
    }

    return true;
}



