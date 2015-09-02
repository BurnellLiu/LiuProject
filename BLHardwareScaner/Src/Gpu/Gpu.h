
/*
 * Copyright (c)ASUSTek Computer, Inc. All rights reserved.
 * Author: Jie_Liu Version:  Date: 2015/01/15
 * Description
 * ���ļ���������CGpu, CNvGpu, CAMDGpu3����, 
 * CNvGpu���ʵ����ͨ��ʹ��Ӣΰ�￪����nvapi��ʵ�ֵ�
 * CAMDGpu���ʵ����ͨ��ʹ��AMD������ADL(AMD Display Library)��ʵ�ֵ�
 * Others:
 * Function List:
 * 1.
 * History:
 * 1.
 */

#ifndef _GPU_H_
#define _GPU_H_

#include <Windows.h>

#include ".\\nvapi\\nvapi.h"
#include ".\\adlapi\\adl_sdk.h"

/// <SUMMARY>
/// Gpu����
/// </SUMMARY>
class LGpu
{
public:
	LGpu();
	virtual ~LGpu();

	/// <SUMMARY>
	/// ��ȡ�¶�
	/// </SUMMARY>
	/// <RETURNS>
	/// �¶�, ��λ���϶�, ��ȡʧ�ܷ���0
	/// </RETURNS>
	virtual unsigned int GetTemperature();
};

/// <SUMMARY>
/// NVIDIA(Ӣΰ��)Gpu��
/// </SUMMARY>
class LNvGpu : public LGpu
{
public:
	LNvGpu();
	virtual ~LNvGpu();

	virtual unsigned int GetTemperature();

private:
	int m_usingGpuIndex; // ����ʹ�õ�Gpu�������
	const int INVALID_GPU_INDEX; // ��ЧGpu�������
	NvPhysicalGpuHandle m_hNvPhysicalGpuList[NVAPI_MAX_PHYSICAL_GPUS]; // ����Gpu�����
};


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

/// <SUMMARY>
/// AMD Gpu��
/// </SUMMARY>
class LAMDGpu : public LGpu
{
public:
	LAMDGpu();
	~LAMDGpu();

	virtual unsigned int GetTemperature();

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

#endif