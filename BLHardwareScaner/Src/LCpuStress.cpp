

#include "LCpuStress.h"

#include <process.h>

#include <Windows.h>

/// @�߾��ȼ�ʱ����
class LTimer
{
public:
    LTimer()
    {
        // ��ȡCPU��ʱ��Ƶ��(��ÿ��ĵδ���)
        QueryPerformanceFrequency((LARGE_INTEGER*)&m_performanceFreq);
    }

    /// @brief ��ʼ��ʱ
    void Start()
    {
        m_startTime = 0;
        m_time = 0;
        QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);
    }

    /// @brief ������ʱ
    void End()
    {
        LONGLONG currentTime = 0;
        QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
        m_time = currentTime - m_startTime;
    }

    /// @brief ��ȡ��ʱʱ��
    /// @return ��ʱʱ��(��λ����)
    double Time()
    {
        double time = 0.0f;
        time = (double)(m_time * 1000) / (double)m_performanceFreq;
        return time;
    }

private:
    LONGLONG m_time; ///< ��ʱʱ�� 
    LONGLONG m_startTime; ///< ��ʱ����ʼʱ�� 
    LONGLONG m_performanceFreq; ///< CPUʱ��Ƶ��(��ÿ���ӵĵδ���)
};

/// @brief �̲߳���
struct SThreadParam
{
    unsigned int Id; ///< ����߳�ID
    CCpuStressTest* PStressTest; ///< CPUѹ�����Զ���ָ��
};

/// @brief CPUѹ������ʵ����
class CCpuStressTest
{
public:
    /// @brief ���캯��
    CCpuStressTest()
    {
        m_testState.TestDone = true;
        m_testState.LogicalCoreNum = 0;
        for (unsigned int i = 0; i < MAX_CPU_LOGICAL_CORE_NUM; i++)
        {
            m_testState.Score[i] = 0;
            m_threadTestDone[i] = false;
        }

        m_threadNum = 0;
        m_bStopTest = false;
        m_timeOut = 0;
    }

    /// @brief ��������
    ~CCpuStressTest()
    {

    }

    /// @brief ��ʼ����
    /// @param[in] timeOut ��ʱʱ��, ��λ��
    /// @return �ɹ�����true, ʧ�ܷ���false, �������ڽ����л᷵��false
    bool Start(IN unsigned int timeOut)
    {
        if (!m_testState.TestDone)
            return false;

        m_testState.TestDone = false;
        for (unsigned int i = 0; i < MAX_CPU_LOGICAL_CORE_NUM; i++)
        {
            m_testState.Score[i] = 0;
            m_threadTestDone[i] = false;
        }

        m_bStopTest = false;

        m_timeOut = timeOut;

        // ��ȡ�߼���������
        SYSTEM_INFO systemInfo = {0};
        GetNativeSystemInfo(&systemInfo);
        m_testState.LogicalCoreNum = systemInfo.dwNumberOfProcessors;
        m_threadNum = systemInfo.dwNumberOfProcessors;

        for (unsigned int i = 0; i < m_threadNum; i++)
        {
            m_threadParam[i].Id = i;
            m_threadParam[i].PStressTest = this;
            _beginthread(CpuStressTestThread, 0, (void*)&(m_threadParam[i]));
        }

         _beginthread(CpuStressMonitorThread, 0, (void*)this);

        return true;
    }

    /// @brief ��ȡ����״̬
    /// @return ����״̬
    const LCpuStressTestState& GetState()
    {
        return m_testState;
    }

    /// @brief ֹͣ����
    void Stop()
    {
        m_bStopTest = true;
    }

private:
    unsigned int m_timeOut; ///< ��ʱʱ��
    bool m_bStopTest; ///< ����Ƿ��������
    unsigned int m_threadNum; ///< �߳�����
    bool m_threadTestDone[MAX_CPU_LOGICAL_CORE_NUM]; ///< ���ÿ�������߳��Ƿ����
    LCpuStressTestState m_testState; ///< ����״̬

    SThreadParam m_threadParam[MAX_CPU_LOGICAL_CORE_NUM]; ///< ��¼�߳�ID

private:
        /// @brief CPUѹ�������߳�
    static void CpuStressMonitorThread(void* pParam)
    {
        CCpuStressTest* pCpuStressTest = (CCpuStressTest*)pParam;

        while (true)
        {
            bool threadRun = false;

            for (unsigned int i = 0; i < pCpuStressTest->m_threadNum; i++)
            {
                if (!pCpuStressTest->m_threadTestDone[i])
                    threadRun = true;
            }

            if (!threadRun)
                break;

            Sleep(200);
        }


        pCpuStressTest->m_testState.TestDone = true;
        
    }

    /// @brief CPUѹ�������߳�
    /// @param[in] pParam ����
    static void CpuStressTestThread(void* pParam)
    {
        SThreadParam* pThreadParam = (SThreadParam*)pParam;

        // �������߳���ص�ָ��CPU����������
        DWORD threadMask = 1;
        threadMask = threadMask << pThreadParam->Id;
        SetThreadAffinityMask(GetCurrentThread(), threadMask);

        CCpuStressTest* pCpuStressTest = pThreadParam->PStressTest;
        CalculatePI(
            pCpuStressTest->m_timeOut, 
            pCpuStressTest->m_bStopTest,
            pCpuStressTest->m_testState.Score[pThreadParam->Id]);

        pCpuStressTest->m_threadTestDone[pThreadParam->Id] = true;
        
    }

    /// @brief ����PI
    /// @param[in] timeOut ��ʱʱ��, ��λ��
    /// @param[in] bStopTest ����Ƿ�ֹͣ����
    /// @param[out] floatLen �洢������ľ���ֵ
    /// @return �����PI�ľ���ֵ
    static void CalculatePI(IN unsigned int timeOut, IN bool& bStopTest, OUT unsigned long& floatLen)
    {
        /*
        PI/2 = 1 + 1/3 + 1/3 * 2/5 + 1/3 * 2/5 * 3/7...
        */

        const int MAX_FLOAT_LEN = 80000; //���������㾫��

        unsigned long pi[MAX_FLOAT_LEN] = {0}; // �洢�������PIֵ, �����е�ÿ��ֵ����ʮ�����е�һλ
        unsigned long z[MAX_FLOAT_LEN] = {0}; // ÿ�ε���piֵ���²���, ��2/3, 2/3 * 2/5, ...

        pi[1] = 2; // ����PI�Ŀ�ʼֵΪ2
        z[1] = 2; // �����²��ֵĿ�ʼֵΪ2

        unsigned long a = 1; // ÿ�ε���ֵΪ 1, 2, 3, 4, ...
        unsigned long b = 3; // ÿ�ε���ֵΪ 3, 5, 7, 9, ...

        floatLen = 0;

        LTimer timer;
        timer.Start();

        while (true)
        {
            unsigned long d = 0;
            unsigned long c = 0; 

            // z *= a;
            d = 0; // �˷��еĽ�λֵ
            for (int i = MAX_FLOAT_LEN - 1; i > 0; i--)
            {
                c = z[i] * a + d;
                z[i] = c % 10;
                d = c / 10;
            }

            // z /= b;
            d = 0; // �����е�����ֵ
            for (int i = 0; i < MAX_FLOAT_LEN; i++)
            {
                c = z[i] + d * 10;
                z[i] = c / b;
                d = c % b;
            }

            // pi += z;
            unsigned long maxFloatFlag = 0; // ���z[i]ȫ��Ϊ0, ��maxFloatFlagΪ0, ��ʾ����ﵽ��󾫶�
            for (int i = MAX_FLOAT_LEN - 1; i > 0; i--)
            {
                c = pi[i] + z[i];
                pi[i] = c % 10;
                pi[i - 1] += c / 10;
                maxFloatFlag |= z[i];
            }

            a++;
            b += 2;

            // �ﵽ��󾫶�, ֱ�ӷ���
            if (maxFloatFlag == 0)
            {
                floatLen = MAX_FLOAT_LEN;
                break;
            }

            // ���㾫��
            for (int i = 0; i < MAX_FLOAT_LEN; i++)
            {
                if (z[i] != 0)
                {
                    floatLen = (unsigned long)i;
                    break;
                }

            }

            // ��ʱ����ֹͣ����, ���˳�
            timer.End();
            if (timer.Time() >= timeOut * 1000 || bStopTest)
            {
                break;
            }
        }

    }
};

LCpuStressTest::LCpuStressTest()
{
    m_pCpuStressTest = new CCpuStressTest();
}

LCpuStressTest::~LCpuStressTest()
{
    if (m_pCpuStressTest != 0)
    {
        delete m_pCpuStressTest;
        m_pCpuStressTest = 0;
    }
}

bool LCpuStressTest::Start(IN unsigned int timeOut)
{
    return m_pCpuStressTest->Start(timeOut);
}

const LCpuStressTestState& LCpuStressTest::GetState()
{
    return m_pCpuStressTest->GetState();
}

void LCpuStressTest::Stop()
{
    return m_pCpuStressTest->Stop();
}