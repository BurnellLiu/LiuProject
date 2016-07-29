
#include "LPdh.h"

#include <Pdh.h>


#pragma comment(lib, "Pdh.lib")


/// @brief PDH�ӿ���
class CPdh
{
public:
    /// @brief ���캯��
    /// @param[in] counterPath �ռ���·��
    explicit CPdh(IN const wstring& counterPath)
    {
        m_hQuery = NULL;
        m_hCounter = NULL;


        PDH_STATUS lStatus = PdhOpenQuery(NULL, NULL, &m_hQuery);
        if (lStatus != ERROR_SUCCESS)
        {
            return;
        }

        PdhAddCounterW(m_hQuery, counterPath.c_str(), NULL, &m_hCounter);
    }

    /// @brief ��������
    ~CPdh()
    {
        if (NULL != m_hCounter)
        {
            PdhRemoveCounter(m_hCounter);
            m_hCounter = NULL;
        }

        if (NULL != m_hQuery)
        {
            PdhCloseQuery(m_hQuery);
            m_hQuery = NULL;
        }
       
        
    }

    /// @brief �ռ���������
    /// �ú����ķ�ָ�����ռ�ʱ��
    /// @param[in] time �ռ�ʱ��, ��λ����
    /// @param[in] value �洢�ռ��������
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool CollectDataLong(IN unsigned long time, OUT long& value)
    {
        if (NULL == m_hCounter)
            return false;

        PDH_STATUS lStatus = PdhCollectQueryData(m_hQuery);
        if (lStatus != ERROR_SUCCESS)
            return false;

        Sleep(time);

        lStatus = PdhCollectQueryData(m_hQuery);
        if (lStatus != ERROR_SUCCESS)
            return false;

        PDH_FMT_COUNTERVALUE counterValue;
        lStatus = PdhGetFormattedCounterValue(m_hCounter, PDH_FMT_LONG, NULL, &counterValue);
        if (lStatus != ERROR_SUCCESS)
        {
            return false;
        }

        value = counterValue.longValue;

        return true;
    }

private:
    HQUERY  m_hQuery; ///< ��ѯ���
    HCOUNTER m_hCounter; ///< �ռ������
};

LPdh::LPdh(IN const wstring& counterPath)
{
    m_pPdh = new CPdh(counterPath);
}

LPdh::~LPdh()
{
    if (NULL != m_pPdh)
    {
        delete m_pPdh;
        m_pPdh = NULL;
    }
}

bool LPdh::CollectDataLong(IN unsigned long time, OUT long& value)
{
    return m_pPdh->CollectDataLong(time, value);
}