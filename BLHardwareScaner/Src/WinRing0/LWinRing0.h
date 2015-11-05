

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
    /// @brief ��ʼ��WinRing0
    /// @param[in] filePath DLL�ļ�·��
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool InitializeWinRing0(IN const wstring& filePath);

    void DeinitializeWinRing0();
};

#endif