#ifndef _LEC_H_
#define _LEC_H_

#include <Windows.h>

/// @brief 该类仅用于初始化WinRing0
///
/// 在本文件中该类实例已被创建
class LWinRing0
{
public:
    LWinRing0();
    ~LWinRing0();
public:
    /// @brief 判断WinRing0是否初始化成功
    /// @return 初始化成功true, 初始化失败返回false 
    static bool IsReady()
    {
        return sm_bInitSuccess;
    }
private:
    static bool sm_bInitSuccess; ///< 初始化WinRing0成功
};

/// @brief EC 基类
class LEc
{
public:
    LEc();
    virtual ~LEc();

    /// @brief 对命令端口进行写
    /// @param[in] command 命令
    void WriteCommandPortByte(BYTE command);

    /// @brief 对命令端口进行读
    /// @brief 读取的数据
    BYTE ReadCommandPortByte();

    /// @brief 对数据端口进行写
    /// @param[in] data 写入的数据
    void WriteDataPortByte(BYTE data);

    /// @brief 对数据端口进行读
    /// @return 读取的数据
    BYTE ReadDataPortByte();

    /// @brief 判断输入缓冲区状态
    /// @return true 输入缓冲区准备就绪, false 输入缓冲区未就绪
    bool IBFReady();

    /// @brief 判断输出缓冲区状态
    /// @return true 输出缓冲区准备就绪, false 输出缓冲区未就绪
    bool OBFReady();

protected:
    WORD EC_SC; ///<  读此端口, 对应状态寄存器; 写操作时对应命令寄存器
    WORD EC_DATA; ///< 对应数据寄存器
};

/// @brief Asus ENE EC
class LAsusEneEc : public LEc
{
public:
    LAsusEneEc();
    virtual ~LAsusEneEc();
};

/// @brief Asus ITE EC
class LAsusIteEc : public LEc
{
public:
    LAsusIteEc();
    virtual ~LAsusIteEc();
};





#endif