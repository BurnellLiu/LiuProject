

#include "LSetupAPI.h"

#include <cassert>

#include <cfgmgr32.h>


#pragma comment(lib, "Cfgmgr32.lib")
#pragma comment(lib, "Setupapi.lib")


LSADevObject::LSADevObject()
{
	m_hDevInfoSet = NULL;
	m_pDevInfoList = NULL;
	m_devCount = 0;
};

LSADevObject::~LSADevObject()
{
	BaseCleanUp();
}

int LSADevObject::GetDevNum()
{
	return m_devCount;
}

bool LSADevObject::Enable(IN int index)
{
	bool bNeedRestart = false;
	return this->ChangeState(index, DICS_ENABLE, bNeedRestart);
}

bool LSADevObject::Disable(IN int index, OUT bool& bNeedRestart)
{
	return this->ChangeState(index, DICS_DISABLE, bNeedRestart);
}

bool LSADevObject::GetDevDesc(IN int index, OUT string& devDesc)
{
	return this->GetRegistryPropertyStr(index, SPDRP_DEVICEDESC, devDesc);
}

bool LSADevObject::IsDisabled(IN int index)
{
	if (index >= m_devCount || index < 0)
		return false;

	ULONG pulStatus;
	ULONG pulProblemNum;
	CONFIGRET configRet =  CM_Get_DevNode_Status(&pulStatus, &pulProblemNum, m_pDevInfoList[index].DevInst, 0);
	if (configRet != CR_SUCCESS)
		return false;

	return ((pulStatus&DN_HAS_PROBLEM) && (pulProblemNum == CM_PROB_DISABLED));
}

bool LSADevObject::IsDisableEnable(IN int index)
{
	if (index >= m_devCount || index < 0)
		return false;

	ULONG pulStatus;
	ULONG pulProblemNum;
	CONFIGRET configRet =  CM_Get_DevNode_Status(&pulStatus, &pulProblemNum, m_pDevInfoList[index].DevInst, 0);
	if (configRet != CR_SUCCESS)
		return false;

	return ((pulStatus&DN_DISABLEABLE) && (pulProblemNum != CM_PROB_HARDWARE_DISABLED));
}

void LSADevObject::BaseCleanUp()
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

bool LSADevObject::BaseInit(IN const GUID* pGUID)
{
	BaseCleanUp();

	if (pGUID == NULL)
		return false;

	m_hDevInfoSet = SetupDiGetClassDevsA(pGUID, NULL, NULL, DIGCF_PRESENT);
	if (m_hDevInfoSet == INVALID_HANDLE_VALUE)
	{
		m_hDevInfoSet = NULL;
		return false;
	}

	// 获取设备数目
	SP_DEVINFO_DATA devInfoData;
	devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	for (int i = 0; SetupDiEnumDeviceInfo(m_hDevInfoSet, i, &devInfoData); i++)
	{
		m_devCount = i + 1;
	}

	if (m_devCount == 0)
		return true;
	
	// 获取设备信息
	m_pDevInfoList = new SP_DEVINFO_DATA[m_devCount];
	assert(m_pDevInfoList != NULL);

	for (int i = 0; i < m_devCount; i++)
	{
		m_pDevInfoList[i].cbSize =  sizeof(SP_DEVINFO_DATA);
		SetupDiEnumDeviceInfo(m_hDevInfoSet, i, &m_pDevInfoList[i]);
	}

	return true;
}

bool LSADevObject::GetRegistryPropertyStr(IN int index, IN DWORD dwProperty, OUT string& strProperty)
{
	// 索引超出范围
	if (index >= m_devCount || index < 0)
		return false;

	DWORD bufferSize = 0;
	LPSTR pBuffer = NULL;
	DWORD dataType;

	bool bSuccess = false;
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
			bSuccess = true;
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
				break;
			}
		}
	}

	if (bSuccess)
	{
		strProperty.clear();
		strProperty.append(pBuffer);
	}
	
	if (pBuffer)
	{
		LocalFree(pBuffer);
		pBuffer = NULL;
	}

	return bSuccess;
}

bool LSADevObject::ChangeState(IN int index, IN DWORD newSate, OUT bool& bNeedRestart)
{
	SP_PROPCHANGE_PARAMS propChangeParam;
	propChangeParam.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
	propChangeParam.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
	propChangeParam.Scope = DICS_FLAG_GLOBAL;
	propChangeParam.StateChange = newSate;
	propChangeParam.HwProfile = 0;
	
	BOOL bRet = SetupDiSetClassInstallParamsA(m_hDevInfoSet, &m_pDevInfoList[index], 
		&propChangeParam.ClassInstallHeader, sizeof(propChangeParam));
	if (!bRet) return false;

	bRet = SetupDiChangeState(m_hDevInfoSet, &m_pDevInfoList[index]);
	if (bRet) 
	{
		bNeedRestart = false;
		return true;
	}

	// 检查是否需要重启
	ULONG pulStatus;
	ULONG pulProblemNum;
	CONFIGRET configRet =  CM_Get_DevNode_Status(&pulStatus, &pulProblemNum, m_pDevInfoList[index].DevInst, 0);
	if (configRet != CR_SUCCESS)
		return false;

	if((pulStatus&DN_NEED_RESTART) > 0)
	{
		bNeedRestart = true;
		return true;
	}

	return false;
}

LSAHumanInputDev::LSAHumanInputDev()
{

}

LSAHumanInputDev::~LSAHumanInputDev()
{

}

bool LSAHumanInputDev::Init()
{
	return BaseInit(&GUID_DEVCLASS_HIDCLASS);
}

LSAMouseDev::LSAMouseDev()
{

}

LSAMouseDev::~LSAMouseDev()
{

}

bool LSAMouseDev::Init()
{
	return BaseInit(&GUID_DEVCLASS_MOUSE);
}

LSABluetoothDev::LSABluetoothDev()
{

}

LSABluetoothDev::~LSABluetoothDev()
{

}

bool LSABluetoothDev::Init()
{
	return BaseInit(&GUID_DEVCLASS_BLUETOOTH);
}


