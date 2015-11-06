

#ifndef _LWINRING0_H_
#define _LWINRING0_H_

#include <string>
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

namespace LWinRing0
{
    /// @brief 初始化WinRing0
    /// @param[in] filePath DLL文件路径
    /// @return 成功返回true, 失败返回false
    bool InitializeWinRing0(IN const wstring& filePath);

    /// @brief 释放WinRing0
    void DeinitializeWinRing0();

    /// @brief CPUID指令
    /// @param[in] index CPUID指令索引
    /// @param[out] eax
    /// @param[out] ebx
    /// @param[out] ecx
    /// @param[out] edx
    /// @return 成功返回1, 失败返回0
    int Cpuid(
        IN unsigned long index, 
        OUT unsigned long* eax, 
        OUT unsigned long* ebx, 
        OUT unsigned long* ecx, 
        OUT unsigned long* edx);

    /// @brief 
    /// @param[in] index
    /// @param[out] eax
    /// @param[out] edx
    /// @return 成功返回1, 失败返回0
    int Rdmsr(IN unsigned long index, OUT unsigned long* eax, OUT unsigned long* edx);
};

#endif