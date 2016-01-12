
#include "LDiskSpeed.h"

#include <stdio.h>
#include <ctime>
#include <cstdlib>
#include <process.h>

#include <vector>
using std::vector;

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

        _beginthread(DiskSequenceTest, 0, (void*)this);

        return true;
    }

    /// @brief ��ȡ����״̬
    LDiskTestState& GetState()
    {
        return m_testState;
    }
private:
    /// @brief ˳�����
    /// @param[in] pParam ����
    static void DiskSequenceTest(void* pParam)
    {
        /*
        �����㷨:
        ����16M�ĳߴ�Ϊ��λ, �������ܲ����д������1���ﵽ2GB��С���ļ�,
        Ȼ������ͬ���ĵ�λ�ߴ��ȡ
        */


        CDiskSequenceTest* pDiskSequenceTest = (CDiskSequenceTest*)pParam;
        LDiskTestState& testState = pDiskSequenceTest->m_testState;

        FILE* pFile = 0; // �ļ�ָ��

        clock_t clockBegin; // ��ʱ��ʼ
        clock_t clockEnd; // ��ʱ����

        // ���˷�ʱ�������ͳ������,ֻ��Ҫ�ѵ�һ������ǿ��ת��Ϊ64λ��
        // �����ļ��ܴ�С2G
        const unsigned long long TOTAL_SIZE = (unsigned long long)(2)  * 1024 * 1024 * 1024;

        // ����һ��16M��д������, ���ҳ�ʼ��Ϊ10101010...�ṹ
        const unsigned long long WRITE_BUFFER_LEN = (unsigned long long)(16) * 1024 * 1024;

        char* pWriteBuffer = new char[WRITE_BUFFER_LEN];
        if (0 == pWriteBuffer)
        {
            testState.Error = true;
            goto SAFE_EXIT;
        }
        for (unsigned long long i = 0; i < WRITE_BUFFER_LEN; i++)
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

        // ����д����
        clockBegin = clock();
        for (unsigned long long i = 0; i < TOTAL_SIZE/WRITE_BUFFER_LEN; i++)
        {
            size_t count = fwrite(pWriteBuffer, WRITE_BUFFER_LEN, 1, pFile);
            fflush(pFile);
            if (1 != count)
            {
                testState.Error = true;
                goto SAFE_EXIT;
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
            testState.WriteSpeed = (float)(WRITE_BUFFER_LEN/1024/1024) * (i + 1.0f)/time;

        }

        // �ر��ļ�Ȼ�����´�
        if (0 != pFile)
        {
            fclose(pFile);
            pFile = 0;
        }
        _wfopen_s(&pFile, pDiskSequenceTest->m_filePath.c_str(), L"rb");
        if (0 == pFile)
        {
            testState.Error = true;
            goto SAFE_EXIT;
        }

        // ���ж�����
        clockBegin = clock();
        for (unsigned long long i = 0; i < TOTAL_SIZE/WRITE_BUFFER_LEN; i++)
        {
            size_t count = fread(pWriteBuffer, WRITE_BUFFER_LEN, 1, pFile);
            if (count != 1)
            {
                testState.Error = true;
                goto SAFE_EXIT;
            }

            // ����ǰ10�β��ȶ�������
            if (i <= 10)
            {
                continue;
            }

            clockEnd = clock();

            // ����ʱ��, �������, �����ȡ�ٶ�
            float time = (float)(clockEnd-clockBegin);
            time = time/1000.0f;
            testState.ReadSpeed = (float)(WRITE_BUFFER_LEN/1024/1024) * (i + 1.0f)/time;
        }
        

SAFE_EXIT:

        // ���ò���״̬
        testState.TestDone = true;

        // �ͷ���Դ
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

        // ɾ���ļ�
        _wremove(pDiskSequenceTest->m_filePath.c_str());
    }

private:
    LDiskTestState m_testState; ///< ����״̬
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

LDiskTestState LDiskSequenceTest::GetState()
{
    return m_pDiskSequenceTest->GetState();
}

class CDisk4KRandomTest
{
public:

    /// @brief ���캯��
    CDisk4KRandomTest()
    {
        m_testState.Error = false;
        m_testState.TestDone = true;
        m_testState.ReadSpeed = 0.0f;
        m_testState.WriteSpeed = 0.0f;
    }

    /// @brief ��������
    ~CDisk4KRandomTest()
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

        _beginthread(Disk4KRandomTest, 0, (void*)this);

        return true;
    }

    /// @brief ��ȡ����״̬
    LDiskTestState& GetState()
    {
        return m_testState;
    }
private:
    /// @brief 4K�������
    /// @param[in] pParam ����
    static void Disk4KRandomTest(void* pParam)
    {
        /*
        �����㷨:
        ����512KB�ĵ�λ�ߴ�����1GB��С�Ĳ����ļ�,
        Ȼ�������ַ��Χ�ڽ������4KB��λ�ߴ����д�뼰��ȡ����, ֱ���ܱ鷶ΧΪֹ
        */

        CDisk4KRandomTest* pDisk4KRandomTest = (CDisk4KRandomTest*)pParam;
        LDiskTestState& testState = pDisk4KRandomTest->m_testState;

        FILE* pFile = 0; // �ļ�ָ��

        clock_t clockBegin; // ��ʱ��ʼ
        clock_t clockEnd; // ��ʱ����

        // ���˷�ʱ�������ͳ������,ֻ��Ҫ�ѵ�һ������ǿ��ת��Ϊ64λ��
        // �����ļ��ܴ�С1G
        const unsigned long long TOTAL_SIZE = (unsigned long long)(1)  * 1024 * 1024 * 1024;

        // ����д�������ͻ�������С
        char* pWriteBuffer = 0;
        const unsigned long long WRITE_BUFFER_LEN = (unsigned long long)(4) * 1024;

        vector<unsigned long long> writeOffsetList; // �ļ�д��λ��ƫ��ֵ�б�

        // ���ɲ����ļ�
        bool bRet = GenerateFile(pDisk4KRandomTest->m_filePath, TOTAL_SIZE, 512 * 1024);
        if (!bRet)
        {
            testState.Error = true;
            goto SAFE_EXIT;
        }

        // ����һ��д������, ���ҳ�ʼ��Ϊ01010101...�ṹ
        pWriteBuffer = new char[WRITE_BUFFER_LEN];
        if (0 == pWriteBuffer)
        {
            testState.Error = true;
            goto SAFE_EXIT;
        }
        for (unsigned long long i = 0; i < WRITE_BUFFER_LEN; i++)
        {
            pWriteBuffer[i] = (char)0x55;
        }



        // ���ļ�
        _wfopen_s(&pFile, pDisk4KRandomTest->m_filePath.c_str(), L"wb+");
        if (0 == pFile)
        {
            testState.Error = true;
            goto SAFE_EXIT;
        }

        // ��ʼ��дƫ���б�, �ճ�ʼ�����ƫ���б�Ϊ˳���
        writeOffsetList.resize(TOTAL_SIZE/WRITE_BUFFER_LEN);
        for (unsigned long long i = 0; i < writeOffsetList.size(); i++)
        {
            writeOffsetList[i] = i * WRITE_BUFFER_LEN;
        }

        // ����˳��
        for (unsigned long long i = 0; i < writeOffsetList.size() * 2; i++)
        {
            
        }

        // ����д����
        clockBegin = clock();
        for (unsigned long long i = 0; i < TOTAL_SIZE/WRITE_BUFFER_LEN; i++)
        {
            size_t count = fwrite(pWriteBuffer, WRITE_BUFFER_LEN, 1, pFile);
            fflush(pFile);
            if (1 != count)
            {
                testState.Error = true;
                goto SAFE_EXIT;
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
            testState.WriteSpeed = (float)(WRITE_BUFFER_LEN/1024/1024) * (i + 1.0f)/time;

        }


SAFE_EXIT:

        // ���ò���״̬
        testState.TestDone = true;

        // �ͷ���Դ
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

        // ɾ���ļ�
        //_wremove(pDisk4KRandomTest->m_filePath.c_str());
    }

    /// @brief �����ļ�
    /// ��λ�ߴ�ֵ������Ա��ļ���С����
    /// @param[in] filePath �ļ�·��
    /// @param[in] fileSize �ļ���С
    /// @param[in] elemSize �����ļ��ĵ�λ�ߴ�
    /// @return �ɹ�����true, ʧ�ܷ���false
    static bool GenerateFile(
        IN const wstring& filePath, 
        IN unsigned long long fileSize, 
        IN unsigned int elemSize)
    {
        bool bRet = false; // ��������ֵ
        FILE* pFile = 0; // �ļ�ָ��
        char* pWriteBuffer = 0; // д������

        // ���ļ�
        _wfopen_s(&pFile, filePath.c_str(), L"wb");
        if (0 == pFile)
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        // ��ʼ��д�뻺����
        pWriteBuffer = new char[elemSize];
        if (0 == pWriteBuffer)
        {
            bRet = false;
            goto SAFE_EXIT;
        }
        for (unsigned long long i = 0; i < elemSize; i++)
        {
            pWriteBuffer[i] = (char)0xAA;
        }

        // ���ɲ����ļ�
        for (unsigned long long i = 0; i < fileSize/elemSize; i++)
        {
            size_t count = fwrite(pWriteBuffer, elemSize, 1, pFile);
            fflush(pFile);
            if (1 != count)
            {
                bRet = false;
                goto SAFE_EXIT;
            }

        }

        bRet = true;

SAFE_EXIT:

        // �ͷ���Դ
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

        return bRet;
    }

private:
    LDiskTestState m_testState; ///< ����״̬
    wstring m_filePath; ///< �����ļ�·��
};

LDisk4KRandomTest::LDisk4KRandomTest()
{
    m_pDisk4KRandomTest = 0;
    m_pDisk4KRandomTest = new CDisk4KRandomTest();
}

LDisk4KRandomTest::~LDisk4KRandomTest()
{
    if (m_pDisk4KRandomTest != 0)
    {
        delete m_pDisk4KRandomTest;
        m_pDisk4KRandomTest = 0;
    }
}

bool LDisk4KRandomTest::Start(IN const wstring& filePath)
{
    return m_pDisk4KRandomTest->Start(filePath);
}

LDiskTestState LDisk4KRandomTest::GetState()
{
    return m_pDisk4KRandomTest->GetState();
}