
#include "LTaskScheduler.h"

#include <Windows.h>
#include <Taskschd.h>
#include <comdef.h>


#pragma comment(lib, "Taskschd.lib")

/// @brief COM��ʼ����
/// ֻ֧�ֵ��߳�
class CInitCom
{
public:
    CInitCom()
    {
        this->bSuccess = false;

        // CoInitialize���ú󷵻�S_FALSE��S_OK�������CoUninitialize
        // CoInitialize���ú󷵻�RPC_E_CHANGE_MODE, ������ǰ�߳��ѱ���ʼ��(�Һ͵�ǰģʽ��ͬ),����Ҫ����CoUninitialize
        HRESULT hr = CoInitialize(NULL);
        if (hr == S_OK || hr == S_FALSE)
        {
            this->bSuccess = true;
        }
        else
        {
            this->bSuccess = false;
        }
    }

    ~CInitCom()
    {
        if (this->bSuccess)
        {
            CoUninitialize();
        }
    }
private:
    bool bSuccess; ///< ��ʶ�Ƿ��ʼ���ɹ�
};

/// @brief ����ƻ���
class CTaskScheduler
{
public:
    /// @brief ɾ��һ������ƻ�
    /// @param[in] pFolderName ����ƻ����ڵ��ļ�������, ���ΪNULL, ���ʾ����ƻ��ڸ�Ŀ¼��
    /// @param[in] pTaskName ����ƻ�����
    /// @return �ɹ�����true, ʧ�ܷ���false
    static bool Delete(IN const wchar_t* pFolderName, const wchar_t* pTaskName)
    {
        bool bRet = true;
        CInitCom initCom;
        ITaskService* pTaskService = NULL;
        ITaskFolder* pRootFolder = NULL;
        ITaskFolder* pMyFolder = NULL;

        if (NULL == pTaskName)
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        HRESULT hr = CoCreateInstance( 
            CLSID_TaskScheduler,
            NULL,
            CLSCTX_INPROC_SERVER,
            IID_ITaskService,
            (void**)&pTaskService);
        if (FAILED(hr))
        {
            bRet = false;
            goto SAFE_EXIT;
        }


        hr = pTaskService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
        if (FAILED(hr))
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        // ��ȡ���ļ���
        hr = pTaskService->GetFolder( _bstr_t( L"\\"), &pRootFolder);
        if (FAILED(hr))
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        if (NULL != pFolderName)
        {
            hr = pRootFolder->GetFolder(_bstr_t(pFolderName), &pMyFolder);
            if(FAILED(hr))
            {
                bRet = false;
                goto SAFE_EXIT;
            }
        }

        if (NULL != pMyFolder)
        {
            hr = pMyFolder->DeleteTask(_bstr_t(pTaskName), 0); 
        }
        else
        {
            hr = pRootFolder->DeleteTask(_bstr_t(pTaskName), 0);
        }

        if(FAILED(hr))
        {
            bRet = false;
            goto SAFE_EXIT;
        }


        bRet = true;

SAFE_EXIT:
        if (NULL != pTaskService)
        {
            pTaskService->Release();
            pTaskService = NULL;
        }

        if (NULL != pRootFolder)
        {
            pRootFolder->Release();
            pRootFolder = NULL;
        }
        if (NULL != pMyFolder)
        {
            pMyFolder->Release();
            pMyFolder = NULL;
        }

        return bRet;
    }

public:
    /// @brief ���캯��
    CTaskScheduler()
    {
        m_pRootTaskFolder = NULL;
        m_pTaskDef = NULL;

        ITaskService* pTaskService = NULL;
        

        HRESULT hr = CoCreateInstance( 
            CLSID_TaskScheduler,
            NULL,
            CLSCTX_INPROC_SERVER,
            IID_ITaskService,
            (void**)&pTaskService);
        if (FAILED(hr))
        {
            goto SAFE_EXIT;
        }


        hr = pTaskService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
        if (FAILED(hr))
        {
            goto SAFE_EXIT;
        }

        // ��ȡ���ļ���
        hr = pTaskService->GetFolder( _bstr_t( L"\\"), &m_pRootTaskFolder);
        if (FAILED(hr))
        {
            goto SAFE_EXIT;
        }

        // ��������
        hr = pTaskService->NewTask(0, &m_pTaskDef);
        if (FAILED(hr))
        {
            goto SAFE_EXIT;
        }

        return;

SAFE_EXIT:
        if (NULL != pTaskService)
        {
            pTaskService->Release();
            pTaskService = NULL;
        }

    }

    /// @brief ��������
    ~CTaskScheduler()
    {
        if (NULL != m_pTaskDef)
        {
            m_pTaskDef->Release();
            m_pTaskDef = NULL;
        }

        if (NULL != m_pRootTaskFolder)
        {
            m_pRootTaskFolder->Release();
            m_pRootTaskFolder = NULL;
        }
    }

    /// @brief ����������Ҫ����
    bool Principal(IN bool bHighestLevel)
    {
        if (NULL == m_pTaskDef)
        {
            return false;
        }


        bool bRet = true;
        IPrincipal* pPrincipal = NULL;

        HRESULT hr = m_pTaskDef->get_Principal(&pPrincipal);
        if (FAILED(hr))
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        if (bHighestLevel)
            hr = pPrincipal->put_RunLevel(TASK_RUNLEVEL_HIGHEST);
        else
            hr = pPrincipal->put_RunLevel(TASK_RUNLEVEL_LUA);

        if (FAILED(hr))
        {
            bRet = false;
            goto SAFE_EXIT;
        }

SAFE_EXIT:

        if (NULL != pPrincipal)
        {
            pPrincipal->Release();
            pPrincipal = NULL;
        }

        return bRet;
    }

    /// @brief ���ô�������
    /// @param[in] trigger, 1(����ʱ����), 2(��¼ʱ����)
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool Trigger(IN unsigned int trigger)
    {
        if (trigger != 1 && trigger != 2)
            return false;

        if (NULL == m_pTaskDef)
            return false;

        bool bRet = true;

        ITriggerCollection* pTriggerCollection = NULL;
        ITrigger* pTrigger = NULL;
        //�õ�����������    
        HRESULT hr = m_pTaskDef->get_Triggers(&pTriggerCollection);
        if(FAILED(hr))
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        //�ڴ����������д���������
        if (1 == trigger)
            hr = pTriggerCollection->Create(TASK_TRIGGER_BOOT, &pTrigger); // ϵͳ����ʱ����
        else if (2 == trigger)
            hr = pTriggerCollection->Create(TASK_TRIGGER_LOGON, &pTrigger); // ���û���¼ʱ����

        if(FAILED(hr))
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        bRet = true;

SAFE_EXIT:
        if (NULL != pTriggerCollection)
        {
            pTriggerCollection->Release();
            pTriggerCollection = NULL;
        }
        if (NULL != pTrigger)
        {
            pTrigger->Release();
            pTrigger = NULL;
        }

        return bRet;
    }

    /// @brief ����������
    bool Settings(IN bool bStartOnBattery)
    {
        if (NULL == m_pTaskDef)
            return false;

        bool bRet = true;
        ITaskSettings* pSettings = NULL;
        HRESULT hr = m_pTaskDef->get_Settings(&pSettings);
        if(FAILED(hr))
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        hr = pSettings->put_DisallowStartIfOnBatteries(VARIANT_BOOL(!bStartOnBattery));
        if(FAILED(hr))
        {
            bRet = false;
            goto SAFE_EXIT;
        }


        bRet = true;

SAFE_EXIT:
        if (NULL != pSettings)
        {
            pSettings->Release();
            pSettings = NULL;
        }

        return bRet;
    }

    /// @brief ��������ƻ�����
    bool Action(IN const wchar_t* pExePath, IN const wchar_t* pParam, IN const wchar_t* pWorkingDir)
    {
        if (NULL == pExePath)
            return false;
        if (NULL == m_pTaskDef)
            return false;

        bool bRet = true;

        IActionCollection* pActionCollection = NULL;
        IAction* pAction = NULL;
        IExecAction* pExecAction = NULL;

        //�õ���������    
        HRESULT hr = m_pTaskDef->get_Actions(&pActionCollection);
        if(FAILED(hr))
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        //�ڶ��������д�������    
        hr = pActionCollection->Create(TASK_ACTION_EXEC, &pAction);
        if(FAILED(hr))
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        //��������д��ִ�г���
        hr = pAction->QueryInterface( IID_IExecAction, (void**)&pExecAction );
        if(FAILED(hr))
        {
            bRet = false;
            goto SAFE_EXIT;
        }
        hr = pExecAction->put_Path(_bstr_t(pExePath)); //���б�����
        if(FAILED(hr))
        {
            bRet = false;
            goto SAFE_EXIT;
        }
        if (NULL != pParam)
        {
            hr = pExecAction->put_Arguments(_bstr_t(pParam));
            if(FAILED(hr))
            {
                bRet = false;
                goto SAFE_EXIT;
            }
        }
        if (NULL != pWorkingDir)
        {
            hr = pExecAction->put_WorkingDirectory(_bstr_t(pWorkingDir));
            if (FAILED(hr))
            {
                bRet = false;
                goto SAFE_EXIT;
            }
        }
        

        bRet = true;

SAFE_EXIT:
        if (NULL != pExecAction)
        {
            pExecAction->Release();
            pExecAction = NULL;
        }
        if (NULL != pActionCollection)
        {
            pActionCollection->Release();
            pActionCollection = NULL;
        }
        if (NULL != pAction)
        {
            pAction->Release();
            pAction = NULL;
        }

        return bRet;

    }

    /// @brief ʹ������ƻ����ƴ����µ�����ƻ�
    bool Register(IN const wchar_t* pFolderName, IN const wchar_t* pTaskName)
    {
        if (NULL == pTaskName)
            return false;
        if (NULL == m_pTaskDef)
            return false;
        if (NULL == m_pRootTaskFolder)
            return false;

        bool bRet = true;
        HRESULT hr = S_OK;
        ITaskFolder* pNewFolder = NULL;
        IRegisteredTask* pRegisteredTask = NULL;
        ITaskFolder* pMyFolder = NULL;

        if (NULL != pFolderName)
        {
            hr = m_pRootTaskFolder->GetFolder(_bstr_t(pFolderName), &pNewFolder);
            if(FAILED(hr)) //����ļ��в�����,�ʹ���һ��
            {
                hr = m_pRootTaskFolder->CreateFolder(_bstr_t(pFolderName), _variant_t(), &pNewFolder);
                if(FAILED(hr))
                {
                    bRet = false;
                    goto SAFE_EXIT;
                }
            }
        }
        
        // ���ָ�����ļ���, ����ָ�����ļ����´�������ƻ�, �����ڸ�Ŀ¼�´�������ƻ�
        if (NULL != pNewFolder)
        {
            pMyFolder = pNewFolder;
        }
        else
        {
            pMyFolder = m_pRootTaskFolder;
        }
        
        hr = pMyFolder->RegisterTaskDefinition(
            _bstr_t(pTaskName),
            m_pTaskDef,
            TASK_CREATE_OR_UPDATE,  
            _variant_t(),
            _variant_t(),
            TASK_LOGON_INTERACTIVE_TOKEN,
            _variant_t(L""),
            &pRegisteredTask);
        if (FAILED(hr))
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        bRet = true;

SAFE_EXIT:
        if (NULL != pNewFolder)
        {
            pNewFolder->Release();
            pNewFolder = NULL;
        }
        if (NULL != pRegisteredTask)
        {
            pRegisteredTask->Release();
            pRegisteredTask = NULL;
        }

        return bRet;

    }

private:

    ITaskDefinition* m_pTaskDef; ///< ����ƻ��������
    ITaskFolder* m_pRootTaskFolder; ///< ����ƻ����ļ��ж���
    CInitCom m_initCom; ///< ��ʼ��COM����
};

bool LTaskScheduler::Delete(IN const wchar_t* pFolderName, const wchar_t* pTaskName)
{
    return CTaskScheduler::Delete(pFolderName, pTaskName);
}

LTaskScheduler::LTaskScheduler()
{
    m_pTaskScheduler = new CTaskScheduler();
}

LTaskScheduler::~LTaskScheduler()
{
    if (NULL != m_pTaskScheduler)
    {
        delete m_pTaskScheduler;
    }
}

bool LTaskScheduler::Principal(IN bool bHighestLevel)
{
    if (NULL == m_pTaskScheduler)
        return false;

    return m_pTaskScheduler->Principal(bHighestLevel);
}

bool LTaskScheduler::Trigger(IN unsigned int trigger)
{
    if (NULL == m_pTaskScheduler)
        return false;

    return m_pTaskScheduler->Trigger(trigger);
}

bool LTaskScheduler::Settings(IN bool bStartOnBattery)
{
    if (NULL == m_pTaskScheduler)
        return false;

    return m_pTaskScheduler->Settings(bStartOnBattery);
}

bool LTaskScheduler::Action(IN const wchar_t* pExePath, IN const wchar_t* pParam, IN const wchar_t* pWorkingDir)
{
    if (NULL == m_pTaskScheduler)
        return false;

    return m_pTaskScheduler->Action(pExePath, pParam, pWorkingDir);
}

bool LTaskScheduler::Register(IN const wchar_t* pFolderName, IN const wchar_t* pTaskName)
{
    if (NULL == m_pTaskScheduler)
        return false;

    return m_pTaskScheduler->Register(pFolderName, pTaskName);
}