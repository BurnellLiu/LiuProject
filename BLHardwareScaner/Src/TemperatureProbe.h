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

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

class LCpuTemperature;

/// @brief 温度探测类
class TemperatureProbe
{
public:
    TemperatureProbe();
    ~TemperatureProbe();

    /// @brief 获取CPU温度
    ///  
    /// 多核CPU仅返回第一个核心的温度
    /// @return CPU温度, 0(获取失败)
    unsigned int GetCpuTemp();

    /// @brief 获取GPU温度
    ///  
    /// @return GPU温度, 0(获取失败)
    unsigned int GetGpuTemp();
private:
    LCpuTemperature* m_pCpuTemperature;
};


#endif