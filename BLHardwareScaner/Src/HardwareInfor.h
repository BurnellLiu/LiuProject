

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

/// @brief 操作系统信息结构
struct OperatingSystemInfor
{
    wstring Caption; ////< 标题
    wstring Architecture; ////< 架构
    wstring Version; ////< 版本
    wstring SystemDrive; ////< 系统盘
};

/// @brief 主板信息结构
struct BaseBoardInfor
{
    wstring Description; ///< 描述
    wstring Manufacturer; ///< 制造商
    wstring SerialNumber; ///< 序列号
};

/// @brief 处理器信息
struct ProcessorInfor
{
    wstring Name; ///< 名称
    wstring Description; ///< 描述
    wstring Manufacturer; ///< 制造商
    unsigned long CoresNumber; ///< 核心数
    unsigned long LogicalProcessorNumber; ///< 逻辑处理器数量
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

    /// @brief 获取操作系统信息
    /// @return 操作系统信息
    const OperatingSystemInfor& GetOperatingSystemInfor() const;

    /// @brief 获取主板信息
    /// @return 主板信息
    const BaseBoardInfor& GetBaseBoardInfor() const;

    /// @brief 获取处理器信息
    /// @return 处理器信息
    const ProcessorInfor& GetProcessorInfor() const;

    /// @brief 析构函数
    ~HardwareInfor();

private:
    /// @brief 扫描硬件信息
    void Scan();
    
    /// @brief 扫描计算机系统信息
    /// @param[out] computerSystemInfor 存储计算机系统信息
    void ScanComputerSystemInfor(OUT ComputerSystemInfor& computerSystemInfor);

    /// @brief 扫描操作系统信息
    /// @param[out] operatingSystemInfor 操作系统信息
    void ScanOperatingSystemInfor(OUT OperatingSystemInfor& operatingSystemInfor);

    /// @brief 扫描主板信息
    /// @param[out] baseBoardInfor 主板信息
    void ScanBaseBoardInfor(OUT BaseBoardInfor& baseBoardInfor);

    /// @brief 扫描处理器信息
    /// @param[out] processorInfor 存储处理器信息
    void ScanProcessorInfor(OUT ProcessorInfor& processorInfor);

    HardwareInfor(); // 禁止构造
    HardwareInfor(const HardwareInfor&); // 禁止默认拷贝构造函数
    HardwareInfor& operator = (const HardwareInfor&); // 禁止赋值操作符

private:
    ComputerSystemInfor m_computerSystemInfor; ///< 计算机系统信息
    OperatingSystemInfor m_operatingSystemInfor; ///< 操作系统信息
    BaseBoardInfor m_baseBoardInfor; ///< 主板信息
    ProcessorInfor m_processorInfor; ///< 处理器信息
};

#endif