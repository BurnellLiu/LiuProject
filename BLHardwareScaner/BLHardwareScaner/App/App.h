

#ifndef _APP_H_
#define _APP_H_

/// @brief 程序启动模式
enum APP_START_MODE
{
    APP_NORMAL = 0, ///< 标准启动模式
    APP_RESTARTAGING ///< 重开机Aging启动模式
};

/// @brief 程序运行模式类
class LAppStartMode
{
public:
    /// @brief 获取程序启动模式
    /// @return 程序启动模式
    static APP_START_MODE Get();

private:
    /// @brief 初始化程序启动模式
    /// @param[in] argc 命令行参数数目
    /// @param[in] argv[] 命令行参数
    static void Init(int argc, char *argv[]);

    static APP_START_MODE sAppRunMode; ///< 程序启动模式

    friend int main(int argc, char *argv[]); ///< 设置main函数为友元
};

#endif