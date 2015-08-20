

#ifndef _LMIOBJECT_H_
#define _LMIOBJECT_H_

#include <string>
using std::wstring;
using std::string;

typedef unsigned char LBYTE;
typedef unsigned int LUINT;
typedef unsigned __int64 LUINT64;

struct IWbemLocator;
struct IWbemServices;
struct IEnumWbemClassObject;
struct IWbemClassObject;

/// @brief 多字节字符串转换为宽字节字符串
/// @param[in] strSrc 源字符串
/// @param[in] strDest 目标字符串
/// @return 成功返回true, 失败返回false
bool LStringToWString(const string& strSrc, wstring& strDest);

/// @brief 宽字节字符串转换为多字节字符串
/// @param[in] strSrc 源字符串
/// @param[in] strDest 目标字符串
/// @return 成功返回true, 失败返回false
bool LWStringToString(const wstring& strSrc, string& strDest);

/// @brief WMI对象基类
///
///
class LWMIObject
{
public:
	LWMIObject();
	virtual ~LWMIObject();

public:
	/// @brief 初始化
	/// @return 成功返回true, 失败返回false
	virtual bool Init() = 0;

protected:
	/// @brief 初始化
	/// @param[in] pNamespace 需要连接的名字空间
	/// @return 成功返回true, 失败返回false
	bool BaseInit(const wchar_t* pNamespace);

	/// @brief WQL查询
	/// @param[in] pQuery 查询语句
	/// @return 成功返回true, 失败返回false
	bool WQLQuery(const wchar_t* pQuery);

	/// @brief 获取字符串属性
	/// @param[in] objectIndex 对象索引
	/// @param[in] pPrppertyName 属性名称
	/// @param[out] strProperty 存储属性值
	/// @return 成功返回true, 失败返回false
	bool GetStringProperty(int objectIndex, const wchar_t* pPropertyName, wstring& strProperty);

	/// @brief 获取无符号整数属性
	/// @param[in] objectIndex 对象索引
	/// @param[in] pPropertyName 属性名称
	/// @param[out] uiProperty 存储属性值
	/// @return 成功返回true, 失败返回false
	bool GetUINT32Property(int objectIndex, const wchar_t* pPropertyName, LUINT& uiProperty);

	/// @brief 获取无符号64位整数属性
	/// @param[in] objectIndex 对象索引
	/// @param[in] pPropertyName 属性名称
	/// @param[out] ui64Property 存储属性值
	/// @return 成功返回true, 失败返回false
	bool GetUINT64Property(int objectIndex, const wchar_t* pPropertyName, LUINT64& ui64Property);

protected:
	IWbemLocator* m_pWbemLocator;
	IWbemServices* m_pWbemServices;

	IEnumWbemClassObject* m_pEnumObject;
	IWbemClassObject** m_pObjectArray;
	int m_objectCount;

private:
	/// @brief 清理资源
	void BaseCleanUp();

};

/// @brief WMI HDDSmart类
///
/// 使用该类的其他方法前,请先使用方法Init()来初始化对象
/// 使用该类,请使用管理员权限来运行程序
class LWMIHddSmart : public LWMIObject
{
public:
	LWMIHddSmart();
	virtual ~LWMIHddSmart();

public:
	/// @brief 获取HDD数目
	/// @return HDD数目
	int GetHDDNum();

	/// @brief 获取HDD的当前温度
	/// @param[in] hddIndex HDD索引
	/// @return 成功获取返回当前温度,失败返回-1
	int GetHDDCurrentTemperature(int hddIndex);

	/// @brief 获取硬盘实例名称
	/// @param[in] hddIndex 硬盘索引
	/// @param[out] instanceName 存储实例名称
	/// @return 成功返回true, 失败返回false
	bool GetHDDInstanceName(int hddIndex, wstring& instanceName);

	/// @brief 初始化
	/// @return 成功返回true, 失败返回false
	virtual bool Init();

private:
	/// @breief SMART 属性结构
	struct SmartAttriubets
	{
		LBYTE Attrib;
		LBYTE Flags;
		LBYTE Worst;
		LBYTE Normal;
		LBYTE Curent;
		LBYTE Curent1;
		LBYTE Curent2;
		LBYTE Curent3;
		LBYTE Curent4;
		LBYTE Curent5;
		LBYTE Curent6;
		LBYTE Curent7;
	};

	/// @breief SMART 属性
	enum SMART_ATTRIB
	{
		USE_TIME = 0x09,  ///< 使用时间
		TEMPERATURE = 0xc2 ///< 温度
	};

	/// @brief 获取HDD Smart属性
	/// @param[in] hddIndex HDD索引
	/// @param[in] attrib 需要获取的属性
	/// @param[out] pSmartAttributes 存储Smart属性
	/// @return 成功返回true, 失败返回false
	bool GetSmartAttribute(int hddIndex, LBYTE attrib, SmartAttriubets* pSmartAttributes);

private:
	// 禁止赋值操作符和复制构造函数
	LWMIHddSmart(const LWMIHddSmart&);
	LWMIHddSmart& operator = (const LWMIHddSmart&);
};

/// @brief WMI VideoController类
///
/// 使用该类的其他方法前,请先使用方法Init()来初始化对象
class LWMIVideoController : public LWMIObject
{
public:
	LWMIVideoController();
	virtual ~LWMIVideoController();

public:
	/// @brief 初始化
	/// @return 成功返回true, 失败返回false
	virtual bool Init();

	/// @brief 获取Video数目
	/// @return Video数目
	int GetVideoNum();

	/// @brief 获取video PNPDeviceID
	/// @param[in] videoIndex video索引
	/// @param[out] pnpDeviceID
	/// @return 成功返回true, 失败返回false
	bool GetPNPDeviceID(int videoIndex, wstring& pnpDeviceID);

	/// @brief 获取video AdapterRAM
	/// @param[in] videoIndex video索引
	/// @param[out] adapterRAM 存储adapterRAM
	/// @return 成功返回true, 失败返回false
	bool GetAdapterRAM(int videoIndex, LUINT& adapterRAM);

	/// @brief 获取video 设备描述
	/// @param[in] videoIndex video索引
	/// @param[out] description 存储设备描述
	/// @return 成功返回true, 失败返回false
	bool GetDescription(int videoIndex, wstring& description);

private:
	// 禁止赋值操作符和复制构造函数
	LWMIVideoController(const LWMIVideoController&);
	LWMIVideoController& operator = (const LWMIVideoController&);
};

class LWMIDiskDrive : public LWMIObject
{
public:
	LWMIDiskDrive();
	~LWMIDiskDrive();

public:
	/// @brief 初始化
	/// @return 成功返回true, 失败返回false
	virtual bool Init();

	/// @brief 获取Disk数目
	/// @return Disk数目
	int GetDiskNum();

	/// @brief 获取DiskModel
	/// @param[in] diskIndex Disk索引
	/// @param[out] diskModel 存储DiskModel
	/// @return 成功返回true, 失败返回false
	bool GetDiskModel(int diskIndex, wstring& diskModel);

	/// @brief 获取DiskSize
	/// @param[in] diskIndex Disk索引
	/// @param[out] diskSize 存储DiskSize
	/// @return 成功返回true, 失败返回false
	bool GetDiskSize(int diskIndex, LUINT64& diskSize);

private:
	// 禁止赋值操作符和复制构造函数
	LWMIDiskDrive(const LWMIDiskDrive&);
	LWMIDiskDrive& operator = (const LWMIDiskDrive&);
};

#endif

