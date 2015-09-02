
/*
 * Copyright (c)ASUSTek Computer, Inc. All rights reserved.
 * Author: Jie_Liu Version:  Date: 2015/01/15
 * Description
 * 该文件中声明了CGpu, CNvGpu, CAMDGpu3个类, 
 * CNvGpu类的实现是通过使用英伟达开发包nvapi来实现的
 * CAMDGpu类的实现是通过使用AMD开发包ADL(AMD Display Library)来实现的
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
/// Gpu基类
/// </SUMMARY>
class LGpu
{
public:
	LGpu();
	virtual ~LGpu();

	/// <SUMMARY>
	/// 获取温度
	/// </SUMMARY>
	/// <RETURNS>
	/// 温度, 单位摄氏度, 获取失败返回0
	/// </RETURNS>
	virtual unsigned int GetTemperature();
};

/// <SUMMARY>
/// NVIDIA(英伟达)Gpu类
/// </SUMMARY>
class LNvGpu : public LGpu
{
public:
	LNvGpu();
	virtual ~LNvGpu();

	virtual unsigned int GetTemperature();

private:
	int m_usingGpuIndex; // 正在使用的Gpu句柄索引
	const int INVALID_GPU_INDEX; // 无效Gpu句柄索引
	NvPhysicalGpuHandle m_hNvPhysicalGpuList[NVAPI_MAX_PHYSICAL_GPUS]; // 物理Gpu句柄表
};


/// <SUMMARY>
/// 初始化ADL2接口
/// </SUMMARY>
/// <PARAM name = "callback" dir = "IN">
/// 内存分配回调函数
/// </PARAM>
/// <PARAM name = "iEnumConnectedAdapters" dir = "IN">
/// 设为1得到物理存在并且被启用的显卡, 设为0得到所有曾经存在的显卡
/// </PARAM>
/// <PARAM name = "context" dir = "OUT">
/// 存储ADL2环境句柄
/// </PARAM>
/// <RETURNS>
/// 成功返回ADL_OK, 失败返回错误码
/// </RETURNS>
typedef int (*ADL2_MAIN_CONTROL_CREATE )(IN ADL_MAIN_MALLOC_CALLBACK callback, IN int iEnumConnectedAdapters, OUT ADL_CONTEXT_HANDLE* context);

/// <SUMMARY>
/// 卸载ADL2
/// </SUMMARY>
/// <PARAM name = "context" dir = "IN">
/// ADL2环境句柄
/// </PARAM>
/// <RETURNS>
/// 成功返回ADL_OK, 失败返回错误码
/// </RETURNS>
typedef int (*ADL2_MAIN_CONTROL_DESTROY )(IN ADL_CONTEXT_HANDLE context);

/// <SUMMARY>
/// 获取适配器数量
/// </SUMMARY>
/// <PARAM name = "context" dir = "IN">
/// ADL2环境句柄
/// </PARAM>
/// <PARAM name = "lpNumAdapters" dir = "OUT">
/// 存储适配器数量
/// </PARAM>
/// <RETURNS>
/// 成功返回ADL_OK, 失败返回错误码
/// </RETURNS>
typedef int (*ADL2_ADAPTER_NUMBEROFADAPTERS_GET ) (IN ADL_CONTEXT_HANDLE context, OUT int* lpNumAdapters);

/// <SUMMARY>
/// 获取适配器信息
/// </SUMMARY>
/// <PARAM name = "context" dir = "IN">
/// ADL2环境句柄
/// </PARAM>
/// <PARAM name = "lpInfo" dir = "OUT">
/// 存储适配器信息
/// </PARAM>
/// <PARAM name = "iInputSize" dir = "IN">
/// 输入缓冲区的大小
/// </PARAM>
/// <RETURNS>
/// 成功返回ADL_OK, 失败返回错误码
/// </RETURNS>
typedef int (*ADL2_ADAPTER_ADAPTERINFO_GET)(IN ADL_CONTEXT_HANDLE context, OUT LPAdapterInfo lpInfo, IN int iInputSize);

/// <SUMMARY>
/// 获取显卡温度
/// </SUMMARY>
/// <PARAM name = "context" dir = "IN">
/// ADL2环境句柄
/// </PARAM>
/// <PARAM name = "iAdapterIndex" dir = "IN">
/// 适配器索引
/// </PARAM>
/// <PARAM name = "iThermalControllerIndex" dir = "IN">
/// 热传感器索引
/// </PARAM>
/// <PARAM name = "lpTemperature" dir = "OUT">
/// 存储温度
/// </PARAM>
/// <RETURNS>
/// 成功返回ADL_OK, 失败返回错误码
/// </RETURNS>
typedef int (*ADL2_OVERDRIVE5_TEMPERATURE_GET)(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int iThermalControllerIndex, ADLTemperature* lpTemperature);

/// <SUMMARY>
/// AMD Gpu类
/// </SUMMARY>
class LAMDGpu : public LGpu
{
public:
	LAMDGpu();
	~LAMDGpu();

	virtual unsigned int GetTemperature();

private:
	ADL2_MAIN_CONTROL_CREATE ADL2_Main_Control_Create; // ADL初始化函数指针
	ADL2_MAIN_CONTROL_DESTROY ADL2_Main_Control_Destroy; // ADL卸载函数指针
	ADL2_ADAPTER_NUMBEROFADAPTERS_GET ADL2_Adapter_NumberOfAdapters_Get; // ADL获取适配器数量函数指针
	ADL2_ADAPTER_ADAPTERINFO_GET ADL2_Adapter_AdapterInfo_Get; // ADL获取适配器信息函数指针
	ADL2_OVERDRIVE5_TEMPERATURE_GET ADL2_Overdrive5_Temperature_Get; // ADL获取显卡温度函数指针

	ADL_CONTEXT_HANDLE m_hADLContext; // ADL环境句柄
	const int INVALID_ADAPTER_INDEX; // 无效显卡适配器索引
	int  m_usingAdapterIndex; // 正在使用的显卡适配器索引
	HMODULE m_hADLDll; // ADL动态库句柄

};

#endif