
#include "LAppInstance.h"

#include <Windows.h>

/// @brief Ӧ�ó���ʵ����
class CAppInstance
{
public:
    /// @brief ���캯��
    explicit CAppInstance(IN const wstring& name)
    {
        m_hMutex = NULL;
        m_bIsAlreadyExist = false;

        m_hMutex = CreateMutexW(NULL, FALSE, name.c_str());
        if (NULL == m_hMutex)
            return;

        if (ERROR_ALREADY_EXISTS == GetLastError())
        {
            m_bIsAlreadyExist = true;
        }
    }

    /// @brief ��������
    ~CAppInstance()
    {
        if (NULL != m_hMutex)
        {
            CloseHandle(m_hMutex);
            m_hMutex = NULL;
        }
    }

    /// @brief �ж��Ƿ��Ѿ����ڸ�ʵ�� 
    bool IsAlreadyExist()
    {
        return m_bIsAlreadyExist;
    }

private:
    bool m_bIsAlreadyExist; ///< ��־�Ƿ��Ѿ�����
    HANDLE m_hMutex; ///< ��������
};

LAppInstance::LAppInstance(IN const wstring& name)
{
    m_pAppInstance = new CAppInstance(name);
}

LAppInstance::~LAppInstance()
{
    if (NULL != m_pAppInstance)
    {
        delete m_pAppInstance;
    }
}

bool LAppInstance::IsAlreadyExist()
{
    if (NULL == m_pAppInstance)
        return false;

    return m_pAppInstance->IsAlreadyExist();
}