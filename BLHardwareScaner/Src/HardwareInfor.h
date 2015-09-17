

#ifndef _HARDWAREINFOR_H_
#define _HARDWAREINFOR_H_

#include <string>
using std::string;
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
enum COMPUTER_TYPE
{
    COMPUTER_TYPE_UNKNOWN = 0, ///< 未知
    COMPUTER_TYPE_DESKTOP = 1, ///< 台式机
    COMPUTER_TYPE_NOTE_BOOK = 2, ///< 笔记本
    COMPUTER_TYPE_TABLET = 3 ///< 平板电脑
};

/// @brief 计算机系统信息结构
struct ComputerSystemInfor
{
    wstring ModelName; ////< 机种名
    wstring Manufacturer; ///< 制造商
    COMPUTER_TYPE Type; ///< 计算机类型
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
struct MotherBoardInfor
{
    wstring ProductName; ///< 产品名
    wstring Manufacturer; ///< 制造商
    wstring SerialNumber; ///< 序列号

    wstring BiosSerialNumber; ///< Bios序列号
    wstring BiosVersion; ///< Bios版本
    wstring BiosReleaseDate; ///< Bios发布日期
    wstring BiosVendor; ///< Bios厂商
};

/// @brief 处理器信息
struct ProcessorInfor
{
    wstring Name; ///< 名称
    wstring Description; ///< 描述
    wstring Manufacturer; ///< 制造商
    unsigned long CoresNumber; ///< 核心数
    unsigned long LogicalProcessorNumber; ///< 逻辑处理器数量
    unsigned long MaxClockSpeed; ///< 最大频率(MHz)
};

/// @brief 显卡类型
enum DISPLAY_CARD_TYPE
{
    DISPLAY_CARD_UNKNOWN = 0, ///< 未知
    DISPLAY_CARD_INTERNAL, ///< 集成显卡
    DISPLAY_CARD_EXTERNAL ///< 独立显卡
};

/// @brief 最大显卡数量
#define MAX_DISPLAYCARD_NUMBER 4

/// @brief 显卡信息组
struct VideoCardInforArray
{
    unsigned long Count; ///< 显卡数量
    wstring Description[MAX_DISPLAYCARD_NUMBER]; ///< 显卡描述
    DISPLAY_CARD_TYPE Type[MAX_DISPLAYCARD_NUMBER]; ///< 显卡类型
    unsigned long RAMSize[MAX_DISPLAYCARD_NUMBER]; ///< 显存大小, 单位(M)
};

/// @brief 最大物理内存数量
#define MAX_PHYSICAL_MEMORY_NUMBER 8

/// @brief 内存信息组
struct PhysicalMemoryInforArray
{
    unsigned long Count; ///< 内存数量
    wstring Manufacturer[MAX_PHYSICAL_MEMORY_NUMBER]; ///< 制造商
    wstring SerialNumber[MAX_PHYSICAL_MEMORY_NUMBER]; ///< 序列号
    wstring PartNumbe[MAX_PHYSICAL_MEMORY_NUMBER]; ///< 型号
    unsigned long Capacity[MAX_PHYSICAL_MEMORY_NUMBER]; ///< 内存容量, 单位(M)
    unsigned long Speed[MAX_PHYSICAL_MEMORY_NUMBER]; ///< 内存频率
};

/// @brief 最大磁盘驱动器数量
#define MAX_DISK_NUMBER 8

/// @brief 磁盘类型
enum DISK_TYPE
{
    UNKNOWN_DISK = 0, // 未知类型
    FIXED_IDE_DISK = 1, // 固定磁盘(如本地硬盘)
    EXTERNAL_USB_DISK = 2, // 扩展磁盘(如USB移动硬盘)
    VIRTUAL_DISK = 3, // 虚拟硬盘(如VHD)
    USB_FLASH_DISK = 4, // U盘
    SD_CARD_DISK = 5// SD卡
};

/// @brief 磁盘信息组
struct DiskInforArray 
{
    unsigned long Count; ///< 磁盘数量
    wstring Model[MAX_DISK_NUMBER]; ///< 磁盘型号
    wstring SerialNumber[MAX_DISK_NUMBER]; ///< 磁盘序列号
    unsigned long TotalSize[MAX_DISK_NUMBER]; ///< 磁盘总大小, 单位(G)
    wstring InterfaceType[MAX_DISK_NUMBER]; ///< 磁盘接口类型(SCSI, HDC, IDE, USB 1394)
    DISK_TYPE DiskType[MAX_DISK_NUMBER]; ///< 磁盘类型

    bool IsATA[MAX_DISK_NUMBER]; ///< 标识是否为ATA接口硬盘
    struct  
    {
        unsigned long RotationRate; ///< 磁盘转速, 如果值为1表示为固态硬盘(SSD), 0表示获取失败
        unsigned long SATAType; ///< SATA接口类型, 1(SATA1.0 1.5Gb/s), 2(SATA2.0 3.0Gb/s), 3(SATA3.0 6.0Gb/s), 0(获取失败)
        unsigned long PowerOnHours; ///< 通电总时间, 单位hours(小时)
    }ATAInfor[MAX_DISK_NUMBER]; ///< 只有IDE(ATA)接口的硬盘才应该使用该结构中的值
};

/// @brief 最大显示器数量
#define MAX_MONITOR_NUMBER 8

/// @brief 显示器信息组
struct MonitorInforArray
{
    unsigned long Count; ///< 显示器数量
    wstring Name[MAX_MONITOR_NUMBER]; ///< 显示器名称
    wstring Date[MAX_MONITOR_NUMBER]; ////< 显示器生产日期
};

/// @brief 电池静态信息
struct BatteryStaticInfor
{
    bool Exist; ///< 标识是否存在电池
    wstring Name; ///< 电池名
    wstring Manufacturer; ///< 电池制造商
    wstring SerialNumber; ///< 电池序列号

    unsigned long DesignedCapacity; ///< 设计容量, 单位mWh
    unsigned long FullChargedCapacity; ///< 充满电容量. 单位mWh
    unsigned long DesignedVoltage; ///< 设计电压, 单位mV
};

/// @brief 最大网卡数量
#define MAX_NETWORKCARD_NUMBER 16

/// @brief 网卡类型
enum NETWORKCARD_TYPE
{
    UNKNOWN_NETCARD = 0, ///< 未知网卡
    WIFI_NETCARD = 1, ///< 无线网卡
    ETHERNET_NETCARD = 2, ///< 网卡
    BLUETOOTH_NETCARD = 3 ///< 蓝牙卡 
};

/// @brief 网卡信息组
struct NetworkCardInforArray
{
    unsigned long Count; ///< 网卡数量
    NETWORKCARD_TYPE Type[MAX_NETWORKCARD_NUMBER]; ///< 网卡类型
    wstring Name[MAX_NETWORKCARD_NUMBER]; ///< 网卡名称
    wstring Manufacturer[MAX_NETWORKCARD_NUMBER]; ///< 网卡制造商
    wstring MACAddress[MAX_NETWORKCARD_NUMBER]; ///< MAC地址
};

/// @brief 最大网卡数量
#define MAX_CDROMDRIVE_NUMBER 8

/// @brief 光驱信息组
struct CDRomDriveInforArray
{
    unsigned long Count; ///< 光驱数量
    wstring Name[MAX_CDROMDRIVE_NUMBER]; ///< 光驱名称
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
    const MotherBoardInfor& GetMotherBoardInfor() const;

    /// @brief 获取处理器信息
    /// @return 处理器信息
    const ProcessorInfor& GetProcessorInfor() const;

    /// @brief 获取显卡信息
    /// @return 显卡信息
    const VideoCardInforArray& GetVideoCardInfor() const;

    /// @brief 获取物理内存信息
    /// @return 物理内存信息
    const PhysicalMemoryInforArray& GetPhysicalMemoryInfor() const;

    /// @brief 获取磁盘信息
    /// @return 磁盘信息
    const DiskInforArray& GetDiskInfor();

    /// @brief 获取显示器信息
    /// @return 显示器信息
    const MonitorInforArray& GetMonitorInfor();

    /// @brief 获取电池静态信息
    /// @return 电池静态信息
    const BatteryStaticInfor& GetBatteryStaticInfor();

    /// @brief 获取网卡信息
    /// @return 网卡信息
    const NetworkCardInforArray& GetNetworkCardInfor();

    /// @brief 获取光驱信息
    /// @return 光驱信息
    const CDRomDriveInforArray& GetCDRomDriveInfor();

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
    /// @param[out] motherBoardInfor 主板信息
    void ScanMotherBoardInfor(OUT MotherBoardInfor& baseBoardInfor);

    /// @brief 扫描处理器信息
    /// @param[out] processorInfor 存储处理器信息
    void ScanProcessorInfor(OUT ProcessorInfor& processorInfor);

    /// @brief 扫描显卡信息
    /// @param[out] videoCardInfor 存储显卡信息
    void ScanVideoCardInfor(OUT VideoCardInforArray& displayCardInfor);

    /// @brief 扫描显卡信息
    /// @param[out] physicalMemoryInfor 存储物理内存信息
    void ScanPhysicalMemoryInfor(OUT PhysicalMemoryInforArray& physicalMemoryInfor);

    /// @brief 扫描磁盘信息
    /// @param[out] diskInfor 存储磁盘信息
    void ScanDiskInfor(OUT DiskInforArray& diskInfor);

    /// @brief 扫描显示器信息
    /// @param[out] monitorInfor 存储显示器信息
    void ScanMonitorInfor(OUT MonitorInforArray& monitorInfor);

    /// @brief 扫描电池静态信息
    /// @param[out] batteryStaticInfor 存储电池静态信息
    void ScanBatteryStaticInfor(OUT BatteryStaticInfor& batteryStaticInfor);

    /// @brief 扫描网卡信息
    /// @param[out] networkCardInfor 存储网卡信息
    void ScanNetworkCardInfor(OUT NetworkCardInforArray& networkCardInfor);

    /// @brief 扫描光驱信息
    /// @param[out] cdRomDrive 存储光驱信息
    void ScanCDRomDriveInfor(OUT CDRomDriveInforArray& cdRomDriveInfor);

    HardwareInfor(); // 禁止构造
    HardwareInfor(const HardwareInfor&); // 禁止默认拷贝构造函数
    HardwareInfor& operator = (const HardwareInfor&); // 禁止赋值操作符

private:
    ComputerSystemInfor m_computerSystemInfor; ///< 计算机系统信息
    OperatingSystemInfor m_operatingSystemInfor; ///< 操作系统信息
    MotherBoardInfor m_motherBoardInfor; ///< 主板信息
    ProcessorInfor m_processorInfor; ///< 处理器信息
    VideoCardInforArray m_videoCardInfor; ///< 显卡信息
    PhysicalMemoryInforArray m_physicalMemoryInfor; ///< 物理内存信息
    DiskInforArray m_diskInfor; ///< 磁盘信息
    MonitorInforArray m_monitorInfor; ////< 显示器信息
    BatteryStaticInfor m_batteryStaticInfor; ///< 电池静态信息
    NetworkCardInforArray m_networkCardInfor; ///< 网卡信息
    CDRomDriveInforArray m_cdRomDriveInfor; ///< 光驱信息
};

#endif