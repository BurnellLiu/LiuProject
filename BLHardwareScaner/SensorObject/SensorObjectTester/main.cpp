#include <cstdio>

#include <Windows.h>

#include "..\\SensorObject\\SensorObject.h"

typedef bool(*GetAccelerometer3DSensorInforFun)(OUT SAccelerometer3DInforArray* pInforArray);

typedef bool(*GetGyrometer3DSensorInforFun)(OUT SGyrometer3DInforArray* pInforArray);

typedef bool(*GetCompass3DSensorInforFun)(OUT SCompass3DInforArray* pInforArray);

typedef bool(*GetAmbientLightSensorInforFun)(OUT SAmbientLightInforArray* pInforArray);

typedef bool(*GetGpsSensorInforFun)(OUT SGpsInforArray* pInforArray);


int main()
{
    HMODULE hSensorObject = NULL; // SensorObject DLL���

    GetAccelerometer3DSensorInforFun pGetAccelerometer3DSensorInforFun = NULL; // ��ȡ�������ٶȴ�������Ϣ����ָ��
    GetGyrometer3DSensorInforFun pGetGyrometer3DSensorInforFun = NULL; // ��ȡ�����Ǵ�������Ϣ����ָ��
    GetCompass3DSensorInforFun pGetCompass3DSensorInforFun = NULL; // ��ȡָ���봫������Ϣ����ָ��
    GetAmbientLightSensorInforFun pGetAmbientLightSensorInforFun = NULL; // ��ȡ�⴫��������ָ��
    GetGpsSensorInforFun pGetGpsSensorInforFun = NULL; // ��ȡGPS����������ָ��

    hSensorObject = LoadLibraryW(L".\\SensorObject.dll");

    if (hSensorObject == NULL)
    {
        printf("Load SensorObject.dll Fail\n");
        return 0;
    }
        

    pGetAccelerometer3DSensorInforFun =
        (GetAccelerometer3DSensorInforFun)GetProcAddress(hSensorObject, "GetAccelerometer3DSensorInfor");

    pGetGyrometer3DSensorInforFun =
        (GetGyrometer3DSensorInforFun)GetProcAddress(hSensorObject, "GetGyrometer3DSensorInfor");

    pGetCompass3DSensorInforFun =
        (GetCompass3DSensorInforFun)GetProcAddress(hSensorObject, "GetCompass3DSensorInfor");

    pGetAmbientLightSensorInforFun =
        (GetAmbientLightSensorInforFun)GetProcAddress(hSensorObject, "GetAmbientLightSensorInfor");

    pGetGpsSensorInforFun =
        (GetGpsSensorInforFun)GetProcAddress(hSensorObject, "GetGpsSensorInfor");

    printf("Get Accelerometer Infor:\n");
    SAccelerometer3DInforArray accelerometerSensor;
    pGetAccelerometer3DSensorInforFun(&accelerometerSensor);
    printf("Accelerometer Count: %u\n", accelerometerSensor.Count);
    printf("\n");

    printf("Get Gyrometer Infor:\n");
    SGyrometer3DInforArray gyrometerSensor;
    pGetGyrometer3DSensorInforFun(&gyrometerSensor);
    printf("Gyrometer Count: %u\n", gyrometerSensor.Count);
    printf("\n");

    printf("Get Compass Infor:\n");
    SCompass3DInforArray compassSensor;
    pGetCompass3DSensorInforFun(&compassSensor);
    printf("Compass Count: %u\n", compassSensor.Count);
    printf("\n");

    printf("Get AmbientLight Infor:\n");
    SAmbientLightInforArray lightSensor;
    pGetAmbientLightSensorInforFun(&lightSensor);
    printf("AmbientLight Count: %u\n", lightSensor.Count);
    printf("\n");

    printf("Get Gps Infor:\n");
    SGpsInforArray gpsSensor;
    pGetGpsSensorInforFun(&gpsSensor);
    printf("Gps Count: %u\n", gpsSensor.Count);
    printf("\n");

    if (hSensorObject != NULL)
    {
        FreeLibrary(hSensorObject);
        hSensorObject = NULL;
    }

    system("pause");
    return 0;
}