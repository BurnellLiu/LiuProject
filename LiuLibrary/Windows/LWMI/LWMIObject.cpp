
#define _WIN32_DCOM

#include "LWMIObject.h"

#include <assert.h>
#include <stdlib.h>

#include <Windows.h>

#include <WbemIdl.h>
#include <comdef.h>
#pragma comment(lib, "WbemUuid.lib")

#include <comutil.h>
#pragma comment(lib, "comsuppw.lib")


#define LSAFE_RELEASE(p) do{if (p){p->Release(); p = 0;}}while(0)
#define LSAFE_DELARRAY(p) do{if (p){delete[] p; p = 0;}}while(0)

#define NAMESPACE_ROOT_WMI L"ROOT\\WMI"
#define NAMESPACE_ROOT_CIMV2 L"ROOT\\cimv2"


bool LStringToWString(const string& strSrc, wstring& strDest)
{
	int nSize = MultiByteToWideChar(CP_ACP, 0, strSrc.c_str(), strSrc.length(), 0, 0);  
	if(nSize <= 0)
		return false;  
	wchar_t* pwszDst = new wchar_t[nSize+1];
	assert(pwszDst != NULL);

	int iRet = MultiByteToWideChar(CP_ACP, 0, strSrc.c_str(), strSrc.length(), pwszDst, nSize);
	assert(iRet > 0);

	pwszDst[nSize] = 0; 

	strDest.clear();
	strDest.assign(pwszDst);

	LSAFE_DELARRAY(pwszDst);

	return true;
}

bool LWStringToString(const wstring& strSrc, string& strDest)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, strSrc.c_str(), -1, NULL, 0, NULL, NULL);  
	if (nLen<= 0)
		return false;

	char* pszDst = new char[nLen];  
	assert(pszDst != NULL);

	int iRet = WideCharToMultiByte(CP_ACP, 0, strSrc.c_str(), -1, pszDst, nLen, NULL, NULL); 
	assert(iRet > 0);

	pszDst[nLen -1] = 0;  
	strDest.clear();
	strDest.assign(pszDst);

	LSAFE_DELARRAY(pszDst);
	
	return true;
}

/// @brief COM初始化类
///
/// 只支持单线程
class LInitCom
{
public:
	LInitCom()
	{
		CoInitialize(NULL);
	}

	~LInitCom()
	{
		CoUninitialize();
	}
};

LInitCom g_lInitCom;  ///< 全局初始化化COM组件


LWMIObject::LWMIObject()
{
	m_pWbemLocator = NULL;
	m_pWbemServices = NULL;
	m_pEnumObject = NULL;
	m_pObjectArray = NULL;
	m_objectCount = 0;
}

LWMIObject::~LWMIObject()
{
	BaseCleanUp();
}

bool LWMIObject::Init()
{
	return false;
}

bool LWMIObject::BaseInit(const wchar_t* pNamespace)
{
	BaseCleanUp();

	if (pNamespace == NULL)
		return false;

	HRESULT hr = CoCreateInstance(CLSID_WbemLocator, NULL, CLSCTX_INPROC_SERVER, 
		IID_IWbemLocator, (LPVOID*)&m_pWbemLocator);
	assert(hr >= 0 && m_pWbemLocator != NULL);
	if (FAILED(hr)) return false;

	hr = m_pWbemLocator->ConnectServer(_bstr_t(pNamespace), NULL, NULL, NULL, 
		0, NULL, NULL, &m_pWbemServices);
	assert(hr >= 0 && m_pWbemServices != NULL);
	if (FAILED(hr)) return false;

	hr = CoSetProxyBlanket(m_pWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, 
		RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
	assert(hr >= 0);
	if (FAILED(hr)) return false;

	return true;
}

bool LWMIObject::GetStringProperty(int objectIndex, const wchar_t* pPropertyName, wstring& strProperty)
{
	if(objectIndex <0 || objectIndex >= m_objectCount)
		return false;
	if (pPropertyName == NULL)
		return false;

	strProperty.clear();

	VARIANT vtProp;
	VariantInit(&vtProp);
	HRESULT hr = m_pObjectArray[objectIndex]->Get(pPropertyName, 0, &vtProp, 0, 0);
	assert(hr == WBEM_S_NO_ERROR);
	strProperty.assign(vtProp.bstrVal);
	VariantClear(&vtProp);

	return true;
}

bool LWMIObject::GetUINT32Property(int objectIndex, const wchar_t* pPropertyName, LUINT& uiProperty)
{
	if(objectIndex <0 || objectIndex >= m_objectCount)
		return false;
	if (pPropertyName == NULL)
		return false;

	VARIANT vtProp;
	VariantInit(&vtProp);
	HRESULT hr = m_pObjectArray[objectIndex]->Get(pPropertyName, 0, &vtProp, 0, 0);
	assert(hr == WBEM_S_NO_ERROR);
	uiProperty = vtProp.uintVal;
	VariantClear(&vtProp);

	return true;
}

bool LWMIObject::GetUINT64Property(int objectIndex, const wchar_t* pPropertyName, LUINT64& ui64Property)
{
	if(objectIndex <0 || objectIndex >= m_objectCount)
		return false;
	if (pPropertyName == NULL)
		return false;

	/*
	64位整数以字符串的方式保存在VARIANT中
	*/
	VARIANT vtProp;
	VariantInit(&vtProp);
	HRESULT hr = m_pObjectArray[objectIndex]->Get(pPropertyName, 0, &vtProp, 0, 0);
	assert(hr == WBEM_S_NO_ERROR);
	ui64Property = _wcstoui64(vtProp.bstrVal, L'\0', 0);
	VariantClear(&vtProp);

	return true;
}

bool LWMIObject::WQLQuery(const wchar_t* pQuery)
{
	if (pQuery == NULL || m_pWbemServices == NULL)
		return false;

	for (int i = 0; i < m_objectCount; i ++)
	{
		LSAFE_RELEASE(m_pObjectArray[i]);
	}
	LSAFE_DELARRAY(m_pObjectArray);
	LSAFE_RELEASE(m_pEnumObject);

	HRESULT hr = m_pWbemServices->ExecQuery(_bstr_t(L"WQL"), _bstr_t(pQuery),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &m_pEnumObject);
	assert(hr >= 0 && m_pEnumObject != NULL);
	if (FAILED(hr)) return false;

	int maxSize = 5;
	m_pObjectArray = new IWbemClassObject*[maxSize];
	ZeroMemory(m_pObjectArray, maxSize * sizeof(IWbemClassObject*));

	ULONG uReturn = 0;
	while(true)
	{
		if (m_objectCount >= maxSize)
		{
			IWbemClassObject** pTempArray = new IWbemClassObject*[maxSize * 2];
			ZeroMemory(pTempArray, maxSize * 2 * sizeof(IWbemClassObject*));
			memcpy(pTempArray, m_pObjectArray, maxSize * sizeof(IWbemClassObject*));
			LSAFE_DELARRAY(m_pObjectArray);
			m_pObjectArray = pTempArray;
			pTempArray = NULL;
			maxSize = maxSize * 2;
		}
		hr = m_pEnumObject->Next(WBEM_INFINITE, 1, 
			&m_pObjectArray[m_objectCount], &uReturn);
		assert(hr >= 0);

		if(0 == uReturn)
		{
			break;
		}

		m_objectCount++;
	}

	return true;
}

void LWMIObject::BaseCleanUp()
{
	for (int i = 0; i < m_objectCount; i ++)
	{
		LSAFE_RELEASE(m_pObjectArray[i]);
	}
	m_objectCount = 0;
	LSAFE_DELARRAY(m_pObjectArray);
	LSAFE_RELEASE(m_pEnumObject);
	LSAFE_RELEASE(m_pWbemServices);
	LSAFE_RELEASE(m_pWbemLocator);
}

LWMIHddSmart::LWMIHddSmart()
{
}

LWMIHddSmart::~LWMIHddSmart()
{

}

bool LWMIHddSmart::Init()
{
	bool bRet = BaseInit(NAMESPACE_ROOT_WMI);
	if (bRet == false) return false;

	bRet = WQLQuery(L"SELECT * FROM MSStorageDriver_ATAPISmartData");
	if (bRet == false) return false;

	return true;
}

int LWMIHddSmart::GetHDDNum()
{
	return m_objectCount;
}

int LWMIHddSmart::GetHDDCurrentTemperature(int hddIndex)
{
	SmartAttriubets smartAttribets = {};
	bool bRet = GetSmartAttribute(hddIndex, TEMPERATURE, &smartAttribets);

	if (false == bRet)
		return -1;
	else
		return smartAttribets.Curent1;
}

bool LWMIHddSmart::GetHDDInstanceName(int hddIndex, wstring& instanceName)
{
	bool bRet = GetStringProperty(hddIndex, L"InstanceName", instanceName);
	return bRet;
}


bool LWMIHddSmart::GetSmartAttribute(int hddIndex, LBYTE attrib, SmartAttriubets* pSmartAttributes)
{
	if (hddIndex < 0 || hddIndex >= m_objectCount)
		return false;
	if (pSmartAttributes == NULL)
		return false;

	IWbemClassObject* pWbemClassObject = m_pObjectArray[hddIndex];

	VARIANT vtProp;
	VariantInit(&vtProp);
	HRESULT hr = pWbemClassObject->Get(L"VendorSpecific", 0, &vtProp, 0, 0);
	assert(hr == WBEM_S_NO_ERROR);

	SAFEARRAY* pSafeArray = NULL;
	pSafeArray = vtProp.parray;

	long LBound; // 下届
	long UBound; // 上届
	SafeArrayGetLBound(pSafeArray, 1, &LBound);
	SafeArrayGetUBound(pSafeArray, 1, &UBound);
	long arraySize = UBound - LBound + 1;

	LBYTE* pData = new LBYTE[arraySize];
	ZeroMemory(pData, arraySize);

	LBYTE* pBuffer = NULL;
	SafeArrayAccessData(pSafeArray, (void**)&pBuffer);
	memcpy(pData, pBuffer, arraySize);
	SafeArrayUnaccessData(pSafeArray);
	pBuffer = NULL;
	pSafeArray = NULL;
	VariantClear(&vtProp);

	/*
	pData中存储硬盘SMART数据,
	第一个和第二个字节代表SMART版本信息,
	从第3个字节起定义SMART的属性,每个属性为12字节长,
	每个属性的第一字节为当前属性定义,
	例:0xc2为温度属性
	*/
	LBYTE* pTemp = NULL;
	bool bGetAttrib = false;
	for (int i = 2; i < arraySize; i += 12)
	{
		pTemp = pData + i;
		if (*pTemp != attrib)
			continue;

		pSmartAttributes->Attrib = *pTemp;
		pSmartAttributes->Flags = *(pTemp + 1);
		pSmartAttributes->Worst = *(pTemp + 2);
		pSmartAttributes->Normal = *(pTemp + 3);
		pSmartAttributes->Curent = *(pTemp + 4);
		pSmartAttributes->Curent1 = *(pTemp + 5);
		pSmartAttributes->Curent2 = *(pTemp + 6);
		pSmartAttributes->Curent3 = *(pTemp + 7);
		pSmartAttributes->Curent4 = *(pTemp + 8);
		pSmartAttributes->Curent5 = *(pTemp + 9);
		pSmartAttributes->Curent6 = *(pTemp + 10);
		pSmartAttributes->Curent7 = *(pTemp + 11);

		bGetAttrib = true;
		break;
	}

	LSAFE_DELARRAY(pData);

	return bGetAttrib;
}

LWMIVideoController::LWMIVideoController()
{

};

LWMIVideoController::~LWMIVideoController()
{

}

bool LWMIVideoController::Init()
{
	bool bRet = BaseInit(NAMESPACE_ROOT_CIMV2);
	if (bRet == false) return false;

	bRet = WQLQuery(L"SELECT * FROM Win32_VideoController");
	if (bRet == false) return false;

	return true;
}

int LWMIVideoController::GetVideoNum()
{
	return m_objectCount;
}

bool LWMIVideoController::GetPNPDeviceID(int videoIndex, wstring& pnpDeviceID)
{
	bool bRet = GetStringProperty(videoIndex, L"PNPDeviceID", pnpDeviceID);
	return bRet;
}

bool LWMIVideoController::GetDescription(int videoIndex, wstring& description)
{
	bool bRet = GetStringProperty(videoIndex, L"Description", description);
	return bRet;
}

bool LWMIVideoController::GetAdapterRAM(int videoIndex, LUINT& adapterRAM)
{
	bool bRet = GetUINT32Property(videoIndex, L"AdapterRAM", adapterRAM);
	return bRet;
}

bool LWMIDiskDrive::Init()
{
	bool bRet = BaseInit(NAMESPACE_ROOT_CIMV2);
	if (bRet == false) return false;

	bRet = WQLQuery(L"SELECT * FROM Win32_DiskDrive");
	if (bRet == false) return false;

	return true;
}

LWMIDiskDrive::LWMIDiskDrive()
{

}

LWMIDiskDrive::~LWMIDiskDrive()
{

}

int LWMIDiskDrive::GetDiskNum()
{
	return m_objectCount;
}

bool LWMIDiskDrive::GetDiskModel(int diskIndex, wstring& diskModel)
{
	bool bRet = GetStringProperty(diskIndex, L"Model", diskModel);
	return bRet;
}

bool LWMIDiskDrive::GetDiskSize(int diskIndex, LUINT64& diskSize)
{
	bool bRet = GetUINT64Property(diskIndex, L"Size", diskSize);
	return bRet;
}