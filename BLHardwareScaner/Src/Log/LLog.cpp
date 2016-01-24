
#include "LLog.h"

#include <cstdio>
#include <cstdlib>
#include <stdarg.h>
#include <ctime>
#include <cstring>

#include <map>
using std::map;

#include <Windows.h>

/// @brief �ֶ�����
class CManuLock
{
public:
    /// @brief ���캯��
    CManuLock()
    {
        InitializeCriticalSection(&m_criSection);
    }

    /// @brief ��������
    ~CManuLock()
    {
        DeleteCriticalSection(&m_criSection);
    }

    /// @brief ����
    void Lock()
    {
        EnterCriticalSection(&m_criSection);
    }

    /// @brief ����
    void UnLock()
    {
        LeaveCriticalSection(&m_criSection);
    }

private:
    CManuLock(const CManuLock&);
    void operator = (const CManuLock&);

    CRITICAL_SECTION m_criSection; ///< �ٽ�������
};

/// @brief �Զ�����
class CAutoLock
{
public:
    /// @brief ���캯�� 
    /// @param[in] pLock �ֶ�������
    CAutoLock(IN CManuLock* pLock)
    {
        if (pLock != 0)
        {
            m_pManuLock = pLock;
            m_pManuLock->Lock();
        }
        
    }

    /// @brief ��������
    ~CAutoLock()
    {
        if (m_pManuLock != 0)
        {
            m_pManuLock->UnLock();
        }
    }

private:
    CAutoLock(const CAutoLock& );
    void operator = (const CAutoLock&);

    CManuLock* m_pManuLock; ///< �ֶ�������
};

/// @brief ����LOG��
/// ʹ�ø���дLOG�����ֱ��д��Ӳ����, �������д�������е����
class CDebugLog
{
public:
    /// @brief ���캯��
    CDebugLog()
    {
        m_hFile = NULL;
        m_pWriteBuffer = 0;
        m_bufferUsedSize = 0;
        m_bufferTotalSize = 512;

        m_fillChar = '\0';
    }

    /// @brief��������
    ~CDebugLog()
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

        // Ԥ�����ļ�ͷд��UNICODE��־��
        char unicode[2] = {0};
        unicode[0] = (char)0xFF;
        unicode[1] = (char)0xFE;
        this->Write(unicode, 2);

        return true;
    }

    /// @brief ��ӡһ��LOG
    /// @param[in] wstr LOG��Ϣ
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool PrintLine(IN const wstring& wstr)
    {
        CAutoLock lock(&m_fileLock);
        this->Write(wstr.c_str(), wstr.length() * 2);
        return this->Write(L"\r\n", 4);
    }

    /// @brief �ر��ļ�
    void Close()
    {
        this->Clear();
    }

private:
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

    CManuLock m_fileLock; ///< �ļ�������, ���ڶ��߳�
};

namespace LLog
{
   

    /// @brief LOG���Խṹ
    struct SLogProperty 
    {
        CDebugLog* DebugLog; ///< �ļ�ָ��
        bool ShowThreadId; ///< ��ʶ�Ƿ���ʾ�߳�Id
        bool ShowTime; ///< ��ʶ�Ƿ���ʾ��ǰʱ��
    };

    static SLogProperty s_logProperty = 
    {
        0,
        false,
        false
    };

    /// @brief ת��Ϊ���ֽ��ַ���
    /// @param[in] strSrc Դ�ַ���
    /// @param[in] strDest �洢ת������ַ���
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool StringToWString(const string& strSrc, wstring& strDest)
    {
        int nSize = MultiByteToWideChar(CP_ACP, 0, strSrc.c_str(), strSrc.length(), 0, 0);  
        if(nSize <= 0)
            return false;  
        wchar_t* pwszDst = new wchar_t[nSize+1];

        int iRet = MultiByteToWideChar(CP_ACP, 0, strSrc.c_str(), strSrc.length(), pwszDst, nSize);

        pwszDst[nSize] = 0; 

        strDest.clear();
        strDest.assign(pwszDst);

        delete[] pwszDst;

        return true;
    }

    /// @brief ��ȡ������Ϣ
    void GetAssistInfor(OUT wstring* pWStr)
    {
        if (pWStr == 0)
            return;
        pWStr->clear();

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


            wchar_t threadId[64] = {0};
            wprintf_s(threadId, L"[Thread %u]", s_threadIdMap[dwId]);
            pWStr->append(threadId);
        }


        if (s_logProperty.ShowTime)
        {
            // LOG�����ӵ�ǰʱ��
            time_t t = time(0);
            wchar_t szTime[64] = {0}; 
            tm tmTemp;
            localtime_s(&tmTemp, &t);
            wcsftime(szTime, sizeof(szTime), L"[%H:%M:%S]",&tmTemp );
            pWStr->append(szTime);
        }
    }

    bool Open(IN const wchar_t* szFileName)
    {
        if (s_logProperty.DebugLog != 0)
        {
            delete s_logProperty.DebugLog;
            s_logProperty.DebugLog = 0;
        }

        s_logProperty.DebugLog = new CDebugLog();
        bool bRet = s_logProperty.DebugLog->Open(szFileName);

        return bRet;
    }

    void Close()
    {
        if (s_logProperty.DebugLog != 0)
        {
            delete s_logProperty.DebugLog;
            s_logProperty.DebugLog = 0;
        }
    }

    void ShowThreadId(IN bool bFlag)
    {
        s_logProperty.ShowThreadId = bFlag;
    }

    void ShowTime(IN bool bFlag)
    {
        s_logProperty.ShowTime = bFlag;
    }

    void WriteLineW(IN const wchar_t* szFormat, ...)
    {
        if (s_logProperty.DebugLog == 0)
            return;

        va_list args;
        va_start(args, szFormat);
        wchar_t printfBuffer[1024] = {0};
        vswprintf_s(printfBuffer, szFormat, args);
        va_end(args);

        wstring message;
        GetAssistInfor(&message);
        message.append(printfBuffer);

        s_logProperty.DebugLog->PrintLine(message);
    }

    void WriteLineA(IN const char* szFormat, ...)
    {
        if (s_logProperty.DebugLog == 0)
            return;

        va_list args;
        va_start(args, szFormat);
        char printfBuffer[1024] = {0};
        vsprintf_s(printfBuffer, szFormat, args);
        va_end(args);

        wstring logMessage;
        GetAssistInfor(&logMessage);

        wstring message;
        StringToWString(printfBuffer, message);

        logMessage += message;
        s_logProperty.DebugLog->PrintLine(logMessage);
    }
}