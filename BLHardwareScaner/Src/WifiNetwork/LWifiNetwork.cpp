
#include "LWifiNetwork.h"



#include <string>
using std::string;
using std::wstring;
#include <sstream>
using std::stringstream;
#include <vector>
using std::vector;

#include <windows.h>
#include <wlanapi.h>

#pragma comment(lib, "Wlanapi.lib")
#pragma comment(lib, "Rpcrt4.lib")

/// <SUMMARY>
/// WIFI�ṹ
/// </SUMMARY>
struct SWifiNetwork
{
    string SSID; // ��������
    bool IsCurrentConnected; // �Ƿ�Ϊ��ǰ���ӵ�����
};


/// <SUMMARY>
/// WIFI����ʵ����
/// </SUMMARY>
class CWifiNetwork
{
public:
    CWifiNetwork();
    ~CWifiNetwork();

    /// <SUMMARY>
    /// WLAN�Ƿ����
    /// </SUMMARY>
    /// <RETURNS>
    /// ���ڷ���true, �����ڷ���false
    /// </RETURNS>
    bool IsWLANExist();

    /// <SUMMARY>
    /// ��ȡWLAN����
    /// </SUMMARY>
    /// <PARAM name = "desc" dir = "OUT">
    /// ����
    /// </PARAM>
    /// <RETURNS>
    ///  �ɹ���ȡ����true, ��ȡʧ����false
    /// </RETURNS>
    bool GetWLANDesc(OUT wstring& desc);

    /// <SUMMARY>
    /// ��ȡWLAN GUID
    /// </SUMMARY>
    /// <PARAM name = "guid" dir = "OUT">
    /// GUID
    /// </PARAM>
    /// <RETURNS>
    /// �ɹ���ȡ����true, ��ȡʧ����false
    /// </RETURNS>
    bool GetWLANGUID(OUT GUID& guid);

    /// <SUMMARY>
    /// �ж�Ӳ��Wifi�����Ƿ񱻿���
    /// </SUMMARY>
    /// <RETURNS>
    /// �Ѿ���������true, û�п�������false
    /// </RETURNS>
    bool HardwareWLANIsEnabled();

    /// <SUMMARY>
    /// �ж����Wifi�����Ƿ񱻿���
    /// </SUMMARY>
    /// <RETURNS>
    /// �Ѿ���������true, û�п�������false
    /// </RETURNS>
    bool SoftwareWLANIsEnabled();

    /// <SUMMARY>
    /// ���忪��Wifi
    /// </SUMMARY>
    /// <RETURNS>
    /// �ɹ���������true, ����ʧ�ܷ���false
    /// </RETURNS>
    bool SoftwareEnableWLAN();

    /// <SUMMARY>
    /// ����ر�Wifi
    /// </SUMMARY>
    /// <RETURNS>
    /// �ɹ��رշ���true, �ر�ʧ�ܷ���false
    /// </RETURNS>
    bool SoftwareDisableWLAN();

    /// <SUMMARY>
    /// �ж�WLAN�Ƿ���������������
    /// </SUMMARY>
    /// <RETURNS>
    /// �Ѿ����ӷ���true, û�����ӷ���false
    /// </RETURNS>
    bool WLANIsConnected();

    /// <SUMMARY>
    /// �ж�WLAN�Ƿ���������������
    /// </SUMMARY>
    /// <PARAM name = "ssid" dir = "OUT">
    /// ��������SSID(wifi����)
    /// </PARAM>
    /// <RETURNS>
    /// �Ѿ����ӷ���true, û�����ӷ���false
    /// </RETURNS>
    bool WLANIsConnected(OUT string& ssid);

    /// <SUMMARY>
    /// �Ͽ�������������
    /// </SUMMARY>
    /// <RETURNS>
    /// �ɹ�����true, ʧ�ܷ���false
    /// </RETURNS>
    bool WLANDisconnect();

    /// <SUMMARY>
    /// ���ӵ�ָ������
    /// </SUMMARY>
    /// <PARAM name = "ssid" dir = "IN">
    /// ����SSID(wifi����)
    /// </PARAM>
    /// <PARAM name = "key" dir = "IN">
    /// ������Կ(wifi����)
    /// </PARAM>
    /// <RETURNS>
    /// �ɹ���ȡ����true, ��ȡʧ����false, ��ʹ��GetErrorMessage��ȡ������Ϣ
    /// </RETURNS>
    bool WLANConnect(IN const string& ssid, IN const string& key);

    /// <SUMMARY>
    /// ���ӵ�ָ������
    /// Ҫ��ϵͳ���Ѿ����ڸ��������ӵ������ļ�
    /// </SUMMARY>
    /// <PARAM name = "ssid" dir = "IN">
    /// ����SSID(wifi����)
    /// </PARAM>
    /// <RETURNS>
    /// �ɹ���ȡ����true, ��ȡʧ����false, ��ʹ��GetErrorMessage��ȡ������Ϣ
    /// </RETURNS>
    bool WLANConnect(IN const string& ssid);

    /// <SUMMARY>
    /// ��ȡ���������б�
    /// </SUMMARY>
    /// <PARAM name = "networkist" dir = "OUT">
    /// �����б�
    /// </PARAM>
    /// <RETURNS>
    ///  �ɹ���ȡ����true, ��ȡʧ����false, ��ʹ��GetErrorMessage��ȡ������Ϣ
    /// </RETURNS>
    bool GetAvailableNetworkList(OUT vector<SWifiNetwork>& networkist);

    /// <SUMMARY>
    /// ��ȡ������Ϣ
    /// </SUMMARY>
    /// <RETURNS>
    /// ������Ϣ
    /// </RETURNS>
    string GetErrorMessage();

private:
    /// <SUMMARY>
    /// WIFI���������ļ��ṹ
    /// </SUMMARY>
    struct SWifiProfile
    {
        string SSID; // ����SSID
        string Key; // ������Կ
        DOT11_AUTH_ALGORITHM AuthAlgo; // ��֤�㷨
        DOT11_CIPHER_ALGORITHM CipherAlgo; // �����㷨
    };    

    /// <SUMMARY>
    /// ��ʼ��
    /// </SUMMARY>
    /// <RETURNS>
    /// ��ʼ���ɹ�����true, ʧ�ܷ���false, ��ʹ��GetErrorMessage��ȡ������Ϣ
    /// </RETURNS>
    bool Init();

    /// <SUMMARY>
    /// ������Դ
    /// </SUMMARY>
    void UnInit();

    void CreateWifiProfile(const SWifiProfile& wifiProfile, OUT wstring& profileStr);

private:
    bool m_bWLANExist; // ��ʶWLAN�Ƿ����
    bool m_bInitSuccess; // ��ʶ�Ƿ��Ѿ���ʼ���ɹ�
    HANDLE m_hWLAN; // �����������
    GUID m_guidWLAN; // ���������ӿ�GUID
    wstring m_descWLAN; // ���������ӿ�����
    stringstream m_errStream; // ��������
};

CWifiNetwork::CWifiNetwork()
{
    m_hWLAN = NULL;
    m_bInitSuccess = false;
    m_bWLANExist = false;

    if (this->Init())
        m_bInitSuccess = true;
    else
        m_bInitSuccess = false;
};

CWifiNetwork::~CWifiNetwork()
{
    this->UnInit();
}


string CWifiNetwork::GetErrorMessage()
{
    return m_errStream.str();
}

bool CWifiNetwork::Init()
{
    bool bReturn = false;

    DWORD dwRet;
    DWORD version;
    PWLAN_INTERFACE_INFO_LIST pWLANInterfaceList = NULL;

    m_bInitSuccess = false;

    this->UnInit();

    dwRet = WlanOpenHandle(2, NULL, &version, &m_hWLAN);
    if (dwRet != ERROR_SUCCESS)
    {
        m_errStream.str("");
        m_errStream << "WlanOpenHandle FAIL, Error code: " << dwRet;
        bReturn = false;
        goto SAFE_EXIT;
    }

  
    dwRet = WlanEnumInterfaces(m_hWLAN, NULL, &pWLANInterfaceList);
    if (dwRet != ERROR_SUCCESS)
    {
        m_errStream.str("");
        m_errStream << "WlanEnumInterfaces FAIL, Error code: " << dwRet;
        bReturn = false;
        goto SAFE_EXIT;
    }

    bReturn = true;
    m_bInitSuccess = true;

    if (pWLANInterfaceList->dwNumberOfItems > 0)
    {
        m_guidWLAN = pWLANInterfaceList->InterfaceInfo[0].InterfaceGuid;
        m_descWLAN = pWLANInterfaceList->InterfaceInfo[0].strInterfaceDescription;
        m_bWLANExist = true;
    }
    else
    {
        m_bWLANExist = false;
    }

SAFE_EXIT:
    if (pWLANInterfaceList != NULL)
    {
        WlanFreeMemory(pWLANInterfaceList);
        pWLANInterfaceList = NULL;
    }

    return bReturn;
}

bool CWifiNetwork::IsWLANExist()
{
    if (m_bInitSuccess && m_bWLANExist)
        return true;

    return false;
}

bool CWifiNetwork::GetWLANDesc(OUT wstring& desc)
{
    if (!m_bInitSuccess)
        return false;

    if (!m_bWLANExist)
        return false;

    desc = m_descWLAN;

    return true;
}

bool CWifiNetwork::GetWLANGUID(OUT GUID& guid)
{
    if (!m_bInitSuccess)
        return false;

    if (!m_bWLANExist)
        return false;

    guid = m_guidWLAN;

    return true;
}

bool CWifiNetwork::HardwareWLANIsEnabled()
{
    bool bReturn = false;

    DWORD dwRet;
    PWLAN_INTERFACE_INFO_LIST pWLANInterfaceList = NULL;
    PWLAN_RADIO_STATE pWLANRadioState = NULL;

    if (!m_bInitSuccess)
    {
        bReturn = false;
        goto SAFE_EXIT;
    }

    if (!m_bWLANExist)
    {
        bReturn = false;
        goto SAFE_EXIT;
    }

    dwRet = WlanEnumInterfaces(m_hWLAN, NULL, &pWLANInterfaceList);
    if (dwRet != ERROR_SUCCESS)
    {
        m_errStream.str("");
        m_errStream << "WlanEnumInterfaces FAIL, Error code: " << dwRet;
        bReturn = false;
        goto SAFE_EXIT;
    }

    if (pWLANInterfaceList->dwNumberOfItems < 1)
    {
        bReturn = false;
        goto SAFE_EXIT;
    }

    
    DWORD dwDataSize = sizeof(WLAN_RADIO_STATE);
    dwRet = WlanQueryInterface(
        m_hWLAN, 
        &(pWLANInterfaceList->InterfaceInfo[0].InterfaceGuid),
        wlan_intf_opcode_radio_state,
        NULL,
        &dwDataSize,
        (PVOID*)&pWLANRadioState,
        NULL
        );
    if (dwRet != ERROR_SUCCESS)
    {
        m_errStream.str("");
        m_errStream << "WlanQueryInterface FAIL, Error code: " << dwRet;
        bReturn = false;
        goto SAFE_EXIT;
    }

    if (pWLANRadioState->PhyRadioState[0].dot11HardwareRadioState == dot11_radio_state_on)
    {
        bReturn = true;
        goto SAFE_EXIT;
    }

SAFE_EXIT:

    if (pWLANRadioState != NULL)
    {
        WlanFreeMemory(pWLANRadioState);
        pWLANRadioState = NULL;
    }

    if (pWLANInterfaceList != NULL)
    {
        WlanFreeMemory(pWLANInterfaceList);
        pWLANInterfaceList = NULL;
    }
    return bReturn;
}

bool CWifiNetwork::SoftwareWLANIsEnabled()
{
    bool bReturn = false;

    DWORD dwRet;
    PWLAN_INTERFACE_INFO_LIST pWLANInterfaceList = NULL;
    PWLAN_RADIO_STATE pWLANRadioState = NULL;

    if (!m_bInitSuccess)
    {
        bReturn = false;
        goto SAFE_EXIT;
    }

    if (!m_bWLANExist)
    {
        bReturn = false;
        goto SAFE_EXIT;
    }

    dwRet = WlanEnumInterfaces(m_hWLAN, NULL, &pWLANInterfaceList);
    if (dwRet != ERROR_SUCCESS)
    {
        m_errStream.str("");
        m_errStream << "WlanEnumInterfaces FAIL, Error code: " << dwRet;
        bReturn = false;
        goto SAFE_EXIT;
    }

    if (pWLANInterfaceList->dwNumberOfItems < 1)
    {
        bReturn = false;
        goto SAFE_EXIT;
    }


    DWORD dwDataSize = sizeof(WLAN_RADIO_STATE);
    dwRet = WlanQueryInterface(
        m_hWLAN, 
        &(pWLANInterfaceList->InterfaceInfo[0].InterfaceGuid),
        wlan_intf_opcode_radio_state,
        NULL,
        &dwDataSize,
        (PVOID*)&pWLANRadioState,
        NULL
        );
    if (dwRet != ERROR_SUCCESS)
    {
        m_errStream.str("");
        m_errStream << "WlanQueryInterface FAIL, Error code: " << dwRet;
        bReturn = false;
        goto SAFE_EXIT;
    }

    if (pWLANRadioState->PhyRadioState[0].dot11SoftwareRadioState == dot11_radio_state_on)
    {
        bReturn = true;
        goto SAFE_EXIT;
    }

SAFE_EXIT:

    if (pWLANRadioState != NULL)
    {
        WlanFreeMemory(pWLANRadioState);
        pWLANRadioState = NULL;
    }

    if (pWLANInterfaceList != NULL)
    {
        WlanFreeMemory(pWLANInterfaceList);
        pWLANInterfaceList = NULL;
    }
    return bReturn;
}

bool CWifiNetwork::SoftwareEnableWLAN()
{
    bool bReturn = false;

    DWORD dwRet;
    PWLAN_INTERFACE_INFO_LIST pWLANInterfaceList = NULL;
    PWLAN_RADIO_STATE pWLANRadioState = NULL;

    if (!m_bInitSuccess)
    {
        bReturn = false;
        goto SAFE_EXIT;
    }

    if (!m_bWLANExist)
    {
        bReturn = false;
        goto SAFE_EXIT;
    }

    dwRet = WlanEnumInterfaces(m_hWLAN, NULL, &pWLANInterfaceList);
    if (dwRet != ERROR_SUCCESS)
    {
        m_errStream.str("");
        m_errStream << "WlanEnumInterfaces FAIL, Error code: " << dwRet;
        bReturn = false;
        goto SAFE_EXIT;
    }

    if (pWLANInterfaceList->dwNumberOfItems < 1)
    {
        bReturn = false;
        goto SAFE_EXIT;
    }

    WLAN_PHY_RADIO_STATE radioState;
    radioState.dwPhyIndex = 0;
    radioState.dot11SoftwareRadioState = dot11_radio_state_on;
    PVOID pData = &radioState;

    dwRet = WlanSetInterface(
        m_hWLAN,
        &(pWLANInterfaceList->InterfaceInfo[0].InterfaceGuid),
        wlan_intf_opcode_radio_state,
        sizeof(WLAN_PHY_RADIO_STATE),
        pData,
        NULL
        );

    if (dwRet != ERROR_SUCCESS)
    {
        m_errStream.str("");
        m_errStream << "WlanSetInterface FAIL, Error code: " << dwRet;
        bReturn = false;
        goto SAFE_EXIT;
    }

    bReturn = TRUE;

SAFE_EXIT:

    if (pWLANRadioState != NULL)
    {
        WlanFreeMemory(pWLANRadioState);
        pWLANRadioState = NULL;
    }

    if (pWLANInterfaceList != NULL)
    {
        WlanFreeMemory(pWLANInterfaceList);
        pWLANInterfaceList = NULL;
    }
    return bReturn;
}

bool CWifiNetwork::SoftwareDisableWLAN()
{
    bool bReturn = false;

    DWORD dwRet;
    PWLAN_INTERFACE_INFO_LIST pWLANInterfaceList = NULL;
    PWLAN_RADIO_STATE pWLANRadioState = NULL;

    if (!m_bInitSuccess)
    {
        bReturn = false;
        goto SAFE_EXIT;
    }

    if (!m_bWLANExist)
    {
        bReturn = false;
        goto SAFE_EXIT;
    }

    dwRet = WlanEnumInterfaces(m_hWLAN, NULL, &pWLANInterfaceList);
    if (dwRet != ERROR_SUCCESS)
    {
        m_errStream.str("");
        m_errStream << "WlanEnumInterfaces FAIL, Error code: " << dwRet;
        bReturn = false;
        goto SAFE_EXIT;
    }

    if (pWLANInterfaceList->dwNumberOfItems < 1)
    {
        bReturn = false;
        goto SAFE_EXIT;
    }

    WLAN_PHY_RADIO_STATE radioState;
    radioState.dwPhyIndex = 0;
    radioState.dot11SoftwareRadioState = dot11_radio_state_off;
    PVOID pData = &radioState;

    dwRet = WlanSetInterface(
        m_hWLAN,
        &(pWLANInterfaceList->InterfaceInfo[0].InterfaceGuid),
        wlan_intf_opcode_radio_state,
        sizeof(WLAN_PHY_RADIO_STATE),
        pData,
        NULL
        );

    if (dwRet != ERROR_SUCCESS)
    {
        m_errStream.str("");
        m_errStream << "WlanSetInterface FAIL, Error code: " << dwRet;
        bReturn = false;
        goto SAFE_EXIT;
    }

    bReturn = TRUE;

SAFE_EXIT:

    if (pWLANRadioState != NULL)
    {
        WlanFreeMemory(pWLANRadioState);
        pWLANRadioState = NULL;
    }

    if (pWLANInterfaceList != NULL)
    {
        WlanFreeMemory(pWLANInterfaceList);
        pWLANInterfaceList = NULL;
    }
    return bReturn;
}

bool CWifiNetwork::WLANIsConnected()
{
    bool bReturn = false;

    DWORD dwRet;
    PWLAN_INTERFACE_INFO_LIST pWLANInterfaceList = NULL;

    if (!m_bInitSuccess)
    {
        bReturn = false;
        goto SAFE_EXIT;
    }

    if (!m_bWLANExist)
    {
        bReturn = false;
        goto SAFE_EXIT;
    }

    dwRet = WlanEnumInterfaces(m_hWLAN, NULL, &pWLANInterfaceList);
    if (dwRet != ERROR_SUCCESS)
    {
        m_errStream.str("");
        m_errStream << "WlanEnumInterfaces FAIL, Error code: " << dwRet;
        bReturn = false;
        goto SAFE_EXIT;
    }

    if (pWLANInterfaceList->dwNumberOfItems > 0)
    {
        if (pWLANInterfaceList->InterfaceInfo[0].isState == wlan_interface_state_connected)
            bReturn = true;
        else
            bReturn = false;

        goto SAFE_EXIT;
    }
    else
    {
        bReturn = false;
        goto SAFE_EXIT;
    }

SAFE_EXIT:
    if (pWLANInterfaceList != NULL)
    {
        WlanFreeMemory(pWLANInterfaceList);
        pWLANInterfaceList = NULL;
    }
    return bReturn;
}

bool CWifiNetwork::WLANIsConnected(OUT string& ssid)
{
	bool bRet = false;
	vector<SWifiNetwork> networkList;
	this->GetAvailableNetworkList(networkList);
	for (unsigned int i = 0; i < networkList.size(); i++)
	{
		if (networkList[i].IsCurrentConnected)
		{
			ssid = networkList[i].SSID;
			bRet = true;
			break;
		}
	}

	return bRet;
}

bool CWifiNetwork::WLANDisconnect()
{
    if (!m_bInitSuccess)
        return false;

    if (!m_bWLANExist)
        return false;

    DWORD dwRet = WlanDisconnect(m_hWLAN, &m_guidWLAN, NULL);
    if (dwRet != ERROR_SUCCESS)
    {
        m_errStream.str("");
        m_errStream << "WlanDisconnect FAIL, Error code: " << dwRet;
        return false;
    }

    return true;
}

bool CWifiNetwork::WLANConnect(IN const string& ssid, IN const string& key)
{
    bool bReturn = false;
    WLAN_REASON_CODE reasonCode;
    DWORD dwRet;
    int destNetworkIndex = -1;
    PWLAN_AVAILABLE_NETWORK_LIST pNetworkList = NULL;
    WLAN_CONNECTION_PARAMETERS connParam;

    SWifiProfile wifiProfile;
    wstring profileStr;

    if (!m_bInitSuccess)
    {
        bReturn = false;
        goto SAFE_EXIT;
    }

    if (!m_bWLANExist)
    {
        bReturn = false;
        goto SAFE_EXIT;
    }

    // ��ȡ�����б�
    dwRet = WlanGetAvailableNetworkList(m_hWLAN, &m_guidWLAN, 
        WLAN_AVAILABLE_NETWORK_INCLUDE_ALL_ADHOC_PROFILES, NULL, &pNetworkList);
    if (dwRet != ERROR_SUCCESS)
    {
        m_errStream.str("");
        m_errStream << "WlanGetAvailableNetworkList FAIL, Error Code: " << dwRet;
        bReturn = false;
        goto SAFE_EXIT;
    }


    // ����ָ������
    for (unsigned int i = 0; i < pNetworkList->dwNumberOfItems; i++ )
    {
        if (ssid.compare((char*)(pNetworkList->Network[i].dot11Ssid.ucSSID)) == 0)
        {
            destNetworkIndex = (int)i;
            break;
        }
    }

    // û���ҵ�ָ��������
    if (destNetworkIndex == -1)
    {
        m_errStream.str("");
        m_errStream << "Can't find specific wifi";
        bReturn = false;
        goto SAFE_EXIT;
    }

    // ָ�������Ѿ�������
    if ((pNetworkList->Network[destNetworkIndex].dwFlags & WLAN_AVAILABLE_NETWORK_CONNECTED) ==
        WLAN_AVAILABLE_NETWORK_CONNECTED)
    {
        bReturn = true;
        goto SAFE_EXIT;
    }

    // �������������ļ�
    wifiProfile.SSID = ssid;
    wifiProfile.Key = key;
    wifiProfile.AuthAlgo = pNetworkList->Network[destNetworkIndex].dot11DefaultAuthAlgorithm;
    wifiProfile.CipherAlgo = pNetworkList->Network[destNetworkIndex].dot11DefaultCipherAlgorithm;
    this->CreateWifiProfile(wifiProfile, profileStr);

    // �������������ļ�
    dwRet = WlanSetProfile(m_hWLAN, &(m_guidWLAN), 0, profileStr.c_str(), NULL, TRUE, NULL, &reasonCode);
    if (dwRet != ERROR_SUCCESS)
    {
        m_errStream.str("");
        m_errStream << "WlanSetProfile FAIL, Error code: " << dwRet << "Reason code: " << reasonCode << std::endl;
        m_errStream << profileStr.c_str();

        bReturn = false;
        goto SAFE_EXIT;
    }

    connParam.wlanConnectionMode = wlan_connection_mode_profile;
    connParam.strProfile =pNetworkList->Network[destNetworkIndex].strProfileName;								
    connParam.pDot11Ssid = NULL;
    connParam.pDesiredBssidList = NULL;
    connParam.dwFlags = 0;
    connParam.dot11BssType = dot11_BSS_type_infrastructure;
    dwRet = WlanConnect(m_hWLAN, &(m_guidWLAN), &connParam, NULL);
    if (dwRet != ERROR_SUCCESS)
    {
        m_errStream.str("");
        m_errStream << "WlanConnect FAIL, Error code: " << dwRet;

        bReturn = false;
        goto SAFE_EXIT;
    }

    bReturn = true;


SAFE_EXIT:
    if (pNetworkList != NULL)
    {
        WlanFreeMemory(pNetworkList);
        pNetworkList = NULL;
    }
    return bReturn;
}

bool CWifiNetwork::WLANConnect(IN const string& ssid)
{
    bool bReturn = false;
    DWORD dwRet;
    int destNetworkIndex = -1;
    PWLAN_AVAILABLE_NETWORK_LIST pNetworkList = NULL;
    WLAN_CONNECTION_PARAMETERS connParam;

    if (!m_bInitSuccess)
    {
        bReturn = false;
        goto SAFE_EXIT;
    }

    if (!m_bWLANExist)
    {
        bReturn = false;
        goto SAFE_EXIT;
    }

    // ��ȡ�����б�
    dwRet = WlanGetAvailableNetworkList(m_hWLAN, &m_guidWLAN, 
        WLAN_AVAILABLE_NETWORK_INCLUDE_ALL_ADHOC_PROFILES, NULL, &pNetworkList);
    if (dwRet != ERROR_SUCCESS)
    {
        m_errStream.str("");
        m_errStream << "WlanGetAvailableNetworkList FAIL, Error Code: " << dwRet;
        bReturn = false;
        goto SAFE_EXIT;
    }


    // ����ָ������
    for (unsigned int i = 0; i < pNetworkList->dwNumberOfItems; i++ )
    {
        if (ssid.compare((char*)(pNetworkList->Network[i].dot11Ssid.ucSSID)) == 0)
        {
            destNetworkIndex = (int)i;
            break;
        }
    }

    // û���ҵ�ָ��������
    if (destNetworkIndex == -1)
    {
        m_errStream.str("");
        m_errStream << "Can't find specific wifi";
        bReturn = false;
        goto SAFE_EXIT;
    }

    // ָ�������Ѿ�������
    if ((pNetworkList->Network[destNetworkIndex].dwFlags & WLAN_AVAILABLE_NETWORK_CONNECTED) ==
        WLAN_AVAILABLE_NETWORK_CONNECTED)
    {
        bReturn = true;
        goto SAFE_EXIT;
    }

    connParam.wlanConnectionMode = wlan_connection_mode_profile;
    connParam.strProfile =pNetworkList->Network[destNetworkIndex].strProfileName;								
    connParam.pDot11Ssid = NULL;
    connParam.pDesiredBssidList = NULL;
    connParam.dwFlags = 0;
    connParam.dot11BssType = dot11_BSS_type_infrastructure;
    dwRet = WlanConnect(m_hWLAN, &(m_guidWLAN), &connParam, NULL);
    if (dwRet != ERROR_SUCCESS)
    {
        m_errStream.str("");
        m_errStream << "WlanConnect FAIL, Error code: " << dwRet;

        bReturn = false;
        goto SAFE_EXIT;
    }

    bReturn = true;


SAFE_EXIT:
    if (pNetworkList != NULL)
    {
        WlanFreeMemory(pNetworkList);
        pNetworkList = NULL;
    }
    return bReturn;
}

bool CWifiNetwork::GetAvailableNetworkList(OUT vector<SWifiNetwork>& networkist)
{
    bool bReturn = false;
    SWifiNetwork network;
    DWORD dwRet;
    PWLAN_AVAILABLE_NETWORK_LIST pNetworkList = NULL;

    networkist.clear();

    if (!m_bInitSuccess)
    {
        bReturn = false;
        goto SAFE_EXIT;
    }

    if (!m_bWLANExist)
    {
        bReturn = false;
        goto SAFE_EXIT;
    }

   dwRet = WlanGetAvailableNetworkList(m_hWLAN, &m_guidWLAN, 
       WLAN_AVAILABLE_NETWORK_INCLUDE_ALL_ADHOC_PROFILES, NULL, &pNetworkList);
   if (dwRet != ERROR_SUCCESS)
   {
       m_errStream.str("");
       m_errStream << "WlanGetAvailableNetworkList FAIL, Error Code: " << dwRet;
       bReturn = false;
       goto SAFE_EXIT;
   }

   bReturn = true;

   for (unsigned int i = 0; i < pNetworkList->dwNumberOfItems; i++ )
   {
       network.IsCurrentConnected = false;
       network.SSID = (char*)(pNetworkList->Network[i].dot11Ssid.ucSSID);
       if ((pNetworkList->Network[i].dwFlags & WLAN_AVAILABLE_NETWORK_CONNECTED) ==
           WLAN_AVAILABLE_NETWORK_CONNECTED)
       {
           network.IsCurrentConnected = true;
       }

       networkist.push_back(network);
   }


SAFE_EXIT:
    if (pNetworkList != NULL)
    {
        WlanFreeMemory(pNetworkList);
        pNetworkList = NULL;
    }
    return bReturn;
}

void CWifiNetwork::UnInit()
{
    if (m_hWLAN != NULL)
    {
        WlanCloseHandle(m_hWLAN, NULL);
        m_hWLAN = NULL;
    }
}


void CWifiNetwork::CreateWifiProfile(const SWifiProfile& wifiProfile, OUT wstring& profileStr)
{
    string authAlgo;
    switch (wifiProfile.AuthAlgo)
    {
    case DOT11_AUTH_ALGO_80211_OPEN:
    case DOT11_AUTH_ALGO_IHV_START:
    case DOT11_AUTH_ALGO_IHV_END:
        authAlgo = "open";
        break;
    case DOT11_AUTH_ALGO_80211_SHARED_KEY:
        authAlgo = "shared";
        break;
    case DOT11_AUTH_ALGO_WPA:
    case DOT11_AUTH_ALGO_WPA_NONE:
        authAlgo = "WPA";
        break;
    case DOT11_AUTH_ALGO_WPA_PSK:
        authAlgo = "WPAPSK";
        break;
    case DOT11_AUTH_ALGO_RSNA:
        authAlgo = "WPA2";
        break;
    case DOT11_AUTH_ALGO_RSNA_PSK:
        authAlgo = "WPA2PSK";
        break;
    default:
        authAlgo = "open";
        break;
    }

    string cipherAlgo;
    switch (wifiProfile.CipherAlgo)
    {
    case DOT11_CIPHER_ALGO_NONE:
    case DOT11_CIPHER_ALGO_IHV_START:
    case DOT11_CIPHER_ALGO_IHV_END:
        cipherAlgo = "none";
        break;
    case  DOT11_CIPHER_ALGO_WEP40:
    case DOT11_CIPHER_ALGO_WEP:
        cipherAlgo = "WEP";
        break;
    case DOT11_CIPHER_ALGO_CCMP:
    case DOT11_CIPHER_ALGO_WEP104:
    case DOT11_CIPHER_ALGO_WPA_USE_GROUP:
    //case DOT11_CIPHER_ALGO_RSN_USE_GROUP:
        cipherAlgo = "AES";
        break;
    case DOT11_CIPHER_ALGO_TKIP:
        cipherAlgo = "TKIP";
        break;
    default:
        cipherAlgo = "none";
        break;
    }

    string keyType;
    if (cipherAlgo.compare("WEP") == 0)
        keyType = "networkKey";
    else
        keyType = "passPhrase";


    const int bufferLen = 2048; 
    wchar_t profileBuffer[bufferLen] = {0};

    swprintf_s(profileBuffer, bufferLen,
                   L"<?xml version=\"1.0\" encoding=\"US-ASCII\"?> \
                      <WLANProfile xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v1\"> \
                      <name>%S</name> \
                      <SSIDConfig> \
                      <SSID> \
                      <name>%S</name> \
                      </SSID> \
                      </SSIDConfig> \
                      <connectionType>ESS</connectionType> \
                      <connectionMode>auto</connectionMode> \
                      <autoSwitch>false</autoSwitch> \
                      <MSM> \
                      <security> \
                      <authEncryption> \
                      <authentication>%S</authentication> \
                      <encryption>%S</encryption> \
                      <useOneX>false</useOneX> \
                      </authEncryption> \
                      <sharedKey> \
                      <keyType>%S</keyType> \
                      <protected>false</protected> \
                      <keyMaterial>%S</keyMaterial> \
                      </sharedKey> \
                      </security> \
                      </MSM> \
                      </WLANProfile>",
                      wifiProfile.SSID.c_str(),
                      wifiProfile.SSID.c_str(),
                      authAlgo.c_str(),
                      cipherAlgo.c_str(),
                      keyType.c_str(),
                      wifiProfile.Key.c_str());

    profileStr = profileBuffer;
}

LWifiNetwork::LWifiNetwork()
{
    m_pWifiNetwork = new CWifiNetwork();
}

LWifiNetwork::~LWifiNetwork()
{
    delete m_pWifiNetwork;
    m_pWifiNetwork = 0;
}

bool LWifiNetwork::IsWLANExist()
{
    return m_pWifiNetwork->IsWLANExist();
}

bool LWifiNetwork::GetWLANDesc(OUT wstring& desc)
{
    return m_pWifiNetwork->GetWLANDesc(desc);
}

bool LWifiNetwork::GetWLANGUID(OUT wstring& strGUID)
{
    GUID guid;
    bool bRet = m_pWifiNetwork->GetWLANGUID(guid);
    if (bRet)
    {
        strGUID = L"{";
        RPC_WSTR pStrGUID = 0;
        UuidToStringW(&guid, &pStrGUID);
        strGUID += (wchar_t*)pStrGUID;
        RpcStringFreeW(&pStrGUID);
        pStrGUID = 0;
        strGUID += L"}";
    }

    return bRet;
}

