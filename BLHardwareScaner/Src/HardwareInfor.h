

#ifndef _HARDWAREINFOR_H_
#define _HARDWAREINFOR_H_

#include <string>
using std::wstring;

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

/// @brief 获取计算机类型
enum PC_SYSTEM_TYPE
{
    UNKNOWN = 0, ///< 未知
    DESKTOP = 1, ///< 台式机
    NOTE_BOOK = 2, ///< 笔记本
    TABLET = 3 ///< 平板电脑
};

/// @brief 计算机系统信息结构
struct ComputerSystemInfor
{
    wstring ModelName; ////< 机种名
    PC_SYSTEM_TYPE Type; ///< 计算机类型
    wstring Manufacturer; ///< 制造商
    
};

/// @brief 硬件信息类
///
/// 该类获取的都是固定信息
class HardwareInfor
{
public:
    /// @brief 获取实例对象
    /// @return 实例对象
    static HardwareInfor& GetInstance();

    /// @brief 获取计算机系统信息
    /// @return 计算机系统信息
    const ComputerSystemInfor& GetComputerSystemInfor() const; 

    /// @brief 析构函数
    ~HardwareInfor();

private:
    /// @brief 扫描硬件信息
    void Scan();
    
    /// @brief 扫描计算机系统信息
    /// @param[in] computerSystemInfor 存储计算机系统信息
    void ScanComputerSystemInfor(OUT ComputerSystemInfor& computerSystemInfor);

    HardwareInfor(); // 禁止构造
    HardwareInfor(const HardwareInfor&); // 禁止默认拷贝构造函数
    HardwareInfor& operator = (const HardwareInfor&); // 禁止赋值操作符

private:
    ComputerSystemInfor m_computerSystemInfor; ///< 计算机系统信息
};

#endif