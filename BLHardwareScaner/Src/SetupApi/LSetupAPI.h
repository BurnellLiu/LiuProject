#ifndef _LSETUPAPI_H_
#define _LSETUPAPI_H_

#include <string>
using std::string;
using std::wstring;

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

class CSADevObject;

/// <SUMMARY>
/// 设备对象接口
/// </SUMMARY>
class LSetupDev
{
public:
    ~LSetupDev();

    /// <SUMMARY>
    /// 获取设备数目
    /// </SUMMARY>
    /// <RETURNS>
    /// 设备数目
    /// </RETURNS>
    int GetDevNum();

    /// <SUMMARY>
    /// 启用设备
    /// 该方法可能需要管理员权限
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// 设备索引
    /// </PARAM>
    /// <RETURNS>
    /// 成功返回0 失败返回ErrorCode, 返回系统的GetLastError()
    /// </RETURNS>
    unsigned long Enable(IN int index);

    /// <SUMMARY>
    /// 禁用设备
    /// 该方法需要管理员权限, 需要重启才可Disable的设备, 该方法也返回0
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// 设备索引
    /// </PARAM>
    /// <RETURNS>
    /// 成功返回0 失败返回ErrorCode, 返回系统的GetLastError()
    /// </RETURNS>
    unsigned long Disable(IN int index);

    /// <SUMMARY>
    /// 获取设备描述
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// 设备索引
    /// </PARAM>
    /// <PARAM name = "devDesc" dir = "OUT">
    /// 保存设备描述
    /// </PARAM>
    /// <RETURNS>
    /// 成功返回0 失败返回ErrorCode, 返回系统的GetLastError()
    /// </RETURNS>
    unsigned long GetDevDesc(IN int index, OUT string& devDesc);

    /// <SUMMARY>
    /// 获取设备硬件ID
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// 设备索引
    /// </PARAM>
    /// <PARAM name = "devHardwareId" dir = "OUT">
    /// 保存设备硬件ID
    /// </PARAM>
    /// <RETURNS>
    /// 成功返回0 失败返回ErrorCode, 返回系统的GetLastError()
    /// </RETURNS>
    unsigned long GetHardwareId(IN int index, OUT string& devHardwareId);

    /// <SUMMARY>
    /// 获取设备友好名称
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// 设备索引
    /// </PARAM>
    /// <PARAM name = "devFrindName" dir = "OUT">
    /// 保存设备友好名称
    /// </PARAM>
    /// <RETURNS>
    /// 成功返回0 失败返回ErrorCode, 返回系统的GetLastError()
    /// </RETURNS>
    unsigned long GetFriendlyName(IN int index, OUT string& devFrindName);

    /// <SUMMARY>
    /// 获取设备位置信息
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// 设备索引
    /// </PARAM>
    /// <PARAM name = "devLocationInfo" dir = "OUT">
    /// 保存设备位置信息
    /// </PARAM>
    /// <RETURNS>
    /// 成功返回0 失败返回ErrorCode, 返回系统的GetLastError()
    /// </RETURNS>
    unsigned long GetLoctionInfo(IN int index, OUT string& devLocationInfo);

    /// <SUMMARY>
    /// 获取设备实例ID
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// 设备索引
    /// </PARAM>
    /// <PARAM name = "devLocationInfo" dir = "OUT">
    /// 保存设备实例ID
    /// </PARAM>
    /// <RETURNS>
    /// 成功返回0 失败返回ErrorCode, 返回系统的GetLastError()
    /// </RETURNS>
    unsigned long GetInstanceID(IN int index, OUT string& devInstanceID);
    unsigned long GetInstanceID(IN int index, OUT wstring& devInstanceID);

    /// <SUMMARY>
    /// 获取父系设备实例ID
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// 设备索引
    /// </PARAM>
    /// <PARAM name = "devLocationInfo" dir = "OUT">
    /// 保存父系设备实例ID
    /// </PARAM>
    /// <RETURNS>
    /// 成功返回0 失败返回ErrorCode, 返回系统的GetLastError()
    /// </RETURNS>
    unsigned long GetParentInstanceId(IN int index, OUT string& devInstanceID);

    /// <SUMMARY>
    /// 获取子代设备数目
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// 设备索引
    /// </PARAM>
    /// <PARAM name = "number" dir = "OUT">
    /// 保存子代设备数目
    /// </PARAM>
    /// <RETURNS>
    /// 成功返回0 失败返回ErrorCode, 返回系统的GetLastError()
    /// </RETURNS>
    unsigned long GetChildrenNumber(IN int index, OUT int& number);

    /// <SUMMARY>
    /// 获取子代设备实例ID列表
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// 设备索引
    /// </PARAM>
    /// <PARAM name = "listSize" dir = "IN">
    /// 列表大小
    /// </PARAM>
    /// <PARAM name = "devInstanceIDList" dir = "OUT">
    /// 保存子代设备实例ID列表(请先使用GetChildrenNumber分配好需要的空间)
    /// </PARAM>
    /// <RETURNS>
    /// 成功返回0, 失败返回ErrorCode, 返回系统的GetLastError()
    /// </RETURNS>
    unsigned long GetChildrenInstanceIdList(IN int index, IN int listSize,OUT string* devInstanceIDList);

    /// <SUMMARY>
    /// 获取设备驱动键名称
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// 设备索引
    /// </PARAM>
    /// <PARAM name = "driverKeyName" dir = "OUT">
    /// 保存设备驱动键名称
    /// </PARAM>
    /// <RETURNS>
    /// 成功返回0 失败返回ErrorCode, 返回系统的GetLastError()
    /// </RETURNS>
    unsigned long GetDriverKeyName(IN int index, OUT string& driverKeyName);

    /// <SUMMARY>
    /// 获取设备类别字符串
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// 设备索引
    /// </PARAM>
    /// <PARAM name = "strClass" dir = "OUT">
    /// 保存设备类别字符串
    /// </PARAM>
    /// <RETURNS>
    /// 成功返回0 失败返回ErrorCode, 返回系统的GetLastError()
    /// </RETURNS>
    unsigned long GetClass(IN int index, OUT string& strClass);

    /// <SUMMARY>
    /// 获取设备类别GUID字符串
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// 设备索引
    /// </PARAM>
    /// <PARAM name = "strClassGuid" dir = "OUT">
    /// 保存设备类别GUID字符串
    /// </PARAM>
    /// <RETURNS>
    /// 成功返回0 失败返回ErrorCode, 返回系统的GetLastError()
    /// </RETURNS>
    unsigned long GetClassGuid(IN int index, OUT string& strClassGuid);

    /// <SUMMARY>
    /// 获取设备总线号
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// 设备索引
    /// </PARAM>
    /// <PARAM name = "busNumber" dir = "OUT">
    /// 保存设备总线号
    /// </PARAM>
    /// <RETURNS>
    /// 成功返回0 失败返回ErrorCode, 返回系统的GetLastError()
    /// </RETURNS>
    unsigned long GetBusNumber(IN int index, OUT unsigned int& busNumber);

    /// <SUMMARY>
    /// 获取设备制造商
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// 设备索引
    /// </PARAM>
    /// <PARAM name = "manufacturer" dir = "OUT">
    /// 保存设备制造商
    /// </PARAM>
    /// <RETURNS>
    /// 成功返回0 失败返回ErrorCode, 返回系统的GetLastError()
    /// </RETURNS>
    unsigned long GetManufacturer(IN int index, OUT string& manufacturer);
protected:
    LSetupDev();

protected:
    CSADevObject* m_pSADevObject;
};

/// <SUMMARY>
/// 显卡设备类
/// </SUMMARY>
class LSetupDisplayCard : public LSetupDev
{
public:
    LSetupDisplayCard();
    ~LSetupDisplayCard();
};

#endif