

/// Author:Burnell_Liu Email:674288799@qq.com Date: 2014/08/13
/// Description: 文件操作
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
    /// @brief 创建一个新文件
    ///
    /// 如果文件已存在则删除它再新建一个文件
    /// @param[in] strFileName 文件名
    /// @return 创建成功返回true, 创建失败返回false
    static bool Create(IN const char* strFileName);

public:
    explicit LFile(IN const char* strFileName);
    ~LFile();

    /// @brief 判断文件是否存在
    /// @return 存在返回true, 不存在返回false
    bool Exist();

    /// @brief 判断文件是否可读
    /// @return 文件可读返回true, 文件不可读返回false
    bool Readable();

    /// @brief 判断文件是否可写
    /// @return 文件可写返回true, 文件不可写返回false
    bool Writable();

    /// @brief 打开文件
    ///
    /// 要求文件存在, 否则打开失败
    /// @return 成功打开返回true,打开失败返回false
    virtual bool Open() = 0;

    /// @brief 关闭文件
    void Close();

protected:
    FILE* m_pFile; ///< 文件指针
    string m_strFileName; ///< 文件名

private:
    /// @brief 文件权限
    enum FILE_PERMISSION
    {
        FILE_OK = 0, ///< 文件存在
        RUN_OK = 1, ///< 执行权限
        WRITW_OK = 2, ///< 写权限
        READ_OK = 4, ///< 读权限
        WR_OK = 6 ///< 读写权限
    };
};

class LFileReader : public LFile
{
public:
    LFileReader(IN const char* strFileName);
    ~LFileReader();

    virtual bool Open();

    /// @brief 文件到达尾部
    /// @return 文件到达尾部返回true, 否则返回false
    bool AtEnd();

    /// @brief 读取一行(不包含换行符)
    void ReadLine(OUT string& line);

private:
    int m_readBufferLen; ///< 读取字符缓冲区长度
    char* m_readBuffer; ///< 读取字符缓冲区
    bool m_bAtEnd; ///< 文件到达尾部
};

class LFileWriter : public LFile
{
public:
    /// @brief 写文件模式
    enum WRITE_MODE
    {
        APPEND = 0, ///< 追加模式(不删除原始数据, 打开后定位到文件末尾)
        NEW ///< 新建模式(删除原始数据)
    };
    LFileWriter(IN const char* strFileName);
    ~LFileWriter();

    /// @brief 设置写文件模式
    /// 
    /// 默认为新建模式
    /// @param[in] mode
    void SetMode(IN WRITE_MODE mode);

    virtual bool Open();

    /// @brief 写数据到文件中, 字符串后自动添加\n
    /// @param[in] strWriteBuffer
    void WriteLine(IN const char* strWriteBuffer);

    /// @brief 刷新, 将缓冲区中的内容写入文件
    void Flush();

private:
    WRITE_MODE m_mode; ///< 写文件模式
};
#endif