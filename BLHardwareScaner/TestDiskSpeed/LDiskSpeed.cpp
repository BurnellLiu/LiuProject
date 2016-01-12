
#include "LDiskSpeed.h"

#include <stdio.h>
#include <ctime>
#include <cstdlib>
#include <process.h>

#include <vector>
using std::vector;

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

        FILE* pFile = 0; // 文件指针

        clock_t clockBegin; // 计时开始
        clock_t clockEnd; // 计时结束

        // 做乘法时遇到整型常量溢出,只需要把第一个乘数强制转换为64位数
        // 测试文件总大小2G
        const unsigned long long TOTAL_SIZE = (unsigned long long)(2)  * 1024 * 1024 * 1024;

        // 分配一个16M的写缓冲区, 并且初始化为10101010...结构
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
        
        // 打开文件
        _wfopen_s(&pFile, pDiskSequenceTest->m_filePath.c_str(), L"wb");
        if (0 == pFile)
        {
            testState.Error = true;
            goto SAFE_EXIT;
        }

        // 进行写测试
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

            // 跳过前10次不稳定的内容
            if (i <= 10)
            {
                continue;
            }
            
            clockEnd = clock();

            // 经历时间, 换算成秒, 计算写入速度
            float time = (float)(clockEnd-clockBegin);
            time = time/1000.0f;
            testState.WriteSpeed = (float)(WRITE_BUFFER_LEN/1024/1024) * (i + 1.0f)/time;

        }

        // 关闭文件然后重新打开
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

        // 进行读测试
        clockBegin = clock();
        for (unsigned long long i = 0; i < TOTAL_SIZE/WRITE_BUFFER_LEN; i++)
        {
            size_t count = fread(pWriteBuffer, WRITE_BUFFER_LEN, 1, pFile);
            if (count != 1)
            {
                testState.Error = true;
                goto SAFE_EXIT;
            }

            // 跳过前10次不稳定的内容
            if (i <= 10)
            {
                continue;
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
        然后在其地址范围内进行随机4KB单位尺寸进行写入及读取测试, 直到跑遍范围为止
        */

        CDisk4KRandomTest* pDisk4KRandomTest = (CDisk4KRandomTest*)pParam;
        LDiskTestState& testState = pDisk4KRandomTest->m_testState;

        FILE* pFile = 0; // 文件指针

        clock_t clockBegin; // 计时开始
        clock_t clockEnd; // 计时结束

        // 做乘法时遇到整型常量溢出,只需要把第一个乘数强制转换为64位数
        // 测试文件总大小1G
        const unsigned long long TOTAL_SIZE = (unsigned long long)(1)  * 1024 * 1024 * 1024;

        // 定义写缓冲区和缓冲区大小
        char* pWriteBuffer = 0;
        const unsigned long long WRITE_BUFFER_LEN = (unsigned long long)(4) * 1024;

        vector<unsigned long long> writeOffsetList; // 文件写入位置偏移值列表

        // 生成测试文件
        bool bRet = GenerateFile(pDisk4KRandomTest->m_filePath, TOTAL_SIZE, 512 * 1024);
        if (!bRet)
        {
            testState.Error = true;
            goto SAFE_EXIT;
        }

        // 分配一个写缓冲区, 并且初始化为01010101...结构
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



        // 打开文件
        _wfopen_s(&pFile, pDisk4KRandomTest->m_filePath.c_str(), L"wb+");
        if (0 == pFile)
        {
            testState.Error = true;
            goto SAFE_EXIT;
        }

        // 初始化写偏移列表, 刚初始化后的偏移列表为顺序的
        writeOffsetList.resize(TOTAL_SIZE/WRITE_BUFFER_LEN);
        for (unsigned long long i = 0; i < writeOffsetList.size(); i++)
        {
            writeOffsetList[i] = i * WRITE_BUFFER_LEN;
        }

        // 打乱顺序
        for (unsigned long long i = 0; i < writeOffsetList.size() * 2; i++)
        {
            
        }

        // 进行写测试
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

            // 跳过前10次不稳定的内容
            if (i <= 10)
            {
                continue;
            }

            clockEnd = clock();

            // 经历时间, 换算成秒, 计算写入速度
            float time = (float)(clockEnd-clockBegin);
            time = time/1000.0f;
            testState.WriteSpeed = (float)(WRITE_BUFFER_LEN/1024/1024) * (i + 1.0f)/time;

        }


SAFE_EXIT:

        // 设置测试状态
        testState.TestDone = true;

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

        // 删除文件
        //_wremove(pDisk4KRandomTest->m_filePath.c_str());
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

        // 生成测试文件
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