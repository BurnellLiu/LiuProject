
#include "LDiskSpeed.h"

#include <stdio.h>
#include <ctime>
#include <process.h>

/// @brief ����˳�����
class CDiskSequenceTest
{
public:

    /// @brief ���캯��
    CDiskSequenceTest()
    {
        m_testState.Error = false;
        m_testState.TestDone = true;
        m_testState.ReadSpeed = 0.0f;
        m_testState.WriteSpeed = 0.0f;
    }

    /// @brief ��������
    ~CDiskSequenceTest()
    {

    }

    /// @brief ��ʼ����
    bool Start(IN const wstring& filePath)
    {
        if (false == m_testState.TestDone)
            return false;
        
        m_filePath = filePath;
        m_testState.TestDone = false;
        m_testState.Error = false;
        m_testState.WriteSpeed = 0.0f;
        m_testState.ReadSpeed = 0.0f;

        _beginthread(SequenceTest, 0, (void*)this);

        return true;
    }

    /// @brief ��ȡ����״̬
    LSpeedTestState& GetState()
    {
        return m_testState;
    }
private:
    /// @brief ˳�����
    /// @param[in] pParam ����
    static void SequenceTest(void* pParam)
    {
        FILE* pFile = 0; // �ļ�ָ��

        clock_t clockBegin; // ��ʱ��ʼ
        clock_t clockEnd; // ��ʱ����

        CDiskSequenceTest* pDiskSequenceTest = (CDiskSequenceTest*)pParam;
        LSpeedTestState& testState = pDiskSequenceTest->m_testState;

        // ����һ��16M��д������, ���ҳ�ʼ��Ϊ10101010...�ṹ
        const unsigned int WRITE_BUFFER_LEN = 16 * 1024 * 1024;
        char* pWriteBuffer = new char[WRITE_BUFFER_LEN];
        if (0 == pWriteBuffer)
        {
            testState.Error = true;
            goto SAFE_EXIT;
        }
        for (unsigned int i = 0; i < WRITE_BUFFER_LEN; i++)
        {
            pWriteBuffer[i] = (char)0xAA;
        }
        
        // ���ļ�
        _wfopen_s(&pFile, pDiskSequenceTest->m_filePath.c_str(), L"wb");
        if (0 == pFile)
        {
            testState.Error = true;
            goto SAFE_EXIT;
        }

        clockBegin = clock();
        // �ܹ�д��128 * 16M = 1G������
        for (unsigned int i = 0; i < 128; i++)
        {
            size_t count = fwrite(pWriteBuffer, WRITE_BUFFER_LEN, 1, pFile);
            fflush(pFile);
            if (1 != count)
            {
                testState.Error = true;
                break;
            }

            // ����ǰ10�β��ȶ�������
            if (i <= 10)
            {
                continue;
            }
            
            clockEnd = clock();

            // ����ʱ��, �������, ����д���ٶ�
            float time = (float)(clockEnd-clockBegin);
            time = time/1000.0f;
            testState.WriteSpeed = 16.0f * (i + 1.0f)/time;

        }
        

        SAFE_EXIT:
        testState.TestDone = true;

        if (0 != pFile)
        {
            fclose(pFile);
            pFile = 0;
        }
        if (0 != pWriteBuffer)
        {
            delete[] pWriteBuffer;
            pWriteBuffer = 0;
        }
    }

private:
    LSpeedTestState m_testState; ///< ����״̬
    wstring m_filePath; ///< �����ļ�·��
};

LDiskSequenceTest::LDiskSequenceTest()
{
    m_pDiskSequenceTest = 0;
    m_pDiskSequenceTest = new CDiskSequenceTest();
}

LDiskSequenceTest::~LDiskSequenceTest()
{
    if (0 != m_pDiskSequenceTest)
    {
        delete m_pDiskSequenceTest;
        m_pDiskSequenceTest = 0;
    }
}

bool LDiskSequenceTest::Start(IN const wstring& filePath)
{
    return m_pDiskSequenceTest->Start(filePath);
}

LSpeedTestState LDiskSequenceTest::GetState()
{
    return m_pDiskSequenceTest->GetState();
}