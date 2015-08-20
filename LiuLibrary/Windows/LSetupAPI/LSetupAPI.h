#ifndef _LSETUPAPI_H_
#define _LSETUPAPI_H_

#include <string>
using std::wstring;
using std::string;

#include <Windows.h>
#include <devguid.h>
#include <SetupAPI.h>

/// @brief SetupAPI设备对象基类
///
///
class LSADevObject
{
public:
	LSADevObject();
	virtual ~LSADevObject();

	/// @brief 初始化
	/// @return 成功返回true 失败返回false
	virtual bool Init() = 0;

	/// @brief 获取设备数目
	/// @return 设备数目
	int GetDevNum();

	/// @brief 禁用设备
	///
	/// 该方法需要管理员权限
	/// @param[in] int index 设备索引
	/// @return 成功返回true 失败返回false
	bool Enable(IN int index);

	/// @brief 启用设备
	///
	/// 该方法需要管理员权限, 需要重启才可Disable的设备, 该方法也返回true
	/// @param[in] index 设备索引
	/// @param[out] bNeedRestart 是否需要重启
	/// @return 成功返回true 失败返回false
	bool Disable(IN int index, OUT bool& bNeedRestart);

	/// @brief 获取设备描述
	/// @param[in] index
	/// @param[out] devDesc
	/// @return 成功返回true 失败返回false
	bool GetDevDesc(IN int index, OUT string& devDesc);

	/// @brief 设备是否已经被禁用
	/// @param[in] index 设备索引
	/// @return 被禁用返回true 否则返回false
	bool IsDisabled(IN int index);

	/// @brief 设备是否可以被禁用
	/// @param[in] index 设备索引
	/// @return 可以被禁用返回true 否则返回false
	bool IsDisableEnable(IN int index);

protected:
	/// @brief 初始化
	/// @param[in] pGUID 要初始化的设备类GUID
	/// @return 成功返回true 失败返回false
	bool BaseInit(IN const GUID* pGUID);

	/// @brief 获取设备属性(字符串)
	/// @param[in] index 指定的设备索引
	/// @param[in] dwProperty 指定的设备属性
	/// @param[in] OUT strProperty 设备属性
	/// @return 成功返回true 失败返回false
	bool GetRegistryPropertyStr(IN int index, IN DWORD dwProperty, OUT string& strProperty);

	/// @brief 改变设备状态
	///
	/// 该方法需要管理员权限
	/// @param[in] index 设备索引
	/// @param[in] newSate 新的状态
	/// @return 成功返回true 失败返回false
	bool ChangeState(IN int index, IN DWORD newSate, OUT bool& bNeedRestart);

protected:
	HDEVINFO m_hDevInfoSet; ///< 设备信息集
	int m_devCount; ///< 设备数目
	SP_DEVINFO_DATA* m_pDevInfoList; ///< 设备信息列表 

private:
	/// @brief 清理资源
	void BaseCleanUp();
};

/// @brief 人体学输入设备
class LSAHumanInputDev : public LSADevObject
{
public:
	LSAHumanInputDev();
	~LSAHumanInputDev();

	/// @brief 初始化
	virtual bool Init();
};

/// @brief 鼠标设备
class LSAMouseDev : public LSADevObject
{
public:
	LSAMouseDev();
	~LSAMouseDev();

	/// @brief 初始化
	virtual bool Init();
};

class LSABluetoothDev : public LSADevObject
{
public:
	LSABluetoothDev();
	~LSABluetoothDev();

	virtual bool Init();
};
#endif