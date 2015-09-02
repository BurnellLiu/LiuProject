
#include "Gpu.h"

#pragma comment(lib, "nvapi.lib")

LGpu::LGpu()
{

}

LGpu::~LGpu()
{

}

unsigned int LGpu::GetTemperature()
{
	return 0;
}

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

unsigned int LNvGpu::GetTemperature()
{
	if (m_usingGpuIndex == INVALID_GPU_INDEX)
		return 0;

    /* 获取温度前先获取其他信息, 不然获取温度会失败*/
    NvU32 busId = 0;
    NvAPI_GPU_GetBusId(m_hNvPhysicalGpuList[m_usingGpuIndex], &busId);

	NV_GPU_THERMAL_SETTINGS thermal;
	thermal.version = NV_GPU_THERMAL_SETTINGS_VER;  
	NvAPI_Status nvRet = NvAPI_GPU_GetThermalSettings(m_hNvPhysicalGpuList[m_usingGpuIndex], 0, &thermal);
	if (nvRet != NVAPI_OK)
		return 0;

	return thermal.sensor[0].currentTemp;
}

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

	// 安装了AMD显卡驱动的系统目录System32下存在atiadlxx.dll(32位或64位)
	// 在64位系统上, 32位的程序装载64位的dll会失败, 所以需要在装载SysWOW64目录下的atiadlxy.dll
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

unsigned int LAMDGpu::GetTemperature()
{
	if (m_usingAdapterIndex == INVALID_ADAPTER_INDEX)
		return 0;

	ADLTemperature temp;
	if (ADL2_Overdrive5_Temperature_Get(m_hADLContext, m_usingAdapterIndex, 0, &temp) != ADL_OK) 
	{
		return 0;
	}

	return temp.iTemperature/1000;
}