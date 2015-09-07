

#include "LSetupAPI.h"

#include <algorithm>  
using std::transform;
#include <string>
using std::string;
using std::wstring;
#include <vector>
using std::vector;

#include <Windows.h>
#include <InitGuid.h>
#include <devguid.h>
#include <SetupAPI.h>
#include <DEVPKEY.H>



#pragma comment(lib, "Setupapi.lib")

/// <SUMMARY>
/// ���ַ���ת��Ϊ�ַ���
/// </SUMMARY>
/// <PARAM name = " strSrc" dir = "IN">
/// Դ���ַ���
/// </PARAM>
/// <PARAM name = " strDest" dir = "OUT">
/// �洢ת������ַ���
/// </PARAM>
/// <RETURNS>
/// �ɹ�����0, ʧ�ܷ���false
/// </RETURNS>
static bool WStringToString(IN const wstring& strSrc, OUT string& strDest)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, strSrc.c_str(), -1, NULL, 0, NULL, NULL);  
	if (nLen<= 0)
		return false;

	char* pszDst = new char[nLen];  

	int iRet = WideCharToMultiByte(CP_ACP, 0, strSrc.c_str(), -1, pszDst, nLen, NULL, NULL); 

	pszDst[nLen -1] = 0;  
	strDest.clear();
	strDest.assign(pszDst);

	delete[] pszDst;

	return true;
}

/// <SUMMARY>
/// �ַ���ת��Ϊ���ַ���
/// </SUMMARY>
/// <PARAM name = " strSrc" dir = "IN">
/// Դ�ַ���
/// </PARAM>
/// <PARAM name = " strDest" dir = "OUT">
/// �洢ת����Ŀ��ַ���
/// </PARAM>
/// <RETURNS>
/// �ɹ�����0, ʧ�ܷ���false
/// </RETURNS>
static bool StringToWString(const string& strSrc, wstring& strDest)
{
    int nSize = MultiByteToWideChar(CP_ACP, 0, strSrc.c_str(), strSrc.length(), 0, 0);  
    if(nSize <= 0)
        return false;  
    wchar_t* pwszDst = new wchar_t[nSize+1];

    int iRet = MultiByteToWideChar(CP_ACP, 0, strSrc.c_str(), strSrc.length(), pwszDst, nSize);

    pwszDst[nSize] = 0; 

    strDest.clear();
    strDest.assign(pwszDst);

    delete[] pwszDst;

    return true;
}

/// <SUMMARY>
/// �豸������
/// ���෽������������Χ���м��
/// ���෽�����ص�ErrorCode��ȡ��GetLastError
/// </SUMMARY>
class CSADevObject
{
public:
    CSADevObject();
    ~CSADevObject();

    DWORD Scan(IN const GUID* pGUID);

    int GetDevNum();

    DWORD Enable(IN int index);

    DWORD Disable(IN int index);

    DWORD GetDevDesc(IN int index, OUT string& devDesc);

    DWORD GetHardwareId(IN int index, OUT string& devHardwareId);

    DWORD GetFriendlyName(IN int index, OUT string& devFrindName);

    DWORD GetLoctionInfo(IN int index, OUT string& devLocationInfo);

    DWORD GetInstanceID(IN int index, OUT string& devInstanceID);
    DWORD GetInstanceID(IN int index, OUT wstring& devInstanceID);

    DWORD GetParentInstanceId(IN int index, OUT string& devInstanceID);

    DWORD GetChildrenNumber(IN int index, OUT int& number);

    DWORD GetChildrenInstanceIdList(IN int index, IN int listSize,OUT string* devInstanceIDList);

    DWORD GetDriverKeyName(IN int index, OUT string& driverKeyName);

    DWORD GetClass(IN int index, OUT string& strClass);

    DWORD GetClassGuid(IN int index, OUT string& strClassGuid);

    DWORD GetBusNumber(IN int index, OUT unsigned int& busNumber);

    DWORD GetManufacturer(IN int index, OUT string& manufacturer);

protected:
    /// <SUMMARY>
    /// ��ȡ�豸ע�������(�ַ���)
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// �豸����
    /// </PARAM>
    /// <PARAM name = "dwProperty" dir = "IN">
    /// ָ�����豸����
    /// </PARAM>
    /// <PARAM name = "strProperty" dir = "OUT">
    /// �����豸����
    /// </PARAM>
    /// <RETURNS>
    /// �ɹ�����0 ʧ�ܷ���ErrorCode
    /// </RETURNS>
    DWORD GetRegistryPropertyStr(IN int index, IN DWORD dwProperty, OUT string& strProperty);

    /// <SUMMARY>
    /// ��ȡ�豸ע�������(�޷�������)
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// �豸����
    /// </PARAM>
    /// <PARAM name = "dwProperty" dir = "IN">
    /// ָ�����豸����
    /// </PARAM>
    /// <PARAM name = "strProperty" dir = "OUT">
    /// �����豸����
    /// </PARAM>
    /// <RETURNS>
    /// �ɹ�����0 ʧ�ܷ���ErrorCode
    /// </RETURNS>
    DWORD GetRegistryPropertyUInt(IN int index, IN DWORD dwProperty, OUT unsigned int& propertyValue);

    /// <SUMMARY>
    /// ��ȡ�豸����(�ַ���)
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// �豸����
    /// </PARAM>
    /// <PARAM name = "propertyKey" dir = "IN">
    /// ���Լ�
    /// </PARAM>
    /// <PARAM name = "strProperty" dir = "OUT">
    /// �����豸����
    /// </PARAM>
    /// <RETURNS>
    /// �ɹ�����0 ʧ�ܷ���ErrorCode
    /// </RETURNS>
    DWORD GetPropertyStr(IN int index, IN const DEVPROPKEY* propertyKey, OUT string& strProperty);

    /// <SUMMARY>
    /// ��ȡ�豸����(�ַ����б�)
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// �豸����
    /// </PARAM>
    /// <PARAM name = "propertyKey" dir = "IN">
    /// ���Լ�
    /// </PARAM>
    /// <PARAM name = "strPropertyList" dir = "OUT">
    /// �����豸�����б�
    /// </PARAM>
    /// <RETURNS>
    /// �ɹ�����0, ʧ�ܷ���ErrorCode
    /// </RETURNS>
    DWORD GetPropertyStrList(IN int index, IN const DEVPROPKEY* propertyKey, OUT vector<string>& strPropertyList);

    /// <SUMMARY>
    /// �ı��豸״̬
    /// �÷�����Ҫ����ԱȨ��
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// �豸����
    /// </PARAM>
    /// <PARAM name = "newState" dir = "IN">
    /// �µ�״̬
    /// </PARAM>
    /// <RETURNS>
    /// �ɹ�����0 ʧ�ܷ���ErrorCode
    /// </RETURNS>
    DWORD ChangeState(IN int index, IN DWORD newSate);

protected:
    HDEVINFO m_hDevInfoSet; ///<�豸��Ϣ��
    int m_devCount; ///<�豸��Ŀ
    SP_DEVINFO_DATA* m_pDevInfoList; /// �豸��Ϣ�б� 

private:
    /// <SUMMARY>
    /// ������Դ
    /// </SUMMARY>
    void BaseCleanUp();
};

CSADevObject::CSADevObject()
{
	m_hDevInfoSet = NULL;
	m_pDevInfoList = NULL;
	m_devCount = 0;
};

CSADevObject::~CSADevObject()
{
	BaseCleanUp();
}

DWORD CSADevObject::Scan(IN const GUID* pGUID)
{
	BaseCleanUp();

	DWORD returnCode = 0;
	if (pGUID != NULL)
	{
        /*
		if ((*pGUID) == GUID_DEVINTERFACE_USB_DEVICE ||
			(*pGUID) == GUID_DEVINTERFACE_USB_HUB ||
			(*pGUID) == GUID_DEVINTERFACE_USB_HOST_CONTROLLER)
		{
			m_hDevInfoSet = SetupDiGetClassDevsA(pGUID, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
		}
		else
		{
			m_hDevInfoSet = SetupDiGetClassDevsA(pGUID, NULL, NULL, DIGCF_PRESENT);
		}
        */
        m_hDevInfoSet = SetupDiGetClassDevsA(pGUID, NULL, NULL, DIGCF_PRESENT);
	}
	else
	{
		m_hDevInfoSet = SetupDiGetClassDevsA(NULL, NULL, NULL, DIGCF_PRESENT | DIGCF_ALLCLASSES);
	}
	if (m_hDevInfoSet == INVALID_HANDLE_VALUE)
	{
		m_hDevInfoSet = NULL;
		returnCode = GetLastError();
		return returnCode;
	}

	// ��ȡ�豸��Ŀ
	SP_DEVINFO_DATA devInfoData;
	devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	for (int i = 0; SetupDiEnumDeviceInfo(m_hDevInfoSet, i, &devInfoData); i++)
	{
		m_devCount = i + 1;
	}

	if (m_devCount == 0)
	{
		returnCode = 0;
		return returnCode;
	}

	// ��ȡ�豸��Ϣ
	m_pDevInfoList = new SP_DEVINFO_DATA[m_devCount];

	for (int i = 0; i < m_devCount; i++)
	{
		m_pDevInfoList[i].cbSize =  sizeof(SP_DEVINFO_DATA);
		BOOL bRet = SetupDiEnumDeviceInfo(m_hDevInfoSet, i, &m_pDevInfoList[i]);
		if (FALSE == bRet)
		{
			m_devCount = 0;
			returnCode = GetLastError();
			return returnCode;
		}
	}

	return returnCode;
}

int CSADevObject::GetDevNum()
{
	return m_devCount;
}

DWORD CSADevObject::Enable(IN int index)
{
    return this->ChangeState(index, DICS_ENABLE);
}

DWORD CSADevObject::Disable(IN int index)
{
    return this->ChangeState(index, DICS_DISABLE);
}

DWORD CSADevObject::GetDevDesc(IN int index, OUT string& devDesc)
{
	return this->GetRegistryPropertyStr(index, SPDRP_DEVICEDESC, devDesc);
}

DWORD CSADevObject::GetHardwareId(IN int index, OUT string& devHardwareId)
{
	return this->GetRegistryPropertyStr(index, SPDRP_HARDWAREID, devHardwareId);
}


DWORD CSADevObject::GetFriendlyName(IN int index, OUT string& devFrindName)
{
    return this->GetRegistryPropertyStr(index, SPDRP_FRIENDLYNAME, devFrindName);
}

DWORD CSADevObject::GetLoctionInfo(IN int index, OUT string& devLocationInfo)
{
    return this->GetRegistryPropertyStr(index, SPDRP_LOCATION_INFORMATION, devLocationInfo);
}

DWORD CSADevObject::GetInstanceID(IN int index, OUT string& devInstanceID)
{
    DWORD bufferSize = 0;
    PSTR pBuffer = NULL;

    DWORD returnCode = 0;
    while (true)
    {
        BOOL bRet = SetupDiGetDeviceInstanceIdA(m_hDevInfoSet,
            &m_pDevInfoList[index],
            pBuffer,
            bufferSize,
            &bufferSize);
        if (bRet == TRUE)
        {
            break;
        }

        if (bRet == FALSE)
        {
            if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
            {
                if (pBuffer)
                {
                    LocalFree(pBuffer);
                    pBuffer = NULL;
                }
                pBuffer = (LPSTR)LocalAlloc(LPTR, bufferSize);
            }
            else
            {
                returnCode = GetLastError();
                break;
            }
        }
    }

    if (returnCode == 0)
    {
        devInstanceID.clear();
        devInstanceID.append(pBuffer);
    }

    if (pBuffer)
    {
        LocalFree(pBuffer);
        pBuffer = NULL;
    }

    return returnCode;
}

DWORD CSADevObject::GetInstanceID(IN int index, OUT wstring& devInstanceID)
{
    DWORD bufferSize = 0;
    PWSTR pBuffer = NULL;

    DWORD returnCode = 0;
    while (true)
    {
        BOOL bRet = SetupDiGetDeviceInstanceIdW(m_hDevInfoSet,
            &m_pDevInfoList[index],
            pBuffer,
            bufferSize,
            &bufferSize);
        if (bRet == TRUE)
        {
            break;
        }

        if (bRet == FALSE)
        {
            if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
            {
                if (pBuffer)
                {
                    delete[] pBuffer;
                    pBuffer = NULL;
                }
                pBuffer = (LPWSTR)new wchar_t[bufferSize];
                ZeroMemory(pBuffer, bufferSize * sizeof(wchar_t));
            }
            else
            {
                returnCode = GetLastError();
                break;
            }
        }
    }

    if (returnCode == 0)
    {
        devInstanceID.clear();
        devInstanceID = pBuffer;
    }

    if (pBuffer)
    {
        delete[] pBuffer;
        pBuffer = NULL;
    }

    return returnCode;
}

DWORD CSADevObject::GetParentInstanceId(IN int index, OUT string& devInstanceID)
{
    DWORD dwRet = this->GetPropertyStr(index, &DEVPKEY_Device_Parent, devInstanceID);
    return dwRet;
}

DWORD CSADevObject::GetChildrenNumber(IN int index, OUT int& number)
{
    vector<string> propertyList;
    DWORD returnCode = this->GetPropertyStrList(index, &DEVPKEY_Device_Children, propertyList);
    number = propertyList.size();
    return returnCode;
}

DWORD CSADevObject::GetChildrenInstanceIdList(IN int index, IN int listSize,OUT string* devInstanceIDList)
{
    vector<string> propertyList;
    DWORD returnCode = this->GetPropertyStrList(index, &DEVPKEY_Device_Children, propertyList);
    if (returnCode != 0)
        return returnCode;

    if ((unsigned int)listSize < propertyList.size())
    {
        returnCode = ERROR_INSUFFICIENT_BUFFER;
        return  returnCode;
    }

    for (unsigned int i = 0; i < propertyList.size(); i++)
    {
        devInstanceIDList[i] = propertyList[i];
    }

    return returnCode;
}

DWORD CSADevObject::GetDriverKeyName(IN int index, OUT string& driverKeyName)
{
    return this->GetRegistryPropertyStr(index, SPDRP_DRIVER, driverKeyName);
}

DWORD CSADevObject::GetClass(IN int index, OUT string& strClass)
{
    return this->GetRegistryPropertyStr(index, SPDRP_CLASS, strClass);
}

DWORD CSADevObject::GetClassGuid(IN int index, OUT string& strClassGuid)
{
    return this->GetRegistryPropertyStr(index, SPDRP_CLASSGUID, strClassGuid);
}

DWORD CSADevObject::GetBusNumber(IN int index, OUT unsigned int& busNumber)
{
    return this->GetRegistryPropertyUInt(index, SPDRP_BUSNUMBER, busNumber);
}

DWORD CSADevObject::GetManufacturer(IN int index, OUT string& manufacturer)
{
    return this->GetPropertyStr(index, &DEVPKEY_Device_Manufacturer, manufacturer);
}


DWORD CSADevObject::GetRegistryPropertyStr(IN int index, IN DWORD dwProperty, OUT string& strProperty)
{
	DWORD bufferSize = 0;
	LPSTR pBuffer = NULL;
	DWORD dataType;

	DWORD returnCode = 0;
	while (true)
	{
		BOOL bRet = SetupDiGetDeviceRegistryPropertyA(m_hDevInfoSet,
			&m_pDevInfoList[index],
			dwProperty,
			&dataType,
			(PBYTE)pBuffer,
			bufferSize,
			&bufferSize);
		if (bRet == TRUE)
		{
			break;
		}

		if (bRet == FALSE)
		{
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				if (pBuffer)
				{
					LocalFree(pBuffer);
					pBuffer = NULL;
				}
				pBuffer = (LPSTR)LocalAlloc(LPTR, bufferSize);
			}
			else
			{
				returnCode = GetLastError();
				break;
			}
		}
	}

	if (returnCode == 0)
	{
		strProperty.clear();
		strProperty.append(pBuffer);
	}
	
	if (pBuffer)
	{
		LocalFree(pBuffer);
		pBuffer = NULL;
	}

	return returnCode;
}

DWORD CSADevObject::GetRegistryPropertyUInt(IN int index, IN DWORD dwProperty, OUT unsigned int& propertyValue)
{
	DWORD bufferSize = 0;
	BYTE* pBuffer = NULL;
	DWORD dataType;

	DWORD returnCode = 0;
	while (true)
	{
		BOOL bRet = SetupDiGetDeviceRegistryPropertyA(m_hDevInfoSet,
			&m_pDevInfoList[index],
			dwProperty,
			&dataType,
			pBuffer,
			bufferSize,
			&bufferSize);
		if (bRet == TRUE)
		{
			break;
		}

		if (bRet == FALSE)
		{
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				if (pBuffer)
				{
					LocalFree(pBuffer);
					pBuffer = NULL;
				}
				pBuffer = (BYTE*)LocalAlloc(LPTR, bufferSize);
			}
			else
			{
				returnCode = GetLastError();
				break;
			}
		}
	}

	if (returnCode == 0)
	{
		propertyValue = (unsigned int)(*pBuffer);
	}

	if (pBuffer)
	{
		LocalFree(pBuffer);
		pBuffer = NULL;
	}

	return returnCode;
}

DWORD CSADevObject::GetPropertyStr(IN int index, IN const DEVPROPKEY* propertyKey, OUT string& strProperty)
{
	DWORD bufferSize = 0;
	LPWSTR pBuffer = NULL;
	DEVPROPTYPE dataType;
	
	DWORD returnCode = 0;
	while (true)
	{
		BOOL bRet = SetupDiGetDevicePropertyW(m_hDevInfoSet,
			&m_pDevInfoList[index],
			propertyKey,
			&dataType,
			(PBYTE)pBuffer,
			bufferSize,
			&bufferSize,
			0);
		if (bRet == TRUE)
		{
			break;
		}

		if (bRet == FALSE)
		{
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				if (pBuffer)
				{
					LocalFree(pBuffer);
					pBuffer = NULL;
				}
				pBuffer = (LPWSTR)LocalAlloc(LPTR, bufferSize);
			}
			else
			{
				returnCode = GetLastError();
				break;
			}
		}
	}

	if (returnCode == 0)
	{
		wstring wstrProperty;
		wstrProperty.append(pBuffer);
		WStringToString(wstrProperty, strProperty);
	}

	if (pBuffer)
	{
		LocalFree(pBuffer);
		pBuffer = NULL;
	}

	return returnCode;
}

DWORD CSADevObject::GetPropertyStrList(IN int index, IN const DEVPROPKEY* propertyKey, OUT vector<string>& strPropertyList)
{
	DWORD bufferSize = 0;
	LPWSTR pBuffer = NULL;
	DEVPROPTYPE dataType;

	DWORD returnCode = 0;
	while (true)
	{
		BOOL bRet = SetupDiGetDevicePropertyW(m_hDevInfoSet,
			&m_pDevInfoList[index],
			propertyKey,
			&dataType,
			(PBYTE)pBuffer,
			bufferSize,
			&bufferSize,
			0);
		if (bRet == TRUE)
		{
			break;
		}

		if (bRet == FALSE)
		{
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				if (pBuffer)
				{
					LocalFree(pBuffer);
					pBuffer = NULL;
				}
				pBuffer = (LPWSTR)LocalAlloc(LPTR, bufferSize);
			}
			else
			{
				returnCode = GetLastError();
				break;
			}
		}
	}

	if (returnCode == 0)
	{
		wstring wstrProperty;
		string strProperty;
		WCHAR lastWChar = L'\0';
		for (unsigned int i = 0; i < bufferSize/sizeof(WCHAR); i++)
		{
			if (lastWChar == L'\0')
			{
				if (pBuffer[i] != L'\0')
				{
					wstrProperty = pBuffer + i;
					WStringToString(wstrProperty, strProperty);
					strPropertyList.push_back(strProperty);
				}
			}

			lastWChar = pBuffer[i];
		}
		
		
	}

	if (pBuffer)
	{
		LocalFree(pBuffer);
		pBuffer = NULL;
	}

	return returnCode;
}

DWORD CSADevObject::ChangeState(IN int index, IN DWORD newSate)
{
	DWORD returnCode = 0;

	SP_PROPCHANGE_PARAMS propChangeParam;
	propChangeParam.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
	propChangeParam.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
	propChangeParam.Scope = DICS_FLAG_GLOBAL;
	propChangeParam.StateChange = newSate;
	propChangeParam.HwProfile = 0;
	
	BOOL bRet = SetupDiSetClassInstallParamsA(m_hDevInfoSet, &m_pDevInfoList[index], 
		&propChangeParam.ClassInstallHeader, sizeof(propChangeParam));
	if (FALSE == bRet)
	{
		returnCode = GetLastError();
		return returnCode;
	}

	bRet = SetupDiChangeState(m_hDevInfoSet, &m_pDevInfoList[index]);
	if (bRet == TRUE) 
		returnCode = 0;
	else
		returnCode = GetLastError();

	return returnCode;
}



void CSADevObject::BaseCleanUp()
{
    if (m_hDevInfoSet != NULL)
    {
        SetupDiDestroyDeviceInfoList(m_hDevInfoSet);
        m_hDevInfoSet = NULL;
    }

    if (m_pDevInfoList != NULL)
    {
        delete [] m_pDevInfoList;
        m_pDevInfoList = NULL;
    }

    m_devCount = 0;
}

LSetupDev::LSetupDev()
{
    m_pSADevObject = new CSADevObject();
}

LSetupDev::~LSetupDev()
{
    delete m_pSADevObject;
}

int LSetupDev::GetDevNum()
{
    return m_pSADevObject->GetDevNum();
}

DWORD LSetupDev::Enable(IN int index)
{
    return m_pSADevObject->Enable(index);
}

DWORD LSetupDev::Disable(IN int index)
{
    return m_pSADevObject->Disable(index);
}

DWORD LSetupDev::GetDevDesc(IN int index, OUT string& devDesc)
{
    return m_pSADevObject->GetDevDesc(index, devDesc);
}

DWORD LSetupDev::GetHardwareId(IN int index, OUT string& devHardwareId)
{
    return m_pSADevObject->GetHardwareId(index, devHardwareId);
}

DWORD LSetupDev::GetFriendlyName(IN int index, OUT string& devFrindName)
{
    return m_pSADevObject->GetFriendlyName(index, devFrindName);
}

DWORD LSetupDev::GetLoctionInfo(IN int index, OUT string& devLocationInfo)
{
    return m_pSADevObject->GetLoctionInfo(index, devLocationInfo);
}

DWORD LSetupDev::GetInstanceID(IN int index, OUT string& devInstanceID)
{
    return m_pSADevObject->GetInstanceID(index, devInstanceID);
}

DWORD LSetupDev::GetInstanceID(IN int index, OUT wstring& devInstanceID)
{
    return m_pSADevObject->GetInstanceID(index, devInstanceID);
}

DWORD LSetupDev::GetParentInstanceId(IN int index, OUT string& devInstanceID)
{
    return m_pSADevObject->GetParentInstanceId(index, devInstanceID);
}

DWORD LSetupDev::GetChildrenNumber(IN int index, OUT int& number)
{
    return m_pSADevObject->GetChildrenNumber(index, number);
}

DWORD LSetupDev::GetChildrenInstanceIdList(IN int index, IN int listSize,OUT string* devInstanceIDList)
{
    return m_pSADevObject->GetChildrenInstanceIdList(index, listSize, devInstanceIDList);
}

DWORD LSetupDev::GetDriverKeyName(IN int index, OUT string& driverKeyName)
{
    return m_pSADevObject->GetDriverKeyName(index, driverKeyName);
}

DWORD LSetupDev::GetClass(IN int index, OUT string& strClass)
{
    return m_pSADevObject->GetClass(index, strClass);
}

DWORD LSetupDev::GetClassGuid(IN int index, OUT string& strClassGuid)
{
    return m_pSADevObject->GetClassGuid(index, strClassGuid);
}

DWORD LSetupDev::GetBusNumber(IN int index, OUT unsigned int& busNumber)
{
    return m_pSADevObject->GetBusNumber(index, busNumber);
}

DWORD LSetupDev::GetManufacturer(IN int index, OUT string& manufacturer)
{
    return m_pSADevObject->GetManufacturer(index, manufacturer);
}

LSetupDisplayCard::LSetupDisplayCard()
    : LSetupDev()
{
    m_pSADevObject->Scan(&GUID_DEVCLASS_DISPLAY);
}

LSetupDisplayCard::~LSetupDisplayCard()
{

}

#pragma pack (1) //ȡ���ڴ����
/// <SUMMARY>
/// ��ʾ����չ��ʾ��ʶ����
/// </SUMMARY>
struct LMonitorEDID
{
    unsigned char HeadInfor[8]; ///< ͷ��Ϣ, 8���ֽ�
    unsigned char VendorID[2]; ///< ����ID
    unsigned char ProductID[2]; ///< ��ƷID
    unsigned char SerialNumber[4]; ///< ���к�
    unsigned char Date[2]; ///< ��������
    unsigned char EDIDVersion[2]; ///< EDID�汾
    unsigned char BasicInfor[5]; ///< ��ʾ��������Ϣ(��Դ, ���߶�, ���)
    unsigned char ColorFeature[10]; ///< ��ʾ����ɫ����
    unsigned char OtherInfor[93]; ///< ������Ϣ
};
#pragma pack() // �ָ��ڴ����

LSetupMonitor::LSetupMonitor()
    : LSetupDev()
{
    m_pSADevObject->Scan(&GUID_DEVCLASS_MONITOR);
}

LSetupMonitor::~LSetupMonitor()
{

}

bool LSetupMonitor::GetExtendInfor(IN int index, OUT LMonitorExtendInfor& extendInfor)
{
    LMonitorEDID edid = {0};
    bool bRet = this->GetEDID(index, edid);
    if (!bRet)
        return false;

    string hardwareID;
    if (this->GetHardwareId(index, hardwareID) != 0)
        return false;

    int loc = hardwareID.find('\\');
    if (loc == string::npos)
        return false;

    extendInfor.Name = hardwareID.substr(loc);



    return true;
}


bool LSetupMonitor::GetEDID(IN int index, OUT LMonitorEDID& edid)
{
    bool bRet = false;
    HKEY hMonitorKey = NULL;
    LSTATUS lRet;
    string monitorKeyName;

    string instanceID;
    if (0 != this->GetInstanceID(index, instanceID))
    {
        bRet = false;
        goto SAFE_EXIT;
    }

    monitorKeyName = "SYSTEM\\CurrentControlSet\\Enum\\";
    monitorKeyName += instanceID;
    monitorKeyName += "\\Device Parameters";

    lRet = RegOpenKeyExA(HKEY_LOCAL_MACHINE, monitorKeyName.c_str(), 0, KEY_READ, &hMonitorKey);
    if (ERROR_SUCCESS != lRet)
    {
        bRet = false;
        goto SAFE_EXIT;
    }

    DWORD dataType = REG_BINARY;
    DWORD dataLen = sizeof(LMonitorEDID);
    lRet = RegQueryValueExA(hMonitorKey, "EDID", 0, &dataType, (BYTE*)&edid, &dataLen);
    if (ERROR_SUCCESS != lRet)
    {
        bRet = false;
        goto SAFE_EXIT;
    }

    bRet = true;

SAFE_EXIT:

    if (hMonitorKey != NULL)
    {
        RegCloseKey(hMonitorKey);
        hMonitorKey = NULL;
    }

    return bRet;
}


