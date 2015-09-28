

#ifndef _LLOG_H_
#define _LLOG_H_

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

#include <string>
using std::string;
using std::wstring;

/// @brief LOG��
class LLog
{
public:
    /// @brief ��LOG��, ����������򴴽�, ����Ѿ�������ôԭʼ���ݽ�������
    /// @param[in] szFileName LOG���ļ���
    /// @return �ɹ�����true, ʧ�ܷ���false
    static bool Open(IN const char* szFileName);

    /// @brief �ر�LOG��
    static void Close();

    /// @brief дһ��LOG, �Զ�����, û�����1024���ַ�
    /// @param[in] szFormat ��Ҫд��LOG�ĸ�ʽ���ַ���
    static void WriteLineW(IN const wchar_t* szFormat, ...);

    /// @brief дһ��LOG, �Զ�����, û�����1024���ַ�
    /// @param[in] szFormat ��Ҫд��LOG�ĸ�ʽ���ַ���
    static void WriteLineA(IN const char* szFormat, ...);
};

/// @brief ��LOG�ļ�
#ifndef OpenLog
#define OpenLog(name) LLog::Open(name)
#endif

/// @brief �ر�LOG�ļ�
#ifndef CloseLog
#define CloseLog() LLog::Close()
#endif

/// @brief ��ӡLOG��
#ifndef PrintLogA
#define PrintLogA(format, ...) LLog::WriteLineA(format, __VA_ARGS__)
#endif

/// @brief ��ӡLOG��
#ifndef PrintLogW
#define PrintLogW(format, ...) LLog::WriteLineW(format, __VA_ARGS__)
#endif

#endif