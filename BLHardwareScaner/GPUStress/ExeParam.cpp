
#include "ExeParam.h"

#include <string>
using std::string;

#include <Windows.h>

#include "Mandelbrot.h"

#define DEVICE_INI_FILE L".\\ExeParam.ini"


/// <SUMMARY>
/// 字符串转换为宽字符串
/// </SUMMARY>
/// <PARAM name = " strSrc" dir = "IN">
/// 源字符串
/// </PARAM>
/// <PARAM name = " strDest" dir = "OUT">
/// 存储转换后的宽字符串
/// </PARAM>
/// <RETURNS>
/// 成功返回0, 失败返回false
/// </RETURNS>
static bool StringToWString(const string& strSrc, wstring& strDest)
{
    int nSize = MultiByteToWideChar(CP_ACP, 0, strSrc.c_str(), strSrc.length(), 0, 0);
    if (nSize <= 0)
        return false;
    wchar_t* pwszDst = new wchar_t[nSize + 1];

    int iRet = MultiByteToWideChar(CP_ACP, 0, strSrc.c_str(), strSrc.length(), pwszDst, nSize);

    pwszDst[nSize] = 0;

    strDest.clear();
    strDest.assign(pwszDst);

    delete[] pwszDst;

    return true;
}

void GenerateDefaultParamFile()
{
    DeleteFileW(DEVICE_INI_FILE);

    // 配置文件中写入运行时间, 默认为10分钟
    WritePrivateProfileStringW(L"ExeParam", L"Time", L"10", DEVICE_INI_FILE);

    WritePrivateProfileStringW(L"ExeParam", L"Device", L"-1", DEVICE_INI_FILE);

    vector<AccDevice> accDevicesVec;
    GetAccelerators(accDevicesVec);

    wchar_t strBuffer[256] = { 0 };
    swprintf_s(strBuffer, L"%u", accDevicesVec.size());
    WritePrivateProfileStringW(L"Device", L"Count", strBuffer, DEVICE_INI_FILE);

    for (unsigned int i = 0; i < accDevicesVec.size(); i++)
    {
        swprintf_s(strBuffer, L"%u", i);
        wstring appName = strBuffer;
        WritePrivateProfileStringW(appName.c_str(), L"DeviceDesc", accDevicesVec[i].DeviceDesc.c_str(), DEVICE_INI_FILE);
        WritePrivateProfileStringW(appName.c_str(), L"DevicePath", accDevicesVec[i].DevicePath.c_str(), DEVICE_INI_FILE);
        WritePrivateProfileStringW(appName.c_str(), L"IsEmulated", accDevicesVec[i].IsEmulated ? L"True" : L"False", DEVICE_INI_FILE);
        WritePrivateProfileStringW(appName.c_str(), L"SupportDouble", accDevicesVec[i].SupportDouble ? L"True" : L"False", DEVICE_INI_FILE);
    }
}

/*
else
{
// 命令行为其他时, 尝试在配置文件中查询指定节点,
// 如果找不到, 则退出
wstring cmdLineW;
StringToWString(cmdLine, cmdLineW);
wchar_t devPath[256] = { 0 };
DWORD num = GetPrivateProfileStringW(
cmdLineW.c_str(),
L"DevicePath",
L"",
devPath,
256,
DEVICE_INI_FILE);
if (0 == num)
return 0;

SetDefaultAccelerator(devPath);
}
*/