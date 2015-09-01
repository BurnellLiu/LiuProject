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


/// @brief CPU温度接口
class LCpuTemperature
{
public:
    /// @brief 获取温度接口
    ///  
    /// @return 温度, 0(获取失败, 不支持)
    virtual unsigned int Get() = 0;
};

/// @brief IntelCPU温度类
class LIntelCpuTemperature : public LCpuTemperature
{
public:
    LIntelCpuTemperature();
    ~LIntelCpuTemperature();

    /// @brief 获取温度接口
    ///  
    /// @return 温度, 0(获取失败, 不支持)
    virtual unsigned int Get();
private:
    bool m_bInitWinRing0Success; ///< 标识是否初始化WinRing0成功
};





#endif