#include "LEC.h"

#include "Windows/LAsusEC_SuperIO/OlsApi.h"
#include "Windows/LAsusEC_SuperIO/OlsDef.h"

#pragma comment(lib, "WinRing0.lib")

bool LWinRing0::sm_bInitSuccess = false;
LWinRing0 g_winRing0; //全局初始化WinRing0

LWinRing0::LWinRing0()
{
    BOOL bRet = InitializeOls();
    if (bRet == FALSE)
        return;
    bRet = GetDllStatus();
    if (bRet != OLS_DLL_NO_ERROR)
        return;

    sm_bInitSuccess = true;
}

LWinRing0::~LWinRing0()
{
    if (sm_bInitSuccess)
        DeinitializeOls();
}



LEc::LEc()
{
  
}

LEc::~LEc()
{

}

bool LEc::IBFReady()
{
    for (int i = 0; i < 300; i++)
    {
        BYTE state = ReadCommandPortByte();

        // bit1: IBF, 为0时表示input buffer为空, 可以写入
        if ((state & 0x02) == 0)
            return true;

        Sleep(5);
    }

    return false;
}

bool LEc::OBFReady()
{
    for (int i = 0; i < 300; i++)
    {
        BYTE state = ReadCommandPortByte();

        // bit0: OBF, 为1时表示data已经准备好, 可以从output buffer中读出数据
        if ((state & 0x01) == 1)
            return true;

        Sleep(5);
    }

    return false;
}

BYTE LEc::ReadCommandPortByte()
{
    return ReadIoPortByte(EC_SC);
}

void LEc::WriteCommandPortByte(BYTE command)
{
    WriteIoPortByte(EC_SC, command);
}

BYTE LEc::ReadDataPortByte()
{
    return ReadIoPortByte(EC_DATA);
}

void LEc::WriteDataPortByte(BYTE data)
{
    WriteIoPortByte(EC_DATA, data);
}

LAsusIteEc::LAsusIteEc()
{
    EC_SC = 0x25D;
    EC_DATA = 0x25C;
}

LAsusIteEc::~LAsusIteEc()
{

}

LAsusEneEc::LAsusEneEc()
{
    EC_SC = 0x66;
    EC_DATA = 0x62;
}

LAsusEneEc::~LAsusEneEc()
{

}

