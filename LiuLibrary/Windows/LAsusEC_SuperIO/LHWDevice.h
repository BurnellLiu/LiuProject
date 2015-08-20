
#ifndef _LHWDEVICE_H_
#define _LHWDEVICE_H_

#include <Windows.h>

/// @brief Ӳ��CPU�ӿ�
class LHWCpu
{
public:
    /// @brief ��ȡ�¶�
    /// @return CPU�¶�, ����-1��ʾ����
    virtual int GetTemperature() = 0;
};

/// @brief Ӳ��FAN�ӿ�
class LHWFan
{
public:
    /// @brief ��ȡ�ٶ�, ��λRPM
    /// @return �����ٶ�, ����-1��ʾ����
    virtual int GetSpeed() = 0;
};


class LEc;
/// @brief EC�豸����
class LEcDevice
{
public:
    LEcDevice();
    virtual ~LEcDevice();

protected:
    LEc* m_pEC; ///< ECָ��
};

/// @brief ITE EC�豸����
class LIteEcDevice : public LEcDevice
{
public:
    LIteEcDevice();
    virtual ~LIteEcDevice();

protected:
    const BYTE EC_READ; ///< ������
    const BYTE EC_WRITE; ///< д����
    const BYTE EC_READ_ACPI; ///< ��ACPI����
};

/// @brief ENE EC�豸����
class LEneEcDevice : public LEcDevice
{
public:
    LEneEcDevice();
    virtual ~LEneEcDevice();

protected:
    const BYTE EC_READ; ///< ������
    const BYTE EC_WRITE; ///< д����
};


/// @brief ITE CPU��
class LIteEcCpu : public LHWCpu, public LIteEcDevice
{
public:
    virtual int GetTemperature();
};

/// @brief ENE CPU��
class LEneEcCpu : public LHWCpu, public LEneEcDevice
{
public:
    virtual int GetTemperature();
};

/// @brief ITE FAN��
class LIteEcFan : LHWFan, public LIteEcDevice
{
public:
    virtual int GetSpeed();
};

/// @brief ENE FAN��
class LEneFan : LHWFan, public LEneEcDevice
{
public:
    virtual int GetSpeed();
};

#endif