
#include "Gpu.h"
#include <Windows.h>

#include ".\\nvapi\\nvapi.h"
#include ".\\adlapi\\adl_sdk.h"

#pragma comment(lib, "nvapi.lib")

/*
 * ���ļ���������CGpu, CNvGpu, CAMDGpu3����, 
 * CNvGpu���ʵ����ͨ��ʹ��Ӣΰ�￪����nvapi��ʵ�ֵ�
 * CAMDGpu���ʵ����ͨ��ʹ��AMD������ADL(AMD Display Library)��ʵ�ֵ�
 */

/// <SUMMARY>
/// Gpu����
/// </SUMMARY>
class CGpu
{
public:
    CGpu(){}
    virtual ~CGpu(){}

    /// <SUMMARY>
    /// ��ȡ�¶�
    /// </SUMMARY>
    /// <RETURNS>
    /// �ɹ�����true, ʧ�ܷ���false
    /// </RETURNS>
    virtual bool GetTemperature(OUT unsigned int& sensorsNum, OUT unsigned int temp[MAX_GPU_SENSORS_NUMBER]){return false;}
};

/// <SUMMARY>
/// NVIDIA(Ӣΰ��)Gpu��
/// </SUMMARY>
class LNvGpu : public CGpu
{
public:
    LNvGpu();
    virtual ~LNvGpu();

    virtual bool GetTemperature(OUT unsigned int& sensorsNum, OUT unsigned int temp[MAX_GPU_SENSORS_NUMBER]);

private:
    int m_usingGpuIndex; // ����ʹ�õ�Gpu�������
    const int INVALID_GPU_INDEX; // ��ЧGpu�������
    NvPhysicalGpuHandle m_hNvPhysicalGpuList[NVAPI_MAX_PHYSICAL_GPUS]; // ����Gpu�����
};

LNvGpu::LNvGpu()
    : INVALID_GPU_INDEX(-1)
{
    m_usingGpuIndex = INVALID_GPU_INDEX;

    NvAPI_Status nvRet = NvAPI_Initialize();
    if (nvRet != NVAPI_OK)
        return;

    NvU32 gpuCount = 0;
    nvRet = NvAPI_EnumPhysicalGPUs(m_hNvPhysicalGpuList, &gpuCount);
    if (nvRet != NVAPI_OK || gpuCount == 0)
        return;


    m_usingGpuIndex = 0;

}

LNvGpu::~LNvGpu()
{

}

bool LNvGpu::GetTemperature(OUT unsigned int& sensorsNum, OUT unsigned int temp[MAX_GPU_SENSORS_NUMBER])
{
    if (m_usingGpuIndex == INVALID_GPU_INDEX)
        return false;

    /* ��ȡ�¶�ǰ�Ȼ�ȡ������Ϣ, ��Ȼ��ȡ�¶Ȼ�ʧ��*/
    NvU32 busId = 0;
    NvAPI_GPU_GetBusId(m_hNvPhysicalGpuList[m_usingGpuIndex], &busId);

    NV_GPU_THERMAL_SETTINGS thermal;
    thermal.version = NV_GPU_THERMAL_SETTINGS_VER;  
    NvAPI_Status nvRet = NvAPI_GPU_GetThermalSettings(m_hNvPhysicalGpuList[m_usingGpuIndex], NVAPI_THERMAL_TARGET_ALL, &thermal);
    if (nvRet != NVAPI_OK)
        return false;

    sensorsNum = thermal.count;
    for (unsigned int i = 0; i < thermal.count; i++)
    {
        temp[i] = thermal.sensor[i].currentTemp;
    }

    return true;
}


/// <SUMMARY>
/// ��ʼ��ADL2�ӿ�
/// </SUMMARY>
/// <PARAM name = "callback" dir = "IN">
/// �ڴ����ص�����
/// </PARAM>
/// <PARAM name = "iEnumConnectedAdapters" dir = "IN">
/// ��Ϊ1�õ�������ڲ��ұ����õ��Կ�, ��Ϊ0�õ������������ڵ��Կ�
/// </PARAM>
/// <PARAM name = "context" dir = "OUT">
/// �洢ADL2�������
/// </PARAM>
/// <RETURNS>
/// �ɹ�����ADL_OK, ʧ�ܷ��ش�����
/// </RETURNS>
typedef int (*ADL2_MAIN_CONTROL_CREATE )(IN ADL_MAIN_MALLOC_CALLBACK callback, IN int iEnumConnectedAdapters, OUT ADL_CONTEXT_HANDLE* context);

/// <SUMMARY>
/// ж��ADL2
/// </SUMMARY>
/// <PARAM name = "context" dir = "IN">
/// ADL2�������
/// </PARAM>
/// <RETURNS>
/// �ɹ�����ADL_OK, ʧ�ܷ��ش�����
/// </RETURNS>
typedef int (*ADL2_MAIN_CONTROL_DESTROY )(IN ADL_CONTEXT_HANDLE context);

/// <SUMMARY>
/// ��ȡ����������
/// </SUMMARY>
/// <PARAM name = "context" dir = "IN">
/// ADL2�������
/// </PARAM>
/// <PARAM name = "lpNumAdapters" dir = "OUT">
/// �洢����������
/// </PARAM>
/// <RETURNS>
/// �ɹ�����ADL_OK, ʧ�ܷ��ش�����
/// </RETURNS>
typedef int (*ADL2_ADAPTER_NUMBEROFADAPTERS_GET ) (IN ADL_CONTEXT_HANDLE context, OUT int* lpNumAdapters);

/// <SUMMARY>
/// ��ȡ��������Ϣ
/// </SUMMARY>
/// <PARAM name = "context" dir = "IN">
/// ADL2�������
/// </PARAM>
/// <PARAM name = "lpInfo" dir = "OUT">
/// �洢��������Ϣ
/// </PARAM>
/// <PARAM name = "iInputSize" dir = "IN">
/// ���뻺�����Ĵ�С
/// </PARAM>
/// <RETURNS>
/// �ɹ�����ADL_OK, ʧ�ܷ��ش�����
/// </RETURNS>
typedef int (*ADL2_ADAPTER_ADAPTERINFO_GET)(IN ADL_CONTEXT_HANDLE context, OUT LPAdapterInfo lpInfo, IN int iInputSize);

/// <SUMMARY>
/// ��ȡ�Կ��¶�
/// </SUMMARY>
/// <PARAM name = "context" dir = "IN">
/// ADL2�������
/// </PARAM>
/// <PARAM name = "iAdapterIndex" dir = "IN">
/// ����������
/// </PARAM>
/// <PARAM name = "iThermalControllerIndex" dir = "IN">
/// �ȴ���������
/// </PARAM>
/// <PARAM name = "lpTemperature" dir = "OUT">
/// �洢�¶�
/// </PARAM>
/// <RETURNS>
/// �ɹ�����ADL_OK, ʧ�ܷ��ش�����
/// </RETURNS>
typedef int (*ADL2_OVERDRIVE5_TEMPERATURE_GET)(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int iThermalControllerIndex, ADLTemperature* lpTemperature);

void* __stdcall ADL_Main_Memory_Alloc ( int iSize )

{
    void* lpBuffer = malloc ( iSize );
    return lpBuffer;
}

void __stdcall ADL_Main_Memory_Free ( void** lpBuffer )
{
    if ( NULL != *lpBuffer )
    {
        free ( *lpBuffer );
        *lpBuffer = NULL;
    }
}

/// <SUMMARY>
/// AMD Gpu��
/// </SUMMARY>
class LAMDGpu : public CGpu
{
public:
    LAMDGpu();
    ~LAMDGpu();

    virtual bool GetTemperature(OUT unsigned int& sensorsNum, OUT unsigned int temp[MAX_GPU_SENSORS_NUMBER]);

private:
    ADL2_MAIN_CONTROL_CREATE ADL2_Main_Control_Create; // ADL��ʼ������ָ��
    ADL2_MAIN_CONTROL_DESTROY ADL2_Main_Control_Destroy; // ADLж�غ���ָ��
    ADL2_ADAPTER_NUMBEROFADAPTERS_GET ADL2_Adapter_NumberOfAdapters_Get; // ADL��ȡ��������������ָ��
    ADL2_ADAPTER_ADAPTERINFO_GET ADL2_Adapter_AdapterInfo_Get; // ADL��ȡ��������Ϣ����ָ��
    ADL2_OVERDRIVE5_TEMPERATURE_GET ADL2_Overdrive5_Temperature_Get; // ADL��ȡ�Կ��¶Ⱥ���ָ��

    ADL_CONTEXT_HANDLE m_hADLContext; // ADL�������
    const int INVALID_ADAPTER_INDEX; // ��Ч�Կ�����������
    int  m_usingAdapterIndex; // ����ʹ�õ��Կ�����������
    HMODULE m_hADLDll; // ADL��̬����

};


LAMDGpu::LAMDGpu()
	: INVALID_ADAPTER_INDEX(-1)
{
	m_hADLContext = NULL;
	ADL2_Main_Control_Create = NULL;
	ADL2_Main_Control_Destroy = NULL;
	ADL2_Adapter_AdapterInfo_Get = NULL;
	ADL2_Adapter_AdapterInfo_Get = NULL;
	m_hADLDll = NULL;
	m_usingAdapterIndex = INVALID_ADAPTER_INDEX;

	// ��װ��AMD�Կ�������ϵͳĿ¼System32�´���atiadlxx.dll(32λ��64λ)
	// ��64λϵͳ��, 32λ�ĳ���װ��64λ��dll��ʧ��, ������Ҫ��װ��SysWOW64Ŀ¼�µ�atiadlxy.dll
	m_hADLDll = LoadLibraryA("atiadlxx.dll");
	if (m_hADLDll == NULL)
		m_hADLDll = LoadLibraryA("atiadlxy.dll");
	if (m_hADLDll == NULL)
		return;

	ADL2_Main_Control_Create = (ADL2_MAIN_CONTROL_CREATE)GetProcAddress(m_hADLDll, "ADL2_Main_Control_Create");
	ADL2_Main_Control_Destroy = (ADL2_MAIN_CONTROL_DESTROY)GetProcAddress(m_hADLDll, "ADL2_Main_Control_Destroy");
	ADL2_Adapter_NumberOfAdapters_Get = (ADL2_ADAPTER_NUMBEROFADAPTERS_GET)GetProcAddress(m_hADLDll, "ADL2_Adapter_NumberOfAdapters_Get");
	ADL2_Adapter_AdapterInfo_Get = (ADL2_ADAPTER_ADAPTERINFO_GET)GetProcAddress(m_hADLDll, "ADL2_Adapter_AdapterInfo_Get");
	ADL2_Overdrive5_Temperature_Get = (ADL2_OVERDRIVE5_TEMPERATURE_GET)(m_hADLDll, "ADL2_Overdrive5_Temperature_Get ");

	if (ADL2_Main_Control_Create == NULL ||
		ADL2_Main_Control_Destroy == NULL ||
		ADL2_Adapter_NumberOfAdapters_Get == NULL ||
		ADL2_Adapter_AdapterInfo_Get == NULL ||
		ADL2_Overdrive5_Temperature_Get == NULL)
		return;

	int iRet;

	iRet = ADL2_Main_Control_Create(ADL_Main_Memory_Alloc, 1, &m_hADLContext);
	if (iRet != ADL_OK)
		return;

	int adapterCount = 0;
	iRet = ADL2_Adapter_NumberOfAdapters_Get(m_hADLContext, &adapterCount);
	if (iRet != ADL_OK)
		return;

	if (adapterCount < 1)
		return;

	
	m_usingAdapterIndex = 0;

}

LAMDGpu::~LAMDGpu()
{

	if (ADL2_Main_Control_Destroy != NULL)
	{
		ADL2_Main_Control_Destroy(m_hADLContext);
	}

	if (m_hADLDll != NULL)
	{
		FreeLibrary(m_hADLDll);
		m_hADLDll = NULL;
	}
}

bool LAMDGpu::GetTemperature(OUT unsigned int& sensorsNum, OUT unsigned int temp[MAX_GPU_SENSORS_NUMBER])
{
	if (m_usingAdapterIndex == INVALID_ADAPTER_INDEX)
		return false;

	ADLTemperature adlTemp;
	if (ADL2_Overdrive5_Temperature_Get(m_hADLContext, m_usingAdapterIndex, 0, &adlTemp) != ADL_OK) 
	{
		return false;
	}

    sensorsNum = 1;
    temp[0] = (unsigned int)adlTemp.iTemperature/1000;

	return true;
}

LGpu::LGpu()
{
    m_pCGpu = new LNvGpu();
    unsigned int sensorsNum = 0;
    unsigned int temp[MAX_GPU_SENSORS_NUMBER] = {0};
    if (m_pCGpu->GetTemperature(sensorsNum, temp))
    {
        return;
    }

    if (m_pCGpu != 0)
    {
        delete m_pCGpu;
        m_pCGpu = 0;
    }

    m_pCGpu = new LAMDGpu();
    if (m_pCGpu->GetTemperature(sensorsNum, temp))
    {
        return;
    }

    if (m_pCGpu != 0)
    {
        delete m_pCGpu;
        m_pCGpu = 0;
    }

}

LGpu::~LGpu()
{
    if (m_pCGpu != 0)
    {
        delete m_pCGpu;
        m_pCGpu = 0;
    }
}

bool LGpu::GetTemperature(OUT unsigned int& sensorsNum, OUT unsigned int temp[MAX_GPU_SENSORS_NUMBER])
{
    if (m_pCGpu == 0)
        return false;

    return m_pCGpu->GetTemperature(sensorsNum, temp);
}