
#include <cstdio>
#include <Windows.h>

#include "..\\Src\\Wmi\\LWMIHardwareClasses.h"
#include "..\\src\\Wmi\\LWMISystemClasses.h"

#include "..\\\Src\LCPUID.h"

int main()
{
    setlocale(LC_CTYPE, "");

    wstring name;
    wstring description;
    wstring manufacturer;
    wstring serialNumber;
    wstring version;
    wstring strArchitecture;
    wstring model;
    unsigned long maxClockSpeed;
    unsigned long coresNum;
    unsigned long logicalProcessorsNum;

    LWMI::LBaseBoardManager baseBoardManager;
    wprintf(L"BaseBoard:\n");
    description.clear();
    baseBoardManager.GetBaseBoardDescription(0, description);
    wprintf(L"Description: %s\n", description.c_str());
    manufacturer.clear();  
    baseBoardManager.GetBaseBoardManufacturer(0, manufacturer);
    wprintf(L"Manufacturer: %s\n", manufacturer.c_str());
    serialNumber.clear();
    baseBoardManager.GetBaseBoardSerialNumber(0, serialNumber);
    wprintf(L"SerialNumber: %s\n", serialNumber.c_str());
    wprintf(L"\n");

    LWMI::LBIOSManager biosManager;
    wprintf(L"BIOS:\n");
    description.clear();
    biosManager.GetBIOSDescription(0, description);
    wprintf(L"Description: %s\n", description.c_str());
    manufacturer.clear();
    biosManager.GetBIOSManufacturer(0, manufacturer);
    wprintf(L"Manufacturer: %s\n", manufacturer.c_str());
    serialNumber.clear();
    biosManager.GetBIOSSerialNumber(0, serialNumber);
    wprintf(L"SerialNumber: %s\n", serialNumber.c_str());
    version.clear();
    biosManager.GetBIOSVersion(0, version);
    wprintf(L"Version: %s\n", version.c_str());
    version.clear();
    biosManager.GetSMBIOSBIOSVersion(0, version);
    wprintf(L"SMBIOSVersion: %s\n", version.c_str());
    wprintf(L"\n");

    LWMI::LProcessorManager processorManager;
    wprintf(L"Processor:\n");
    name.clear();
    processorManager.GetProcessorName(0, name);
    wprintf(L"Name: %s\n", name.c_str());
    description.clear();
    processorManager.GetProcessorDesription(0, description);
    wprintf(L"Description: %s\n", description.c_str());
    LWMI::LProcessorManager::LPROCESSOR_ARCHITECTURE architecture;
    processorManager.GetProcessorArchitecture(0, architecture);
    strArchitecture.clear();
    if (architecture == 9)
    {
        strArchitecture = L"X64";
    }
    if (architecture == 0)
    {
        strArchitecture = L"X86";
    }
    wprintf(L"Architecture: %s\n", strArchitecture.c_str());

    manufacturer.clear();
    processorManager.GetProcessorManufacturer(0, manufacturer);
    wprintf(L"Manufacturer: %s\n", manufacturer.c_str());
    processorManager.GetProcessorMaxClockSpeed(0, maxClockSpeed);
    wprintf(L"MaxClockSpeed: %uMHz\n", maxClockSpeed);
    processorManager.GetProcessorCoresNumber(0, coresNum);
    wprintf(L"CoresNumber: %u\n", coresNum);
    processorManager.GetProcessorLogicalProcessorNumber(0, logicalProcessorsNum);
    wprintf(L"LogicalProcessorsNumber: %u\n", logicalProcessorsNum);
    wprintf(L"\n");

    LWMI::LPhysicalMemoryManager memoryManager;
    for (int i = 0; i < memoryManager.GetPhysicalMemoryCount(); i++)
    {
         wprintf(L"PhysocalMemory: %d\n", i);
         description.clear();
         memoryManager.GetPhysicalMemoryDescription(i, description);
         wprintf(L"Description: %s\n", description.c_str());
         manufacturer.clear();
         memoryManager.GetPhysicalMemoryManufacturer(i, manufacturer);
         wprintf(L"Manufacturer: %s\n", manufacturer.c_str());
         serialNumber.clear();
         memoryManager.GetPhysicalMemorySerialNumber(i, serialNumber);
         wprintf(L"SerialNumber: %s\n", serialNumber.c_str());
         unsigned long capacity;
         memoryManager.GetPhysicalMemoryCapacity(i, capacity);
         wprintf(L"Capacity: %uM\n", capacity);
         wprintf(L"\n");
    }

    LWMI::LVideoControllerManager videoControllerManager;
    for (int i = 0; i < videoControllerManager.GetVideoControllerCount(); i++)
    {
        wprintf(L"VideoController: %d\n", i);
        description.clear();
        videoControllerManager.GetVideoControllerDescription(i, description);
        wprintf(L"Description: %s\n", description.c_str());
        unsigned long capacity;
        videoControllerManager.GetVideoControllerAdapterRAMCapacity(i, capacity);
        wprintf(L"Capacity: %uM\n", capacity);
        wprintf(L"\n");
    }

    LWMI::LComputerSystemManager computerSystemManager;
    wprintf(L"ComputerSystem: \n");
    model.clear();
    computerSystemManager.GetComputerSystemModel(0, model);
    wprintf(L"ModelName: %s\n", model.c_str());
    manufacturer.clear();
    computerSystemManager.GetComputerSystemManufacturer(0, manufacturer);
    wprintf(L"Manufacturer: %s\n", manufacturer.c_str());
    LWMI::LComputerSystemManager::LPC_SYSTEM_TYPE pcSystemType;
    computerSystemManager.GetComputerSystemPCType(0, pcSystemType);
    wstring strPCSystemType;
    if (pcSystemType == LWMI::LComputerSystemManager::DESKTOP)
    {
        strPCSystemType = L"Desktop";
    }
    if (pcSystemType == LWMI::LComputerSystemManager::NOTE_BOOK)
    {
        strPCSystemType = L"NoteBook";
    }
    if (pcSystemType == LWMI::LComputerSystemManager::TABLET)
    {
        strPCSystemType = L"Tablet";
    }
    wprintf(L"PCSystemType: %s\n", strPCSystemType.c_str());
    wstring systemType;
    computerSystemManager.GetComputerSystemType(0, systemType);
    wprintf(L"SystemType: %s\n", systemType.c_str());
    wprintf(L"\n");

    LWMI::LOperatingSystemManager osManager;
    wprintf(L"OperatingSystem:\n");
    wstring caption;
    osManager.GetOSCaption(0, caption);
    wprintf(L"Caption: %s\n", caption.c_str());
    strArchitecture.clear();
    osManager.GetOSArchitecture(0, strArchitecture);
    wprintf(L"Architecture: %s\n", strArchitecture.c_str());
    version.clear();
    osManager.GetOSVersion(0, version);
    wprintf(L"Version: %s\n", version.c_str());
    wstring systemDrive;
    osManager.GetOSSystemDrive(0, systemDrive);
    wprintf(L"SystemDrive: %s\n", systemDrive.c_str());
    wprintf(L"\n");

    LCPUID cpuid;
    printf("CPUID:\n");
    string cpuVendor;
    cpuid.GetVendor(cpuVendor);
    printf("Vendor: %s\n", cpuVendor.c_str());
    string cpuBrand;
    cpuid.GetBrand(cpuBrand);
    printf("Brand: %s\n", cpuBrand.c_str());
    system("pause");
    return 0;
}