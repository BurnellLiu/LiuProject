
#include "HardwareInfor.h"

#include "Wmi/LWMISystemClasses.h"



HardwareInfor& HardwareInfor::GetInstance()
{
    static HardwareInfor s_instance;
    static bool s_bScanDone = false; // 标识是否完成扫描硬件信息

    if (!s_bScanDone)
    {
        s_instance.Scan();
        s_bScanDone = true;
    }

    return s_instance;
}

HardwareInfor::HardwareInfor()
{

}

HardwareInfor::~HardwareInfor()
{

}

const ComputerSystemInfor& HardwareInfor::GetComputerSystemInfor() const
{
    return m_computerSystemInfor;
}

void HardwareInfor::Scan()
{
    this->ScanComputerSystemInfor(m_computerSystemInfor);
}

void HardwareInfor::ScanComputerSystemInfor(OUT ComputerSystemInfor& computerSystemInfor)
{
    computerSystemInfor.Type = UNKNOWN;

    LWMI::LComputerSystemManager computerSytemManager;
    computerSytemManager.GetComputerSystemManufacturer(0, computerSystemInfor.Manufacturer);
    LWMI::LComputerSystemManager::LPC_SYSTEM_TYPE pcType;
    computerSytemManager.GetComputerSystemPCType(0, pcType);
    computerSystemInfor.Type = (PC_SYSTEM_TYPE)pcType;
    computerSytemManager.GetComputerSystemModel(0, computerSystemInfor.ModelName);
}