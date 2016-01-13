
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

/// @brief 产生随机整数
/// @param[in] min 随机整数的最小值
/// @param[in] max 随机整数的最大值
/// @return 随机整数
static int RandInt(int min, int max) 
{
    return rand()%(max - min + 1) + min;
}

/// @brief 创建随机缓冲区
/// 缓冲区中将被随机填充值
/// @param[in] bufferSize 缓冲区大小
/// @return 缓冲区指针
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

/// @brief 随机产生256K范围内的值
/// @return 随机值范围[0, 256K-1]
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



/// @brief 磁盘顺序测试
class CDiskSequenceTest
{
public:

    /// @brief 构造函数
    CDiskSequenceTest()
    {
        m_testState.Error = false;
        m_testState.TestDone = true;
        m_testState.ReadSpeed = 0.0f;
        m_testState.WriteSpeed = 0.0f;
    }

    /// @brief 析构函数
    ~CDiskSequenceTest()
    {

    }

    /// @brief 开始测试
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

    /// @brief 获取测试状态
    LDiskTestState& GetState()
    {
        return m_testState;
    }
private:
    /// @brief 顺序测试
    /// @param[in] pParam 参数
    static void DiskSequenceTest(void* pParam)
    {
        /*
        测试算法:
        先以16M的尺寸为单位, 持续向受测分区写入生产1个达到2GB大小的文件,
        然后再以同样的单位尺寸读取
        */

        CDiskSequenceTest* pDiskSequenceTest = (CDiskSequenceTest*)pParam;
        LDiskTestState& testState = pDiskSequenceTest->m_testState;

        HANDLE hFile = 0; // 文件句柄

        clock_t clockBegin; // 计时开始
        clock_t clockEnd; // 计时结束

        // 做乘法时遇到整型常量溢出,只需要把第一个乘数强制转换为64位数
        // 测试文件总大小2G
        const unsigned long long TOTAL_SIZE = (unsigned long long)(2)  * 1024 * 1024 * 1024;

        // 分配一个16M的写缓冲区, 并且初始化为10101010...结构
        const unsigned long long WRITE_BUFFER_LEN = (unsigned long long)(16) * 1024 * 1024;

        char* pWriteBuffer = CreateRandomBuffer(WRITE_BUFFER_LEN);
        if (0 == pWriteBuffer)
        {
            testState.Error = true;
            goto SAFE_EXIT;
        }
        
        // 创建文件
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

        // 进行写测试
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

            // 经历时间, 换算成秒, 计算写入速度
            float time = (float)(clockEnd-clockBegin);
            time = time/1000.0f;
            testState.WriteSpeed = (float)(WRITE_BUFFER_LEN/1024/1024) * (i + 1.0f)/time;

        }

        // 进行读测试
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

            // 经历时间, 换算成秒, 计算读取速度
            float time = (float)(clockEnd-clockBegin);
            time = time/1000.0f;
            testState.ReadSpeed = (float)(WRITE_BUFFER_LEN/1024/1024) * (i + 1.0f)/time;
        }
        

SAFE_EXIT:

        // 设置测试状态
        testState.TestDone = true;

        // 释放资源
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

        // 删除文件
        _wremove(pDiskSequenceTest->m_filePath.c_str());
    }

private:
    LDiskTestState m_testState; ///< 测试状态
    wstring m_filePath; ///< 测试文件路径
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

    /// @brief 构造函数
    CDisk4KRandomTest()
    {
        m_testState.Error = false;
        m_testState.TestDone = true;
        m_testState.ReadSpeed = 0.0f;
        m_testState.WriteSpeed = 0.0f;
    }

    /// @brief 析构函数
    ~CDisk4KRandomTest()
    {

    }

    /// @brief 开始测试
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

    /// @brief 获取测试状态
    LDiskTestState& GetState()
    {
        return m_testState;
    }
private:
    /// @brief 4K随机测试
    /// @param[in] pParam 参数
    static void Disk4KRandomTest(void* pParam)
    {
        /*
        测试算法:
        先以512KB的单位尺寸生成1GB大小的测试文件,
        然后在其地址范围内进行随机4KB单位尺寸进行写入及读取测试
        */

        CDisk4KRandomTest* pDisk4KRandomTest = (CDisk4KRandomTest*)pParam;
        LDiskTestState& testState = pDisk4KRandomTest->m_testState;
        wstring& filePath = pDisk4KRandomTest->m_filePath;

        // 做乘法时遇到整型常量溢出,只需要把第一个乘数强制转换为64位数
        // 测试文件总大小1G
        const unsigned long TOTAL_SIZE = (unsigned long)(1)  * 1024 * 1024 * 1024;


        // 生成测试文件
        bool bRet = GenerateFile(filePath, TOTAL_SIZE, 512 * 1024);
        if (!bRet)
        {
            testState.Error = true;
            goto SAFE_EXIT;
        }

        // 进行写测试
        bRet = Disk4KRandomWriteTest(filePath, &testState.WriteSpeed);
        if (!bRet)
        {
            testState.Error = true;
            goto SAFE_EXIT;
        }

        // 进行读测试
        bRet = Disk4KRandomReadTest(filePath, &testState.ReadSpeed);
        if (!bRet)
        {
            testState.Error = true;
            goto SAFE_EXIT;
        }

SAFE_EXIT:

        // 设置测试状态
        testState.TestDone = true;

        // 删除文件
        _wremove(filePath.c_str());
    }

    /// @brief 4K随机写测试
    /// 请确保输入的文件大小不小于1G, 该项测试最多进行20秒
    /// @param[in] filePath 文件路径
    /// @param[out] pSpeed 存储写速度
    static bool Disk4KRandomWriteTest(IN const wstring& filePath, OUT float* pSpeed)
    {
        const unsigned long TEST_TIME = 20000; // 单位ms
        const unsigned long  WRITE_BUFFER_LEN = (unsigned long)(4) * 1024;


        // 标识函数返回值
        bool bRet = false;

        HANDLE hFile = NULL;
        unsigned int rwCount = 0;
        
        // 计时开始变量
        clock_t clockBegin;
        // 计时结束变量
        clock_t clockEnd; 

        // 定义写缓冲区
        char* pWriteBuffer = 0;

        if ( 0 == pSpeed)
        {
            bRet = false;
            goto SAFE_EXIT;
        }
        

        // 分配一个写缓冲区
        pWriteBuffer = CreateRandomBuffer(WRITE_BUFFER_LEN);
        if (0 == pWriteBuffer)
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        // 打开文件
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

        // 进行写测试
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

            // 经历时间, 换算成秒, 计算写入速度
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

    /// @brief 4K随机读测试
    /// 请确保输入的文件大小不小于1G, 该项测试最多进行20秒
    /// @param[in] filePath 文件路径
    /// @param[out] pSpeed 存储读速度
    static bool Disk4KRandomReadTest(IN const wstring& filePath, OUT float* pSpeed)
    {
        const unsigned long TEST_TIME = 20000; // 单位ms
        const unsigned long  READ_BUFFER_LEN = (unsigned long)(4) * 1024;


        // 标识函数返回值
        bool bRet = false;

        HANDLE hFile = NULL;
        unsigned int rwCount = 0;

        // 计时开始变量
        clock_t clockBegin;
        // 计时结束变量
        clock_t clockEnd; 

        // 定义读缓冲区
        char* pReadBuffer = 0;

        if ( 0 == pSpeed)
        {
            bRet = false;
            goto SAFE_EXIT;
        }


        // 分配一个读缓冲区
        pReadBuffer = CreateRandomBuffer(READ_BUFFER_LEN);
        if (0 == pReadBuffer)
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        // 打开文件
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

        // 进行读测试
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

            // 经历时间, 换算成秒, 计算写入速度
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

    /// @brief 生成文件
    /// 单位尺寸值必须可以被文件大小整除
    /// @param[in] filePath 文件路径
    /// @param[in] fileSize 文件大小
    /// @param[in] elemSize 生成文件的单位尺寸
    /// @return 成功返回true, 失败返回false
    static bool GenerateFile(
        IN const wstring& filePath, 
        IN unsigned long long fileSize, 
        IN unsigned int elemSize)
    {
        bool bRet = false; // 函数返回值
        FILE* pFile = 0; // 文件指针
        char* pWriteBuffer = 0; // 写缓冲区

        // 打开文件
        _wfopen_s(&pFile, filePath.c_str(), L"wb");
        if (0 == pFile)
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        // 初始化写入缓冲区
        pWriteBuffer = CreateRandomBuffer(elemSize);
        if (0 == pWriteBuffer)
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        // 生成测试文件
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

        // 释放资源
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
    LDiskTestState m_testState; ///< 测试状态
    wstring m_filePath; ///< 测试文件路径
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