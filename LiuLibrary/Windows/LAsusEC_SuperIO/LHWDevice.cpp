#include "LHWDevice.h"

#include "LEC.h"



LEcDevice::LEcDevice()
{
    m_pEC = 0;
}

LEcDevice::~LEcDevice()
{

}

LIteEcDevice::LIteEcDevice()
    : EC_READ(0x80), EC_WRITE(0x81), EC_READ_ACPI(0x90)
{
    m_pEC = new LAsusIteEc();
}

LIteEcDevice::~LIteEcDevice()
{
    if (m_pEC != 0)
    {
        delete m_pEC;
        m_pEC = 0;
    }
}

LEneEcDevice::LEneEcDevice()
    : EC_READ(0x80), EC_WRITE(0x81)
{
    m_pEC = new LAsusEneEc();
}

LEneEcDevice::~LEneEcDevice()
{
    if (m_pEC != 0)
    {
        delete m_pEC;
        m_pEC = 0;
    }
}

int LIteEcCpu::GetTemperature()
{
    if (!LWinRing0::IsReady())
        return -1;

    if (!m_pEC->IBFReady())
        return -1;
    m_pEC->WriteCommandPortByte(EC_READ);

    if (!m_pEC->IBFReady())
        return -1;
    m_pEC->WriteDataPortByte(0x03);

    if (!m_pEC->IBFReady())
        return -1;
    m_pEC->WriteDataPortByte(0xA0);

    if (!m_pEC->OBFReady())
        return -1;
    int temp = (int)m_pEC->ReadDataPortByte();

    return temp;
}

int LIteEcFan::GetSpeed()
{
    if (!LWinRing0::IsReady())
        return -1;

    if (!m_pEC->IBFReady())
        return -1;
    m_pEC->WriteCommandPortByte(EC_READ);

    if (!m_pEC->IBFReady())
        return -1;
    m_pEC->WriteDataPortByte(0x18);

    if (!m_pEC->IBFReady())
        return -1;
    m_pEC->WriteDataPortByte(0x1F);

    if (!m_pEC->OBFReady())
        return -1;
    BYTE high = m_pEC->ReadDataPortByte();

    if (!m_pEC->IBFReady())
        return -1;
    m_pEC->WriteCommandPortByte(EC_READ);

    if (!m_pEC->IBFReady())
        return -1;
    m_pEC->WriteDataPortByte(0x18);

    if (!m_pEC->IBFReady())
        return -1;
    m_pEC->WriteDataPortByte(0x1E);

    if (!m_pEC->OBFReady())
        return -1;
    BYTE low = m_pEC->ReadDataPortByte();

    int tacho = (high << 8) + low;
    if (tacho <= 0)
        return -1;

    int rpm = ( 60 * 71875)/( tacho * 2);
    return rpm;
}