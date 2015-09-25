

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
using std::wstring;
using std::string;

/// @brief LOG��
class LLog
{
public:
    /// @brief ��LOG��, ����������򴴽�, ����Ѿ�������ôԭʼ���ݽ�������
    /// @param[in] strFileName LOG���ļ���
    /// @return �ɹ�����true, ʧ�ܷ���false
    static bool Open(IN const string& strFileName);

    /// @brief �ر�LOG��
    static void Close();

    /// @brief дһ��LOG, �Զ�����
    /// @param[in] strFormart ��Ҫд��LOG�ĸ�ʽ���ַ���
    static void WriteLine(IN const string& strFormat, ...);

    /// @brief дһ��LOG, �Զ�����
    /// @param[in] strFormat ��Ҫд��LOG�ĸ�ʽ���ַ���
    static void WriteLineW(IN const wstring& strFormat, ...);
};

#endif