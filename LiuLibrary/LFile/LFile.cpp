
#include "LFile.h"

#include <io.h>

//-----------------------------------------------------------------------
// 安全删除数组对象宏宏
//-----------------------------------------------------------------------
#ifndef SAFE_DELETE_ARR
#define SAFE_DELETE_ARR(p) \
do\
{\
    if ((p) != NULL)\
    { \
        delete [] p;\
        p = NULL; \
    }\
}while(0)
#endif

// 安全关闭文件宏
#ifndef SAFE_CLOSE_FILE
#define SAFE_CLOSE_FILE(p) \
do\
{\
    if ((p) != NULL)\
    { \
    fclose(p);\
    p = NULL; \
    }\
}while(0)
#endif

bool LFile::Create(IN const char* strFileName)
{
      FILE* pFile = NULL;
      int ret = fopen_s(&pFile, strFileName, "w");
      if (ret == 0)
      {
          SAFE_CLOSE_FILE(pFile);
          return true;
      }
      else
      {
          return false;
      }
}

LFile::LFile(IN const char* strFileName)
{
    m_pFile = NULL;

    if (strFileName != NULL)
        m_strFileName = strFileName;
}

LFile::~LFile()
{
    SAFE_CLOSE_FILE(m_pFile);
}

bool LFile::Exist()
{
    return _access(m_strFileName.c_str(), FILE_OK) == 0 ? true : false;
}

bool LFile::Readable()
{
    return _access(m_strFileName.c_str(), READ_OK) == 0 ? true : false;
}

bool LFile::Writable()
{
    return _access(m_strFileName.c_str(), WRITW_OK) == 0 ? true : false;
}

void LFile::Close()
{
    SAFE_CLOSE_FILE(m_pFile);
}

LFileReader::LFileReader(IN const char* strFileName)
    : LFile(strFileName)
{
    m_bAtEnd = false;
    m_readBufferLen = 256;
    m_readBuffer = new char[m_readBufferLen];
}

LFileReader::~LFileReader()
{
    SAFE_DELETE_ARR(m_readBuffer);
}

bool LFileReader::Open()
{
    SAFE_CLOSE_FILE(m_pFile);
    int ret = fopen_s(&m_pFile, m_strFileName.c_str(), "r");

    m_bAtEnd = false;

    if (ret == 0)
        return true;
    else
        return false;
}

bool LFileReader::AtEnd()
{
    if (m_pFile == NULL)
        return true;

    return m_bAtEnd;
}

void LFileReader::ReadLine(OUT string& line)
{
    if (m_pFile== NULL)
        return;

    for (int i = 0; i < m_readBufferLen; i++)
        m_readBuffer[i] = '\0';
    line.clear();

    char currentCh; // 当前字符
    int charNum = 0;
    do 
    {
        currentCh = fgetc(m_pFile);
        if (currentCh == EOF)
        {
            m_bAtEnd = true;
            break;
        }
        if (currentCh == '\n')
            break;
        
        
        if (charNum == m_readBufferLen-1)
        {
            line.append(m_readBuffer);
            for (int i = 0; i < m_readBufferLen; i++)
                m_readBuffer[i] = '\0';
            charNum = 0;
           
        }

         m_readBuffer[charNum++] = currentCh;
       
    } while (true);

    line.append(m_readBuffer);
}

LFileWriter::LFileWriter(IN const char* strFileName)
    : LFile(strFileName)
{
    m_mode = NEW;
}

LFileWriter::~LFileWriter()
{

}

void LFileWriter::SetMode(IN WRITE_MODE mode)
{
    m_mode = mode;
}

bool LFileWriter::Open()
{
    if (!Exist())
        return false;

    SAFE_CLOSE_FILE(m_pFile);

    int ret;
    switch (m_mode)
    {
    case APPEND:
         ret = fopen_s(&m_pFile, m_strFileName.c_str(), "a");
        break;
    case NEW:
        ret = fopen_s(&m_pFile, m_strFileName.c_str(), "w");
        break;
    default:
        ret = fopen_s(&m_pFile, m_strFileName.c_str(), "w");
        break;
    }

    if (ret == 0)
        return true;
    else
        return false;

}

void LFileWriter::WriteLine(IN const char* strWriteBuffer)
{
    if (m_pFile == NULL || strWriteBuffer == NULL)
        return;

    fprintf(m_pFile, "%s\n", strWriteBuffer);
}

void LFileWriter::Flush()
{
    if (m_pFile == NULL)
        return;

    fflush(m_pFile);
}