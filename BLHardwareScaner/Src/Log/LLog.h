

#ifndef _LLOG_H_
#define _LLOG_H_

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

#include <string>
using std::wstring;
using std::string;

/// @brief LOG类
class LLog
{
public:
    /// @brief 打开LOG档, 如果不存在则创建, 如果已经存在那么原始内容将被销毁
    /// @param[in] strFileName LOG档文件名
    /// @return 成功返回true, 失败返回false
    static bool Open(IN const string& strFileName);

    /// @brief 关闭LOG档
    static void Close();

    /// @brief 写一行LOG, 自动换行
    /// @param[in] strFormart 需要写入LOG的格式化字符串
    static void WriteLine(IN const string& strFormat, ...);

    /// @brief 写一行LOG, 自动换行
    /// @param[in] strFormat 需要写入LOG的格式化字符串
    static void WriteLineW(IN const wstring& strFormat, ...);
};

#endif