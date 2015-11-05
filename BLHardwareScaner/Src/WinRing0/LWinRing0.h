

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

    void DeinitializeWinRing0();
};

#endif