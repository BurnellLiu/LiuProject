/// @file TemperatureProbe.h
/// @brief 该头文件中声明了温度探测类
/// 
/// 该头文件申明的类关联WinRing0, 并且需要管理员权限
/// Detail:
/// @author Burnell_Liu  
/// @version   
/// @date 1:9:2015

#ifndef _TEMPERATUREPROBE_H_
#define _TEMPERATUREPROBE_H_

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

/// @brief 最大获取磁盘温度数量
#define MAX_DISKTEMP_NUMBER 8

/// @brief 磁盘温度信息组
struct DiskTempInforArray
{
    unsigned int Count; ///< 成功获取到温度的磁盘的数量
    unsigned int Temp[MAX_DISKTEMP_NUMBER]; ///< 磁盘温度, 单位摄氏度(C)
    wstring DiskDriveID[MAX_DISKTEMP_NUMBER]; ///< 磁盘驱动器ID, 如 L"\\\\.\\PhysicalDrive0"
};

/// @brief 最大处理器物理核心数
#ifndef MAX_PROCESSOR_PHYSICAL_CORE_NUM
#define MAX_PROCESSOR_PHYSICAL_CORE_NUM 64
#endif

/// @brief CPU温度信息
struct CpuTempInfor
{
    unsigned int CoreNum; ///< CPU物理核心数
    unsigned int CoreTemp[MAX_PROCESSOR_PHYSICAL_CORE_NUM]; ///< CPU核心温度
};

/// @brief 温度探测类
class TemperatureProbe
{
public:
    TemperatureProbe();
    ~TemperatureProbe();

    /// @brief 获取CPU温度
    /// @param[out] OUT cpuTemp 存储CPU温度信息
    /// @return 成功返回true, 失败返回false
    bool GetCpuTemp(OUT CpuTempInfor& cpuTemp);

    /// @brief 获取GPU温度
    /// @return GPU温度, 0(获取失败)
    unsigned int GetGpuTemp();

    /// @brief 获取磁盘温度
    /// @param[out] diskTemp 存储磁盘温度
    void GetDiskTemp(OUT DiskTempInforArray& diskTemp);
};


#endif