
#include "LDiskSpeed.h"

#include <stdio.h>
#include <ctime>
#include <cstdlib>
#include <process.h>

#include <vector>
using std::vector;
#include <bitset>
using std::bitset;

#include <Windows.h>

/// @brief �����������
/// @param[in] min �����������Сֵ
/// @param[in] max ������������ֵ
/// @return �������
static int RandInt(int min, int max) 
{
    return rand()%(max - min + 1) + min;
}

/// @brief �������������
/// �������н���������ֵ
/// @param[in] bufferSize ��������С
/// @return ������ָ��
static char* CreateRandomBuffer(unsigned long bufferSize)
{
    char* pBuffer = new char[bufferSize];
    if (0 != pBuffer)
    {
        for (unsigned int i = 0; i < bufferSize; i++)
        {
            pBuffer[i] = (char)RandInt(0, 255);
        }
    }

    return pBuffer;
}

/// @brief �������256K��Χ�ڵ�ֵ
/// @return ���ֵ��Χ[0, 256K-1]
static unsigned long RandValueFrom256K()
{
    bitset<32> set(0);
    for (unsigned int i = 0; i < 18; i++)
    {
        if (RandInt(0, 1) == 1)
            set[i] = 1;
    }

    unsigned long randValue = set.to_ulong();
    return randValue;
}



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

        HANDLE hFile = 0; // �ļ����

        clock_t clockBegin; // ��ʱ��ʼ
        clock_t clockEnd; // ��ʱ����

        // ���˷�ʱ�������ͳ������,ֻ��Ҫ�ѵ�һ������ǿ��ת��Ϊ64λ��
        // �����ļ��ܴ�С2G
        const unsigned long long TOTAL_SIZE = (unsigned long long)(2)  * 1024 * 1024 * 1024;

        // ����һ��16M��д������, ���ҳ�ʼ��Ϊ10101010...�ṹ
        const unsigned long long WRITE_BUFFER_LEN = (unsigned long long)(16) * 1024 * 1024;

        char* pWriteBuffer = CreateRandomBuffer(WRITE_BUFFER_LEN);
        if (0 == pWriteBuffer)
        {
            testState.Error = true;
            goto SAFE_EXIT;
        }
        
        // �����ļ�
        hFile = CreateFileW(
            pDiskSequenceTest->m_filePath.c_str(), 
            GENERIC_WRITE | GENERIC_READ, 
            0, 
            NULL, 
            CREATE_ALWAYS, 
            FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH, 
            NULL);
        if (0 == hFile || INVALID_HANDLE_VALUE == hFile)
        {
            testState.Error = true;
            goto SAFE_EXIT;
        }

        // ����д����
        clockBegin = clock();
        for (unsigned long long i = 0; i < TOTAL_SIZE/WRITE_BUFFER_LEN; i++)
        {
            unsigned long count = 0;
            BOOL ret = WriteFile(hFile, (LPCVOID)pWriteBuffer, WRITE_BUFFER_LEN, &count, NULL);
            if (FALSE == ret)
            {
                testState.Error = true;
                goto SAFE_EXIT;
            }
            
            clockEnd = clock();

            // ����ʱ��, �������, ����д���ٶ�
            float time = (float)(clockEnd-clockBegin);
            time = time/1000.0f;
            testState.WriteSpeed = (float)(WRITE_BUFFER_LEN/1024/1024) * (i + 1.0f)/time;

        }

        // ���ж�����
        SetFilePointer(hFile, 0, 0, FILE_BEGIN);
        clockBegin = clock();
        for (unsigned long long i = 0; i < TOTAL_SIZE/WRITE_BUFFER_LEN; i++)
        {
            unsigned long count = 0;
            BOOL ret = ReadFile(hFile, (LPVOID)pWriteBuffer, WRITE_BUFFER_LEN, &count, NULL);
            if (FALSE == ret)
            {
                testState.Error = true;
                goto SAFE_EXIT;
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
        if (0 != hFile)
        {
            CloseHandle(hFile);
            hFile = 0;
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
        Ȼ�������ַ��Χ�ڽ������4KB��λ�ߴ����д�뼰��ȡ����
        */

        CDisk4KRandomTest* pDisk4KRandomTest = (CDisk4KRandomTest*)pParam;
        LDiskTestState& testState = pDisk4KRandomTest->m_testState;
        wstring& filePath = pDisk4KRandomTest->m_filePath;

        // ���˷�ʱ�������ͳ������,ֻ��Ҫ�ѵ�һ������ǿ��ת��Ϊ64λ��
        // �����ļ��ܴ�С1G
        const unsigned long TOTAL_SIZE = (unsigned long)(1)  * 1024 * 1024 * 1024;


        // ���ɲ����ļ�
        bool bRet = GenerateFile(filePath, TOTAL_SIZE, 512 * 1024);
        if (!bRet)
        {
            testState.Error = true;
            goto SAFE_EXIT;
        }

        // ����д����
        bRet = Disk4KRandomWriteTest(filePath, &testState.WriteSpeed);
        if (!bRet)
        {
            testState.Error = true;
            goto SAFE_EXIT;
        }

        // ���ж�����
        bRet = Disk4KRandomReadTest(filePath, &testState.ReadSpeed);
        if (!bRet)
        {
            testState.Error = true;
            goto SAFE_EXIT;
        }

SAFE_EXIT:

        // ���ò���״̬
        testState.TestDone = true;

        // ɾ���ļ�
        _wremove(filePath.c_str());
    }

    /// @brief 4K���д����
    /// ��ȷ��������ļ���С��С��1G, �������������20��
    /// @param[in] filePath �ļ�·��
    /// @param[out] pSpeed �洢д�ٶ�
    static bool Disk4KRandomWriteTest(IN const wstring& filePath, OUT float* pSpeed)
    {
        const unsigned long TEST_TIME = 20000; // ��λms
        const unsigned long  WRITE_BUFFER_LEN = (unsigned long)(4) * 1024;


        // ��ʶ��������ֵ
        bool bRet = false;

        HANDLE hFile = NULL;
        unsigned int rwCount = 0;
        
        // ��ʱ��ʼ����
        clock_t clockBegin;
        // ��ʱ��������
        clock_t clockEnd; 

        // ����д������
        char* pWriteBuffer = 0;

        if ( 0 == pSpeed)
        {
            bRet = false;
            goto SAFE_EXIT;
        }
        

        // ����һ��д������
        pWriteBuffer = CreateRandomBuffer(WRITE_BUFFER_LEN);
        if (0 == pWriteBuffer)
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        // ���ļ�
        hFile = CreateFileW(
            filePath.c_str(), 
            GENERIC_WRITE | GENERIC_READ, 
            0, 
            NULL, 
            OPEN_EXISTING, 
            FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH, 
            NULL);
        if (0 == hFile || INVALID_HANDLE_VALUE == hFile)
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        // ����д����
        clockBegin = clock();
        clockEnd = clock();
        rwCount = 0;
        while ((clockEnd - clockBegin) < TEST_TIME)
        {
            rwCount++;

            // 4K * 256K = 1G
            unsigned long randAddr = RandValueFrom256K();
            SetFilePointer(hFile, randAddr * WRITE_BUFFER_LEN, 0, FILE_BEGIN);

            unsigned long count = 0;
            BOOL ret = WriteFile(hFile, (LPCVOID)pWriteBuffer, WRITE_BUFFER_LEN, &count, NULL);

            if (FALSE == ret)
            {
                bRet = false;
                goto SAFE_EXIT;
            }

            clockEnd = clock();

            // ����ʱ��, �������, ����д���ٶ�
            float time = (float)(clockEnd-clockBegin);
            time = time/1000.0f;
            (*pSpeed) = (float)WRITE_BUFFER_LEN/1024.0f/1024.0f * rwCount/time;
        }

        bRet = true;

SAFE_EXIT:
        if (NULL != hFile ||
            INVALID_HANDLE_VALUE != hFile)
        {
            CloseHandle(hFile);
            hFile = NULL;
        }

        if (0 != pWriteBuffer)
        {
            delete[] pWriteBuffer;
            pWriteBuffer = 0;
        }

        return bRet;
    }

    /// @brief 4K���������
    /// ��ȷ��������ļ���С��С��1G, �������������20��
    /// @param[in] filePath �ļ�·��
    /// @param[out] pSpeed �洢���ٶ�
    static bool Disk4KRandomReadTest(IN const wstring& filePath, OUT float* pSpeed)
    {
        const unsigned long TEST_TIME = 20000; // ��λms
        const unsigned long  READ_BUFFER_LEN = (unsigned long)(4) * 1024;


        // ��ʶ��������ֵ
        bool bRet = false;

        HANDLE hFile = NULL;
        unsigned int rwCount = 0;

        // ��ʱ��ʼ����
        clock_t clockBegin;
        // ��ʱ��������
        clock_t clockEnd; 

        // �����������
        char* pReadBuffer = 0;

        if ( 0 == pSpeed)
        {
            bRet = false;
            goto SAFE_EXIT;
        }


        // ����һ����������
        pReadBuffer = CreateRandomBuffer(READ_BUFFER_LEN);
        if (0 == pReadBuffer)
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        // ���ļ�
        hFile = CreateFileW(
            filePath.c_str(), 
            GENERIC_WRITE | GENERIC_READ, 
            0, 
            NULL, 
            OPEN_EXISTING, 
            FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH, 
            NULL);
        if (0 == hFile || INVALID_HANDLE_VALUE == hFile)
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        // ���ж�����
        clockBegin = clock();
        clockEnd = clock();
        rwCount = 0;
        while ((clockEnd - clockBegin) < TEST_TIME)
        {
            rwCount++;

            // 4K * 256K = 1G
            unsigned long randAddr = RandValueFrom256K();
            SetFilePointer(hFile, randAddr * READ_BUFFER_LEN, 0, FILE_BEGIN);

            unsigned long count = 0;
            BOOL ret = ReadFile(hFile, (LPVOID)pReadBuffer, READ_BUFFER_LEN, &count, NULL);

            if (FALSE == ret)
            {
                bRet = false;
                goto SAFE_EXIT;
            }

            clockEnd = clock();

            // ����ʱ��, �������, ����д���ٶ�
            float time = (float)(clockEnd-clockBegin);
            time = time/1000.0f;
            (*pSpeed) = (float)READ_BUFFER_LEN/1024.0f/1024.0f * rwCount/time;
        }

        bRet = true;

SAFE_EXIT:
        if (NULL != hFile ||
            INVALID_HANDLE_VALUE != hFile)
        {
            CloseHandle(hFile);
            hFile = NULL;
        }

        if (0 != pReadBuffer)
        {
            delete[] pReadBuffer;
            pReadBuffer = 0;
        }

        return bRet;
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
        pWriteBuffer = CreateRandomBuffer(elemSize);
        if (0 == pWriteBuffer)
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        // ���ɲ����ļ�
        for (unsigned long long i = 0; i < fileSize/elemSize; i++)
        {
            size_t count = fwrite(pWriteBuffer, elemSize, 1, pFile);
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