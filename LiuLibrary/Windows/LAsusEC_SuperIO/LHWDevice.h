
#ifndef _LHWDEVICE_H_
#define _LHWDEVICE_H_

#include <Windows.h>

/// @brief 硬件CPU接口
class LHWCpu
{
public:
    /// @brief 获取温度
    /// @return CPU温度, 返回-1表示出错
    virtual int GetTemperature() = 0;
};

/// @brief 硬件FAN接口
class LHWFan
{
public:
    /// @brief 获取速度, 单位RPM
    /// @return 风扇速度, 返回-1表示出错
    virtual int GetSpeed() = 0;
};


class LEc;
/// @brief EC设备基类
class LEcDevice
{
public:
    LEcDevice();
    virtual ~LEcDevice();

protected:
    LEc* m_pEC; ///< EC指针
};

/// @brief ITE EC设备基类
class LIteEcDevice : public LEcDevice
{
public:
    LIteEcDevice();
    virtual ~LIteEcDevice();

protected:
    const BYTE EC_READ; ///< 读命令
    const BYTE EC_WRITE; ///< 写命令
    const BYTE EC_READ_ACPI; ///< 读ACPI命令
};

/// @brief ENE EC设备基类
class LEneEcDevice : public LEcDevice
{
public:
    LEneEcDevice();
    virtual ~LEneEcDevice();

protected:
    const BYTE EC_READ; ///< 读命令
    const BYTE EC_WRITE; ///< 写命令
};


/// @brief ITE CPU类
class LIteEcCpu : public LHWCpu, public LIteEcDevice
{
public:
    virtual int GetTemperature();
};

/// @brief ENE CPU类
class LEneEcCpu : public LHWCpu, public LEneEcDevice
{
public:
    virtual int GetTemperature();
};

/// @brief ITE FAN类
class LIteEcFan : LHWFan, public LIteEcDevice
{
public:
    virtual int GetSpeed();
};

/// @brief ENE FAN类
class LEneFan : LHWFan, public LEneEcDevice
{
public:
    virtual int GetSpeed();
};

#endif