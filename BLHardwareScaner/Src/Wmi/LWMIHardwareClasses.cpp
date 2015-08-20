
#include "LWMIHardwareClasses.h"

#include "LWMICoreManager.h"

namespace LWMI
{
    LBaseBoardManager::LBaseBoardManager()
    {
        m_pWMICoreManager = 0;
        m_pWMICoreManager = new LWMICoreManager();
        bool bRet = m_pWMICoreManager->BaseInit(NAMESPACE_ROOT_CIMV2);
        bRet = m_pWMICoreManager->WQLQuery(L"SELECT * FROM Win32_BaseBoard");
    }

    LBaseBoardManager::~LBaseBoardManager()
    {
        delete m_pWMICoreManager;
    }

    int LBaseBoardManager::GetBaseBoardCount()
    {
        return m_pWMICoreManager->GetObjectsCount();
    }

    bool LBaseBoardManager::GetBaseBoardCaption(IN int index, OUT wstring& caption)
    {
        return this->m_pWMICoreManager->GetStringProperty(index, L"Caption", caption);
    }

    bool LBaseBoardManager::GetBaseBoardDescription(IN int index, OUT wstring& description)
    {
        return this->m_pWMICoreManager->GetStringProperty(index, L"Description", description);
    }

    bool LBaseBoardManager::GetBaseBoardManufacturer(IN int index, OUT wstring& manufacturer)
    {
        return this->m_pWMICoreManager->GetStringProperty(index, L"Manufacturer", manufacturer);
    }

    bool LBaseBoardManager::GetBaseBoardSerialNumber(IN int index, OUT wstring& serialNumber)
    {
        return this->m_pWMICoreManager->GetStringProperty(index, L"SerialNumber", serialNumber);
    }

    LBIOSManager::LBIOSManager()
    {
        m_pWMICoreManager = 0;
        m_pWMICoreManager = new LWMICoreManager();
        bool bRet = m_pWMICoreManager->BaseInit(NAMESPACE_ROOT_CIMV2);
        bRet = m_pWMICoreManager->WQLQuery(L"SELECT * FROM Win32_BIOS");
    }

    LBIOSManager::~LBIOSManager()
    {
        delete m_pWMICoreManager;
    }

    int LBIOSManager::GetBIOSCount()
    {
        return m_pWMICoreManager->GetObjectsCount();
    }

    bool LBIOSManager::GetBIOSDescription(IN int index, OUT wstring& description)
    {
        return this->m_pWMICoreManager->GetStringProperty(index, L"Description", description);
    }

    bool LBIOSManager::GetBIOSManufacturer(IN int index, OUT wstring& manufacturer)
    {
        return this->m_pWMICoreManager->GetStringProperty(index, L"Manufacturer", manufacturer);
    }

    bool LBIOSManager::GetBIOSSerialNumber(IN int index, OUT wstring& serialNumber)
    {
        return this->m_pWMICoreManager->GetStringProperty(index, L"SerialNumber", serialNumber);
    }

    bool LBIOSManager::GetBIOSVersion(IN int index, OUT wstring& version)
    {
        return this->m_pWMICoreManager->GetStringProperty(index, L"Version", version);
    }

    bool LBIOSManager::GetSMBIOSBIOSVersion(IN int index, OUT wstring& version)
    {
        return this->m_pWMICoreManager->GetStringProperty(index, L"SMBIOSBIOSVersion", version);
    }

    LProcessorManager::LProcessorManager()
    {
        m_pWMICoreManager = 0;
        m_pWMICoreManager = new LWMICoreManager();
        bool bRet = m_pWMICoreManager->BaseInit(NAMESPACE_ROOT_CIMV2);
        bRet = m_pWMICoreManager->WQLQuery(L"SELECT * FROM Win32_Processor");
    }

    LProcessorManager::~LProcessorManager()
    {
        delete m_pWMICoreManager;
        m_pWMICoreManager = 0;
    }

    int LProcessorManager::GetProcessorCount()
    {
        return m_pWMICoreManager->GetObjectsCount();
    }

    bool LProcessorManager::GetProcessorName(IN int index, OUT wstring& name)
    {
        return m_pWMICoreManager->GetStringProperty(index, L"Name", name);
    }

    bool LProcessorManager::GetProcessorDesription(IN int index, OUT wstring& description)
    {
        return m_pWMICoreManager->GetStringProperty(index, L"Description", description);
    }

    bool LProcessorManager::GetProcessorArchitecture(IN int index, OUT LPROCESSOR_ARCHITECTURE& architecture)
    {
        LUINT16 temp;
        bool bRet = m_pWMICoreManager->GetUINT16Property(index, L"Architecture", temp);
        if (bRet)
            architecture = (LPROCESSOR_ARCHITECTURE)temp;
        return bRet;
    }

    bool LProcessorManager::GetProcessorManufacturer(IN int index, OUT wstring& manufacturer)
    {
        return m_pWMICoreManager->GetStringProperty(index, L"Manufacturer", manufacturer);
    }

    bool LProcessorManager::GetProcessorMaxClockSpeed(IN int index, OUT unsigned long& speed)
    {
        LUINT temp;
        bool bRet = m_pWMICoreManager->GetUINT32Property(index, L"MaxClockSpeed", temp);
        if (bRet)
            speed = temp;
        return bRet;
    }

    bool LProcessorManager::GetProcessorCoresNumber(IN int index, OUT unsigned long& coresNumber)
    {
        LUINT temp;
        bool bRet = m_pWMICoreManager->GetUINT32Property(index, L"NumberOfCores", temp);
        if (bRet)
            coresNumber = temp;
        return bRet;
    }

    bool LProcessorManager::GetProcessorLogicalProcessorNumber(IN int index, OUT unsigned long& logicalProcessorsNumber)
    {
        LUINT temp;
        bool bRet = m_pWMICoreManager->GetUINT32Property(index, L"NumberOfLogicalProcessors", temp);
        if (bRet)
            logicalProcessorsNumber = temp;
        return bRet;
    }

    LPhysicalMemoryManager::LPhysicalMemoryManager()
    {
        m_pWMICoreManager = 0;
        m_pWMICoreManager = new LWMICoreManager();
        bool bRet = m_pWMICoreManager->BaseInit(NAMESPACE_ROOT_CIMV2);
        bRet = m_pWMICoreManager->WQLQuery(L"SELECT * FROM Win32_PhysicalMemory");
    }

    LPhysicalMemoryManager::~LPhysicalMemoryManager()
    {
        delete m_pWMICoreManager;
        m_pWMICoreManager = 0;
    }

    int LPhysicalMemoryManager::GetPhysicalMemoryCount()
    {
        return m_pWMICoreManager->GetObjectsCount();
    }

    bool LPhysicalMemoryManager::GetPhysicalMemoryDescription(IN int index, OUT wstring& description)
    {
        return m_pWMICoreManager->GetStringProperty(index, L"Description", description);
    }

    bool LPhysicalMemoryManager::GetPhysicalMemoryManufacturer(IN int index, OUT wstring& manufacturer)
    {
        return m_pWMICoreManager->GetStringProperty(index, L"Manufacturer", manufacturer);
    }

    bool LPhysicalMemoryManager::GetPhysicalMemorySerialNumber(IN int index, OUT wstring& serialNumber)
    {
        return m_pWMICoreManager->GetStringProperty(index, L"SerialNumber", serialNumber);
    }

    bool LPhysicalMemoryManager::GetPhysicalMemoryCapacity(IN int index, OUT unsigned long& capacity)
    {
        LUINT64 temp;
        bool bRet = m_pWMICoreManager->GetUINT64Property(index, L"Capacity", temp);
        if (bRet)
        {
            temp = temp/1024; //K
            temp = temp/1024; // M
            capacity = (unsigned long)temp;
        }

        return bRet;
    }

    LVideoControllerManager::LVideoControllerManager()
    {
        m_pWMICoreManager = 0;
        m_pWMICoreManager = new LWMICoreManager();
        bool bRet = m_pWMICoreManager->BaseInit(NAMESPACE_ROOT_CIMV2);
        bRet = m_pWMICoreManager->WQLQuery(L"SELECT * FROM Win32_VideoController");
    }

    int LVideoControllerManager::GetVideoControllerCount()
    {
        return m_pWMICoreManager->GetObjectsCount();
    }

    bool LVideoControllerManager::GetVideoControllerDescription(IN int index, OUT wstring& description)
    {
        return m_pWMICoreManager->GetStringProperty(index, L"Description", description);
    }

    bool LVideoControllerManager::GetVideoControllerAdapterRAMCapacity(IN int index, OUT unsigned long& ramCapacity)
    {
        LUINT temp;
        bool bRet = m_pWMICoreManager->GetUINT32Property(index, L"AdapterRAM", temp);
        if (bRet)
        {
            temp = temp/1024; // K
            temp = temp/1024; // M
            ramCapacity = temp;
        }

        return bRet;
    }

    LVideoControllerManager::~LVideoControllerManager()
    {
        delete m_pWMICoreManager;
        m_pWMICoreManager = 0;
    }
}