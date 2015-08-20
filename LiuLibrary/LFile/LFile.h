

/// Author:Burnell_Liu Email:674288799@qq.com Date: 2014/08/13
/// Description: �ļ�����
/// 
/// Others: 
/// Function List: 
///
/// History: 
///  1. Date, Author 
///  Modification
///

#ifndef _LFILE_H_
#define _LFILE_H_

#include <cstdio>

#include <string>
using std::string;

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

class LFile
{
public:
    /// @brief ����һ�����ļ�
    ///
    /// ����ļ��Ѵ�����ɾ�������½�һ���ļ�
    /// @param[in] strFileName �ļ���
    /// @return �����ɹ�����true, ����ʧ�ܷ���false
    static bool Create(IN const char* strFileName);

public:
    explicit LFile(IN const char* strFileName);
    ~LFile();

    /// @brief �ж��ļ��Ƿ����
    /// @return ���ڷ���true, �����ڷ���false
    bool Exist();

    /// @brief �ж��ļ��Ƿ�ɶ�
    /// @return �ļ��ɶ�����true, �ļ����ɶ�����false
    bool Readable();

    /// @brief �ж��ļ��Ƿ��д
    /// @return �ļ���д����true, �ļ�����д����false
    bool Writable();

    /// @brief ���ļ�
    ///
    /// Ҫ���ļ�����, �����ʧ��
    /// @return �ɹ��򿪷���true,��ʧ�ܷ���false
    virtual bool Open() = 0;

    /// @brief �ر��ļ�
    void Close();

protected:
    FILE* m_pFile; ///< �ļ�ָ��
    string m_strFileName; ///< �ļ���

private:
    /// @brief �ļ�Ȩ��
    enum FILE_PERMISSION
    {
        FILE_OK = 0, ///< �ļ�����
        RUN_OK = 1, ///< ִ��Ȩ��
        WRITW_OK = 2, ///< дȨ��
        READ_OK = 4, ///< ��Ȩ��
        WR_OK = 6 ///< ��дȨ��
    };
};

class LFileReader : public LFile
{
public:
    LFileReader(IN const char* strFileName);
    ~LFileReader();

    virtual bool Open();

    /// @brief �ļ�����β��
    /// @return �ļ�����β������true, ���򷵻�false
    bool AtEnd();

    /// @brief ��ȡһ��(���������з�)
    void ReadLine(OUT string& line);

private:
    int m_readBufferLen; ///< ��ȡ�ַ�����������
    char* m_readBuffer; ///< ��ȡ�ַ�������
    bool m_bAtEnd; ///< �ļ�����β��
};

class LFileWriter : public LFile
{
public:
    /// @brief д�ļ�ģʽ
    enum WRITE_MODE
    {
        APPEND = 0, ///< ׷��ģʽ(��ɾ��ԭʼ����, �򿪺�λ���ļ�ĩβ)
        NEW ///< �½�ģʽ(ɾ��ԭʼ����)
    };
    LFileWriter(IN const char* strFileName);
    ~LFileWriter();

    /// @brief ����д�ļ�ģʽ
    /// 
    /// Ĭ��Ϊ�½�ģʽ
    /// @param[in] mode
    void SetMode(IN WRITE_MODE mode);

    virtual bool Open();

    /// @brief д���ݵ��ļ���, �ַ������Զ����\n
    /// @param[in] strWriteBuffer
    void WriteLine(IN const char* strWriteBuffer);

    /// @brief ˢ��, ���������е�����д���ļ�
    void Flush();

private:
    WRITE_MODE m_mode; ///< д�ļ�ģʽ
};
#endif