
#include "LLog.h"

#include <cstdio>
#include <cstdlib>
#include <stdarg.h>
#include <ctime>
#include <cstring>

#include <map>
using std::map;

#include <Windows.h>

/// @brief ����LOG�ļ���
class CDebugLogFile
{
public:
    /// @brief ���캯��
    CDebugLogFile()
    {
        m_hFile = NULL;
        m_pWriteBuffer = 0;
        m_bufferUsedSize = 0;
        m_bufferTotalSize = 512;

        m_fillChar = '\n';
    }

    /// @brief��������
    ~CDebugLogFile()
    {
        this->Clear();
    }

    /// @brief ���ļ�
    /// @param[in] fileName �ļ���
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool Open(IN const wstring& fileName)
    {
        this->Clear();

        // �����ļ�
        m_hFile = CreateFileW(
            fileName.c_str(), 
            GENERIC_READ | GENERIC_WRITE, 
            0, 
            NULL, 
            CREATE_ALWAYS, 
            FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH, 
            NULL);
        if (0 == m_hFile || INVALID_HANDLE_VALUE == m_hFile)
        {
            this->Clear();
            return false;
        }

        m_pWriteBuffer = new unsigned char[m_bufferTotalSize];
        if (0 == m_pWriteBuffer)
        {
            this->Clear();
            return false;
        }

        memset(m_pWriteBuffer, m_fillChar, m_bufferTotalSize);

        return true;
    }

    /// @brief д�ļ�
    /// @param[in] pBuffer д������
    /// @param[in] size д��������С
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool Write(IN const void* pBuffer, unsigned int size)
    {
        if (NULL == m_hFile || 
            INVALID_HANDLE_VALUE == m_hFile)
            return false;

        if (0 == pBuffer ||
            0 == size)
            return false;

        unsigned long count = 0;
        BOOL iRet = FALSE;

        if ((m_bufferUsedSize + size) < m_bufferTotalSize)
        {
            memcpy(m_pWriteBuffer + m_bufferUsedSize, pBuffer, size);

            iRet = WriteFile(m_hFile, (LPCVOID)m_pWriteBuffer, m_bufferTotalSize, &count, NULL);
            if (FALSE == iRet)
            {
                memset(m_pWriteBuffer + m_bufferUsedSize, m_fillChar, size);
                return false;
            }

            SetFilePointer(m_hFile, -1 * (long)m_bufferTotalSize, 0, FILE_CURRENT);
            m_bufferUsedSize += size;
            return true;
        }

        if ((m_bufferUsedSize + size) == m_bufferTotalSize)
        {
            memcpy(m_pWriteBuffer + m_bufferUsedSize, pBuffer, size);

            iRet = WriteFile(m_hFile, (LPCVOID)m_pWriteBuffer, m_bufferTotalSize, &count, NULL);
            if (FALSE == iRet)
            {
                memset(m_pWriteBuffer + m_bufferUsedSize, m_fillChar, size);
                return false;
            }

            m_bufferUsedSize = 0;
            memset(m_pWriteBuffer, m_fillChar, m_bufferTotalSize);

            return true;
        }

        if ((m_bufferUsedSize + size) > m_bufferTotalSize)
        {
            unsigned int newWriteCount = m_bufferTotalSize-m_bufferUsedSize;
            memcpy(m_pWriteBuffer + m_bufferUsedSize, pBuffer, newWriteCount);
            iRet = WriteFile(m_hFile, (LPCVOID)m_pWriteBuffer, m_bufferTotalSize, &count, NULL);
            if (FALSE == iRet)
            {
                memset(m_pWriteBuffer + m_bufferUsedSize, m_fillChar, newWriteCount);
                return false;
            }

            m_bufferUsedSize = 0;
            memset(m_pWriteBuffer, m_fillChar, m_bufferTotalSize);

            void* pNewBuffer = (char*)pBuffer + newWriteCount;
            return this->Write(pNewBuffer, size-newWriteCount);

        }

        return false;
    }

    /// @brief �ر��ļ�
    void Close()
    {
        this->Clear();
    }

private:
    /// @brief ������Դ
    void Clear()
    {
        if (NULL != m_hFile &&
            INVALID_HANDLE_VALUE != m_hFile)
        {
            CloseHandle(m_hFile);
            m_hFile = 0;
        }

        if (0 != m_pWriteBuffer)
        {
            delete[] m_pWriteBuffer;
            m_pWriteBuffer = 0;
        }

        m_bufferUsedSize = 0;
    }

private:
    HANDLE m_hFile; ///< �ļ����

    char m_fillChar; ///< ����ַ�
    unsigned char* m_pWriteBuffer; ///< д������
    unsigned int m_bufferUsedSize; ///< ���������ѱ�ʹ�õĴ�С
    unsigned int m_bufferTotalSize; ///< �������ܴ�С
};

/// @brief LOG���Խṹ
struct SLogProperty 
{
    CDebugLogFile* LogFile; ///< �ļ�ָ��
    bool ShowThreadId; ///< ��ʶ�Ƿ���ʾ�߳�Id
    bool ShowTime; ///< ��ʶ�Ƿ���ʾ��ǰʱ��
};

static SLogProperty s_logProperty = 
{
    0,
    false,
    false
};

/// @brief ��LOG�����Ӹ�����Ϣ
static void AddAssistInforToLog()
{
    if (s_logProperty.LogFile == 0)
        return;

    if (s_logProperty.ShowThreadId)
    {
        static map<DWORD, DWORD> s_threadIdMap;
        // LOG�����ӵ�ǰ�߳�ID, �����ڶ��߳������ֲ�ͬ�̵߳�LOG
        DWORD dwId = GetCurrentThreadId();
        if (s_threadIdMap.count(dwId) == 0)
        {
            s_threadIdMap[dwId] = 0;
            s_threadIdMap[dwId] = s_threadIdMap.size();
        }
   

        char threadId[64] = {0};
        sprintf_s(threadId, "[Thread %u]", s_threadIdMap[dwId]);
        s_logProperty.LogFile->Write(threadId, strlen(threadId));
    }
    

    if (s_logProperty.ShowTime)
    {
        // LOG�����ӵ�ǰʱ��
        time_t t = time(0);
        char szTime[64] = {0}; 
        tm tmTemp;
        localtime_s(&tmTemp, &t);
        strftime(szTime, sizeof(szTime), "[%H:%M:%S]",&tmTemp );
        s_logProperty.LogFile->Write(szTime, strlen(szTime));
    }
}

bool LLog::Open(IN const wchar_t* szFileName)
{
    if (s_logProperty.LogFile != 0)
    {
        delete s_logProperty.LogFile;
        s_logProperty.LogFile = 0;
    }

    s_logProperty.LogFile = new CDebugLogFile();
    bool bRet = s_logProperty.LogFile->Open(szFileName);

    return bRet;
}

void LLog::Close()
{
    if (s_logProperty.LogFile != 0)
    {
        delete s_logProperty.LogFile;
        s_logProperty.LogFile = 0;
    }
}

void LLog::ShowThreadId(IN bool bFlag)
{
    s_logProperty.ShowThreadId = bFlag;
}

void LLog::ShowTime(IN bool bFlag)
{
    s_logProperty.ShowTime = bFlag;
}

void LLog::WriteLineW(IN const wchar_t* szFormat, ...)
{
    if (s_logProperty.LogFile == 0)
        return;

    AddAssistInforToLog();

    va_list args;
    va_start(args, szFormat);
    wchar_t printfBuffer[1024] = {0};
    vswprintf_s(printfBuffer, szFormat, args);
    va_end(args);

    s_logProperty.LogFile->Write(printfBuffer, wcslen(printfBuffer) * 2);
    s_logProperty.LogFile->Write(L"\r\n", 4);
}

void LLog::WriteLineA(IN const char* szFormat, ...)
{
    if (s_logProperty.LogFile == 0)
        return;

    AddAssistInforToLog();

    va_list args;
    va_start(args, szFormat);
    char printfBuffer[1024] = {0};
    vsprintf_s(printfBuffer, szFormat, args);
    va_end(args);

    s_logProperty.LogFile->Write(printfBuffer, strlen(printfBuffer));
    s_logProperty.LogFile->Write("\r\n", 2);
}