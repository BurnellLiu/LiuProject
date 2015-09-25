

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

/// @brief LOG��
class LLog
{
public:
    /// @brief ��LOG��, ����������򴴽�, ����Ѿ�������ôԭʼ���ݽ�������
    /// @return �ɹ�����true, ʧ�ܷ���false
    static bool Open(IN const char* szFileName);

    /// @brief �ر�LOG��
    static void Close();

    /// @brief дһ��LOG, �Զ�����
    /// @param[in] szFormart ��Ҫд��LOG�ĸ�ʽ���ַ���
    static void WriteLine(IN char* szFormat, ...);

    static void WriteLineW(IN wchar_t* szFormat, ...);
};

#endif