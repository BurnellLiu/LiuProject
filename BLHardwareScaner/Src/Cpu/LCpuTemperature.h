/// @file LCpuTemperature.h
/// @brief 该头文件中声明了CPU温度获取类
/// 
/// 该头文件申明的类关联WinRing0, 并且需要管理员权限
/// Detail:
/// @author Burnell_Liu  
/// @version   
/// @date 1:9:2015

#ifndef _LCPUTEMPERATURE_H_
#define _LCPUTEMPERATURE_H_

// 最大处理器核心数
#define MAX_PROCESSOR_CORE_NUMBER 64 


/// @brief CPU温度接口
class LCpuTemperature
{
public:
    /// @brief 获取温度
    ///  
    /// 每个处理器核心的温度值按顺序存储在数组中, 数组中的值为0表示没有该处理器核心
    /// @param[out] temp[MAX_PROCESSOR_CORE_NUMBER] 存储温度值(单位摄氏度), 
    /// @return 成功返回true, 失败返回false
    virtual bool Get(unsigned long temp[MAX_PROCESSOR_CORE_NUMBER]) = 0;

protected:
    /// @brief 获取处理器核心数
    /// @param[out] physicalCoreNumber 物理核心数
    /// @param[out] logicalProcessorNumber 逻辑处理器数量
    /// @return 成功返回true, 失败返回false
    bool GetProcessorCoreNumber(unsigned long& physicalCoreNumber, unsigned long& logicalProcessorNumber);
};

/// @brief IntelCPU温度类
class LIntelCpuTemperature : public LCpuTemperature
{
public:
    LIntelCpuTemperature();
    ~LIntelCpuTemperature();

    /// @brief 获取温度
    ///  
    /// 每个处理器核心的温度值按顺序存储在数组中, 数组中的值为0表示没有该处理器核心
    /// @param[out] temp[MAX_PROCESSOR_CORE_NUMBER] 存储温度值(单位摄氏度), 
    /// @return 成功返回true, 失败返回false
    virtual bool Get(unsigned long temp[MAX_PROCESSOR_CORE_NUMBER]);
private:
    bool m_bInitWinRing0Success; ///< 标识是否初始化WinRing0成功
};





#endif