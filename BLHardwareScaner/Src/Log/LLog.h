

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

    /// @brief ÿ��LOGǰ��ʾ�߳�Id
    /// Ĭ�ϲ������߳�Id
    /// @param[in] bFlag ��ʶ�Ƿ���ÿ��LOGǰ��ʾ�߳�Id
    static void ShowThreadId(IN bool bFlag);

    /// @brief ÿ��LOGǰ��ʾ��ǰʱ��
    /// Ĭ�ϲ����ӵ�ǰʱ��
    /// @param[in] bFlag ��ʶ�Ƿ���ÿ��LOGǰ��ʾ��ǰʱ��
    static void ShowTime(IN bool bFlag);

    /// @brief �����Ƿ��ӳ�д(д����)
    /// Ĭ��Ϊ�ӳ�д
    /// �ӳ�д�ڳ������ʱ����LOGûд���ļ������
    /// ֱ��д���ܲ���
    /// @param[in] bFlag ��ʶ�Ƿ��ӳ�д, true(�ӳ�д), false(ֱ��д)
    static void SetDelayedWrite(IN bool bFlag);

    /// @brief дһ��LOG, �Զ�����, û�����1024���ַ�
    /// @param[in] szFormat ��Ҫд��LOG�ĸ�ʽ���ַ���
    static void WriteLineW(IN const wchar_t* szFormat, ...);

    /// @brief дһ��LOG, �Զ�����, û�����1024���ַ�
    /// @param[in] szFormat ��Ҫд��LOG�ĸ�ʽ���ַ���
    static void WriteLineA(IN const char* szFormat, ...);
};

/// @brief ��ӡLOG��
#ifndef PrintLogA
#define PrintLogA(format, ...) LLog::WriteLineA(format, __VA_ARGS__)
#endif

/// @brief ��ӡLOG��
#ifndef PrintLogW
#define PrintLogW(format, ...) LLog::WriteLineW(format, __VA_ARGS__)
#endif

#endif