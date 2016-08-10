

#ifndef _LTASKSCHEDULER_H_
#define _LTASKSCHEDULER_H_


#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif


class CTaskScheduler;

/// @brief ����ƻ���
/// ���ú�����ƻ��������Ժ����Register����������ϵͳ��ע��ƻ�����
class LTaskScheduler
{
public:
    /// @brief ɾ��һ������ƻ�
    /// @param[in] pFolderName ����ƻ����ڵ��ļ�������, ���ΪNULL, ���ʾ����ƻ��ڸ�Ŀ¼��
    /// @param[in] pTaskName ����ƻ�����
    /// @return �ɹ�����true, ʧ�ܷ���false
    static bool Delete(IN const wchar_t* pFolderName, const wchar_t* pTaskName);

public:
    /// @brief ���캯��
    LTaskScheduler();

    /// @brief ��������
    ~LTaskScheduler();

    /// @brief ����������Ҫ����
    /// �÷���Ϊ��ѡ��, �������Ϊ���Ȩ��, ����Ҫ����ԱȨ��, ��Ȼע������ƻ���ʧ��
    /// @param[in] bHighestLevel ��־�Ƿ�Ϊ���Ȩ������, Ĭ�ϲ������Ȩ��
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool Principal(IN bool bHighestLevel);

    /// @brief ���ô�������
    /// �÷���Ϊ��ѡ��
    /// @param[in] trigger, 1(����ʱ����), 2(��¼ʱ����), û��Ĭ��ֵ
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool Trigger(IN unsigned int trigger);

    /// @brief ����������
    /// �÷���Ϊ��ѡ��
    /// @param[in] bStartOnBattery ��־�Ƿ������DCģʽ������, Ĭ��ֵΪ������
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool Settings(IN bool bStartOnBattery);

    /// @brief ��������ƻ�����
    /// �÷���Ϊ����������
    /// @param[in] pExePath ����·��, ������ΪNULL
    /// @param[in] pParam ������������, ����ΪNULL
    /// @param[in] pWorkingDir ������Ŀ¼, ����ΪNULL, ΪNULL��ʾʹ��ϵͳĿ¼��Ϊ����Ŀ¼, ��:"C:\\Windows\\System32"
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool Action(IN const wchar_t* pExePath, IN const wchar_t* pParam, IN const wchar_t* pWorkingDir);

    /// @brief ������ƻ�ע�ᵽϵͳ��
    /// �÷���Ϊ����������
    /// ���ϵͳ���Ѿ�����ͬ���ļƻ�����, ���ʹ���µ�����������
    /// @param[in] pFolderName �ļ�������, ��������ڸ��ļ���, �򴴽��ļ���, ��: "Test\\MyFolder", �����ֵΪNULL, ���ڸ�Ŀ¼�´�������ƻ�
    /// @param[in] pTaskName ����ƻ�����, ������ΪNULL
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool Register(IN const wchar_t* pFolderName, const wchar_t* pTaskName);

private:
    CTaskScheduler* m_pTaskScheduler; ///< ����ƻ�ʵ�ֶ���
};

#endif