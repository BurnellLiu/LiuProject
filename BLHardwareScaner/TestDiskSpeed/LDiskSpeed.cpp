
#include "LDiskSpeed.h"

#include <stdio.h>
#include <ctime>
#include <process.h>

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

        _beginthread(SequenceTest, 0, (void*)this);

        return true;
    }

    /// @brief 获取测试状态
    LSpeedTestState& GetState()
    {
        return m_testState;
    }
private:
    /// @brief 顺序测试
    /// @param[in] pParam 参数
    static void SequenceTest(void* pParam)
    {
        FILE* pFile = 0; // 文件指针

        clock_t clockBegin; // 计时开始
        clock_t clockEnd; // 计时结束

        CDiskSequenceTest* pDiskSequenceTest = (CDiskSequenceTest*)pParam;
        LSpeedTestState& testState = pDiskSequenceTest->m_testState;

        // 分配一个16M的写缓冲区, 并且初始化为10101010...结构
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
        
        // 打开文件
        _wfopen_s(&pFile, pDiskSequenceTest->m_filePath.c_str(), L"wb");
        if (0 == pFile)
        {
            testState.Error = true;
            goto SAFE_EXIT;
        }

        clockBegin = clock();
        // 总共写入128 * 16M = 1G的内容
        for (unsigned int i = 0; i < 128; i++)
        {
            size_t count = fwrite(pWriteBuffer, WRITE_BUFFER_LEN, 1, pFile);
            fflush(pFile);
            if (1 != count)
            {
                testState.Error = true;
                break;
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
    LSpeedTestState m_testState; ///< 测试状态
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

LSpeedTestState LDiskSequenceTest::GetState()
{
    return m_pDiskSequenceTest->GetState();
}