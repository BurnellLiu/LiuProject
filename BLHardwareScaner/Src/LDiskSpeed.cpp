
#include "LDiskSpeed.h"

#include <ctime>
#include <cstdlib>
#include <process.h>

#include <vector>
using std::vector;
#include <bitset>
using std::bitset;

#include <Windows.h>

/// @brief ������Ϣ�ַ����б�
static wchar_t* ErrorMsgList[] =
{
    L"No Error",
    L"Open File Error",
    L"Write File Error",
    L"Read File Error",
    L"Alloc Memory Error",
    L"Test Abort Error"
};

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

/// @brief ��ȡWindows������
/// @param[out] pWStr �洢������Ϣ
static void GetWindowsError(OUT wstring* pWStr)
{
    if (0 == pWStr)
        return;

    wchar_t windowsErrorCode[256] = {0};
    wsprintfW(windowsErrorCode, L"Windows Error Code: 0x%x", GetLastError());
    (*pWStr) = windowsErrorCode;
}

/// @brief �����ٶȲ����ļ��ṹ
struct LDiskSpeedTestFile
{
    wstring Path; ///< �����ļ�·��
    unsigned long Size; ///< �����ļ���С, �����ļ����Ϊ2^32-1(��4G-1)
};

/// @brief ����˳�����
class CDiskSequenceTest
{
public:

    /// @brief ���캯��
    CDiskSequenceTest()
    {
        m_bStopTest = false;

        m_testFile.Path = L"";
        m_testFile.Size = 0;

        m_testState.Error = DST_NO_ERROR;
        m_testState.ErrorMsg = ErrorMsgList[DST_NO_ERROR];
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
       

        m_bStopTest = false;

        // �����ļ�����Ϊ2G��С
        m_testFile.Path = filePath;
        m_testFile.Size = (unsigned long)(2)  * 1024 * 1024 * 1024;

        m_testState.TestDone = false;
        m_testState.Error = DST_NO_ERROR;
        m_testState.ErrorMsg = ErrorMsgList[DST_NO_ERROR];
        m_testState.ErrorMsgWindows = L"";
        m_testState.WriteSpeed = 0.0f;
        m_testState.ReadSpeed = 0.0f;

        _beginthread(DiskSequenceTestThread, 0, (void*)this);

        return true;
    }

    /// @brief ��ȡ����״̬
    LDiskSpeedTestState& GetState()
    {
        return m_testState;
    }

    /// @brief ֹͣ����
    void Stop()
    {
        m_bStopTest = true;
    }

private:
    LDiskSpeedTestFile m_testFile; ///< �����ļ�
    LDiskSpeedTestState m_testState; ///< ����״̬
    bool m_bStopTest; ///< ����Ƿ�ֹͣ����

private:
    /// @brief ˳�����
    /// @param[in] pParam ����
    static void DiskSequenceTestThread(void* pParam)
    {
        /*
        �����㷨:
        ����16M�ĳߴ�Ϊ��λ, �������ܲ����д������1���ﵽ2GB��С���ļ�,
        Ȼ������ͬ���ĵ�λ�ߴ��ȡ
        */

        CDiskSequenceTest* pDiskSequenceTest = (CDiskSequenceTest*)pParam;
        LDiskSpeedTestState& testState = pDiskSequenceTest->m_testState;
        LDiskSpeedTestFile& testFile = pDiskSequenceTest->m_testFile;

        HANDLE hFile = 0; // �ļ����

        clock_t clockBegin; // ��ʱ��ʼ
        clock_t clockEnd; // ��ʱ����

        // ����һ��16M�Ķ�д������
        const unsigned long RW_BUFFER_SIZE = (unsigned long)(16) * 1024 * 1024;

        char* pRWBuffer = CreateRandomBuffer(RW_BUFFER_SIZE);
        if (0 == pRWBuffer)
        {
            testState.Error = DST_ALLOC_MEMORY_ERROR;
            testState.ErrorMsg = ErrorMsgList[DST_ALLOC_MEMORY_ERROR];
            goto SAFE_EXIT;
        }
        
        // �����ļ�, �ر��ļ��������Զ�ɾ���ļ�, ��������رպ�, Ҳ��ɾ�������ļ�
        hFile = CreateFileW(
            testFile.Path.c_str(), 
            GENERIC_WRITE | GENERIC_READ, 
            0, 
            NULL, 
            CREATE_ALWAYS, 
            FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH | FILE_FLAG_DELETE_ON_CLOSE, 
            NULL);
        if (0 == hFile || INVALID_HANDLE_VALUE == hFile)
        {
            testState.Error = DST_OPEN_FILE_ERROR;
            testState.ErrorMsg = ErrorMsgList[DST_OPEN_FILE_ERROR];
            GetWindowsError(&testState.ErrorMsgWindows);

            goto SAFE_EXIT;
        }

        // ����д����
        clockBegin = clock();
        for (unsigned long i = 0; i < testFile.Size/RW_BUFFER_SIZE; i++)
        {
            unsigned long count = 0;
            BOOL ret = WriteFile(hFile, (LPCVOID)pRWBuffer, RW_BUFFER_SIZE, &count, NULL);
            if (FALSE == ret)
            {
                testState.Error = DST_WRITE_FILE_ERROR;
                testState.ErrorMsg = ErrorMsgList[DST_WRITE_FILE_ERROR];
                GetWindowsError(&testState.ErrorMsgWindows);

                goto SAFE_EXIT;
            }
            
            if (pDiskSequenceTest->m_bStopTest)
            {
                testState.Error = DST_TEST_ABORT_ERROR;
                testState.ErrorMsg = ErrorMsgList[DST_TEST_ABORT_ERROR];
                goto SAFE_EXIT;
            }

            clockEnd = clock();

            // ����ʱ��, �������, ����д���ٶ�
            float time = (float)(clockEnd-clockBegin);
            time = time/1000.0f;
            testState.WriteSpeed = (float)(RW_BUFFER_SIZE/1024/1024) * (i + 1.0f)/time;

        }

        // ���ж�����
        SetFilePointer(hFile, 0, 0, FILE_BEGIN);
        clockBegin = clock();
        for (unsigned long i = 0; i < testFile.Size/RW_BUFFER_SIZE; i++)
        {
            unsigned long count = 0;
            BOOL ret = ReadFile(hFile, (LPVOID)pRWBuffer, RW_BUFFER_SIZE, &count, NULL);
            if (FALSE == ret)
            {
                testState.Error = DST_READ_FILE_ERROR;
                testState.ErrorMsg = ErrorMsgList[DST_READ_FILE_ERROR];
                GetWindowsError(&testState.ErrorMsgWindows);

                goto SAFE_EXIT;
            }

            if (pDiskSequenceTest->m_bStopTest)
            {
                testState.Error = DST_TEST_ABORT_ERROR;
                testState.ErrorMsg = ErrorMsgList[DST_TEST_ABORT_ERROR];
                goto SAFE_EXIT;
            }

            clockEnd = clock();

            // ����ʱ��, �������, �����ȡ�ٶ�
            float time = (float)(clockEnd-clockBegin);
            time = time/1000.0f;
            testState.ReadSpeed = (float)(RW_BUFFER_SIZE/1024/1024) * (i + 1.0f)/time;
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
        if (0 != pRWBuffer)
        {
            delete[] pRWBuffer;
            pRWBuffer = 0;
        }
    }
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

LDiskSpeedTestState LDiskSequenceTest::GetState()
{
    return m_pDiskSequenceTest->GetState();
}

void LDiskSequenceTest::Stop()
{
    m_pDiskSequenceTest->Stop();
}

class CDisk4KRandomTest
{
public:

    /// @brief ���캯��
    CDisk4KRandomTest()
    {
        m_testTime = 0;

        m_bStopTest = false;

        m_testState.Error = DST_NO_ERROR;
        m_testState.ErrorMsg = ErrorMsgList[DST_NO_ERROR];
        m_testState.TestDone = true;
        m_testState.ReadSpeed = 0.0f;
        m_testState.WriteSpeed = 0.0f;

        m_testFile.Path = L"";
        m_testFile.Size = 0;

        m_testFileHandle = 0;
    }

    /// @brief ��������
    ~CDisk4KRandomTest()
    {
        if (m_testFileHandle != 0 &&
            m_testFileHandle != INVALID_HANDLE_VALUE)
        {
            CloseHandle(m_testFileHandle);
            m_testFileHandle = 0;
        }

    }

    /// @brief ��ʼ����
    bool Start(IN const wstring& filePath)
    {
        if (false == m_testState.TestDone)
            return false;

        // ����ʱ������Ϊ20��
        m_testTime = 20000;

        m_bStopTest = false;

        m_testFile.Path = filePath;

        // ���˷�ʱ�������ͳ������,ֻ��Ҫ�ѵ�һ������ǿ��ת��Ϊ�޷���32λ��
        // �����ļ��ܴ�С1G
        m_testFile.Size = (unsigned long)(1)  * 1024 * 1024 * 1024;

        m_testState.TestDone = false;
        m_testState.Error = DST_NO_ERROR;
        m_testState.ErrorMsg = ErrorMsgList[DST_NO_ERROR];
        m_testState.ErrorMsgWindows = L"";
        m_testState.WriteSpeed = 0.0f;
        m_testState.ReadSpeed = 0.0f;

        if (m_testFileHandle != 0 &&
            m_testFileHandle != INVALID_HANDLE_VALUE)
        {
            CloseHandle(m_testFileHandle);
            m_testFileHandle = 0;
        }

        _beginthread(Disk4KRandomTestThread, 0, (void*)this);

        return true;
    }

    /// @brief ��ȡ����״̬
    LDiskSpeedTestState& GetState()
    {
        return m_testState;
    }

    /// @brief ֹͣ����
    void Stop()
    {
        if (true == m_testState.TestDone)
            return;

        m_bStopTest = true;

    }

private:
    /// @brief 4K���д����
    /// �������������20��
    bool Disk4KRandomWriteTest()
    {
        // д������4K
        const unsigned long  WRITE_BUFFER_SIZE = (unsigned long)(4) * 1024;

        // ��ʶ��������ֵ
        bool bRet = false;

        unsigned int rwCount = 0;
        
        // ��ʱ��ʼ����
        clock_t clockBegin;
        // ��ʱ��������
        clock_t clockEnd; 

        // ����д������
        char* pWriteBuffer = 0;
        

        // ����һ��д������
        pWriteBuffer = CreateRandomBuffer(WRITE_BUFFER_SIZE);
        if (0 == pWriteBuffer)
        {
            bRet = false;
            m_testState.Error = DST_ALLOC_MEMORY_ERROR;
            m_testState.ErrorMsg = ErrorMsgList[DST_ALLOC_MEMORY_ERROR];
            goto SAFE_EXIT;
        }

        // ����д����
        clockBegin = clock();
        clockEnd = clock();
        rwCount = 0;
        while ((clockEnd - clockBegin) < (long)m_testTime)
        {
            rwCount++;

            // 4K * 256K = 1G
            unsigned long randAddr = RandValueFrom256K();
            SetFilePointer(m_testFileHandle, randAddr * WRITE_BUFFER_SIZE, 0, FILE_BEGIN);

            unsigned long count = 0;
            BOOL ret = WriteFile(m_testFileHandle, (LPCVOID)pWriteBuffer, WRITE_BUFFER_SIZE, &count, NULL);

            if (FALSE == ret)
            {
                bRet = false;
                m_testState.Error = DST_WRITE_FILE_ERROR;
                m_testState.ErrorMsg = ErrorMsgList[DST_WRITE_FILE_ERROR];
                GetWindowsError(&m_testState.ErrorMsgWindows);

                goto SAFE_EXIT;
            }

            if (m_bStopTest)
            {
                bRet = false;
                m_testState.Error = DST_TEST_ABORT_ERROR;
                m_testState.ErrorMsg = ErrorMsgList[DST_TEST_ABORT_ERROR];
                goto SAFE_EXIT;
            }

            clockEnd = clock();

            // ����ʱ��, �������, ����д���ٶ�
            float time = (float)(clockEnd-clockBegin);
            time = time/1000.0f;
            m_testState.WriteSpeed = (float)WRITE_BUFFER_SIZE/1024.0f/1024.0f * rwCount/time;
        }

        bRet = true;

SAFE_EXIT:
        if (0 != pWriteBuffer)
        {
            delete[] pWriteBuffer;
            pWriteBuffer = 0;
        }

        return bRet;
    }

    /// @brief 4K���������
    /// �������������20��
    bool Disk4KRandomReadTest()
    {
        // ��������4K
        const unsigned long  READ_BUFFER_LEN = (unsigned long)(4) * 1024;

        // ��ʶ��������ֵ
        bool bRet = false;

        unsigned int rwCount = 0;

        // ��ʱ��ʼ����
        clock_t clockBegin;
        // ��ʱ��������
        clock_t clockEnd; 

        // �����������
        char* pReadBuffer = 0;


        // ����һ����������
        pReadBuffer = CreateRandomBuffer(READ_BUFFER_LEN);
        if (0 == pReadBuffer)
        {
            bRet = false;
            m_testState.Error = DST_ALLOC_MEMORY_ERROR;
            m_testState.ErrorMsg = ErrorMsgList[DST_ALLOC_MEMORY_ERROR];
            goto SAFE_EXIT;
        }

        // ���ж�����
        clockBegin = clock();
        clockEnd = clock();
        rwCount = 0;
        while ((clockEnd - clockBegin) < (long)m_testTime)
        {
            rwCount++;

            // 4K * 256K = 1G
            unsigned long randAddr = RandValueFrom256K();
            SetFilePointer(m_testFileHandle, randAddr * READ_BUFFER_LEN, 0, FILE_BEGIN);

            unsigned long count = 0;
            BOOL ret = ReadFile(m_testFileHandle, (LPVOID)pReadBuffer, READ_BUFFER_LEN, &count, NULL);

            if (FALSE == ret)
            {
                bRet = false;
                m_testState.Error = DST_READ_FILE_ERROR;
                m_testState.ErrorMsg = ErrorMsgList[DST_READ_FILE_ERROR];
                GetWindowsError(&m_testState.ErrorMsgWindows);

                goto SAFE_EXIT;
            }

            if (m_bStopTest)
            {
                bRet = false;
                m_testState.Error = DST_TEST_ABORT_ERROR;
                m_testState.ErrorMsg = ErrorMsgList[DST_TEST_ABORT_ERROR];
                goto SAFE_EXIT;
            }

            clockEnd = clock();

            // ����ʱ��, �������, �����ȡ�ٶ�
            float time = (float)(clockEnd-clockBegin);
            time = time/1000.0f;
            m_testState.ReadSpeed = (float)READ_BUFFER_LEN/1024.0f/1024.0f * rwCount/time;
        }

        bRet = true;

SAFE_EXIT:
        if (0 != pReadBuffer)
        {
            delete[] pReadBuffer;
            pReadBuffer = 0;
        }

        return bRet;
    }

    /// @brief �����ļ�
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GenerateTestFile()
    {
        // 512KB
        const unsigned long WRITE_BUFFER_SIZE = 512 * 1024;

        bool bRet = false; // ��������ֵ
        char* pWriteBuffer = 0; // д������

        // �����ļ�, �ر��ļ��������Զ�ɾ���ļ�, ��������رպ�, Ҳ��ɾ�������ļ�
        m_testFileHandle = CreateFileW(
            m_testFile.Path.c_str(), 
            GENERIC_WRITE | GENERIC_READ, 
            0, 
            NULL, 
            CREATE_ALWAYS, 
            FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH | FILE_FLAG_DELETE_ON_CLOSE, 
            NULL);
        if (0 == m_testFileHandle || INVALID_HANDLE_VALUE == m_testFileHandle)
        {
            m_testState.Error = DST_OPEN_FILE_ERROR;
            m_testState.ErrorMsg = ErrorMsgList[DST_OPEN_FILE_ERROR];
            GetWindowsError(&m_testState.ErrorMsgWindows);
            bRet = false;

            goto SAFE_EXIT;
        }

        // ��ʼ��д�뻺����
        pWriteBuffer = CreateRandomBuffer(WRITE_BUFFER_SIZE);
        if (0 == pWriteBuffer)
        {
            bRet = false;
            m_testState.Error = DST_ALLOC_MEMORY_ERROR;
            m_testState.ErrorMsg = ErrorMsgList[DST_ALLOC_MEMORY_ERROR];
            goto SAFE_EXIT;
        }

       
        // ���ɲ����ļ�
        for (unsigned long long i = 0; i < m_testFile.Size/WRITE_BUFFER_SIZE; i++)
        {
            unsigned long count = 0;
            BOOL ret = WriteFile(m_testFileHandle, (LPCVOID)pWriteBuffer, WRITE_BUFFER_SIZE, &count, NULL);
            if (FALSE == ret)
            {
                bRet = false;
                m_testState.Error = DST_WRITE_FILE_ERROR;
                m_testState.ErrorMsg = ErrorMsgList[DST_WRITE_FILE_ERROR];
                GetWindowsError(&m_testState.ErrorMsgWindows);

                goto SAFE_EXIT;
            }

            if (m_bStopTest)
            {
                bRet = false;
                m_testState.Error = DST_TEST_ABORT_ERROR;
                m_testState.ErrorMsg = ErrorMsgList[DST_TEST_ABORT_ERROR];
                goto SAFE_EXIT;
            }

        }

        bRet = true;

SAFE_EXIT:
        if (0 != pWriteBuffer)
        {
            delete[] pWriteBuffer;
            pWriteBuffer = 0;
        }

        return bRet;
    }

    /// @brief ɾ�������ļ�
    void DeleteTestFile()
    {
        if (m_testFileHandle != 0 && m_testFileHandle != INVALID_HANDLE_VALUE)
        {
            CloseHandle(m_testFileHandle);
            m_testFileHandle = 0;
        }
    }

private:
    unsigned long m_testTime; ///< ������Ե�ʱ��, ��λ����
    LDiskSpeedTestState m_testState; ///< ����״̬
    LDiskSpeedTestFile m_testFile; ///< �����ļ�
    bool m_bStopTest; ///< ��ʶ�Ƿ�ֹͣ����

    HANDLE m_testFileHandle; ///< �����ļ����

private:
     /// @brief 4K�������
    /// @param[in] pParam ����
    static void Disk4KRandomTestThread(void* pParam)
    {
        /*
        �����㷨:
        ����512KB�ĵ�λ�ߴ�����1GB��С�Ĳ����ļ�,
        Ȼ�������ַ��Χ�ڽ������4KB��λ�ߴ����д�뼰��ȡ����
        */

        CDisk4KRandomTest* pDisk4KRandomTest = (CDisk4KRandomTest*)pParam;
        LDiskSpeedTestState& testState = pDisk4KRandomTest->m_testState;

        
        // ���ɲ����ļ�
        bool bRet = pDisk4KRandomTest->GenerateTestFile();
        if (!bRet)
            goto SAFE_EXIT;

        // ����д����
        bRet = pDisk4KRandomTest->Disk4KRandomWriteTest();
        if (!bRet)
            goto SAFE_EXIT;

        // ���ж�����
        bRet = pDisk4KRandomTest->Disk4KRandomReadTest();
        if (!bRet)
            goto SAFE_EXIT;

SAFE_EXIT:

        // ���ò���״̬
        testState.TestDone = true;

        // ɾ�������ļ�
        pDisk4KRandomTest->DeleteTestFile();


    }
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

LDiskSpeedTestState LDisk4KRandomTest::GetState()
{
    return m_pDisk4KRandomTest->GetState();
}

void LDisk4KRandomTest::Stop()
{
    m_pDisk4KRandomTest->Stop();
}