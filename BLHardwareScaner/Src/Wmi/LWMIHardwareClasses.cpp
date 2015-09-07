
#include "LWMIHardwareClasses.h"

#include <Windows.h>

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

    bool LPhysicalMemoryManager::GetPhysicalMemorySpeed(IN int index, OUT unsigned long& speed)
    {
        LUINT uSpeed;
        bool bRet = m_pWMICoreManager->GetUINT32Property(index, L"Speed", uSpeed);
        speed = uSpeed;

        return bRet;
    }

    bool LPhysicalMemoryManager::GetPhysicalMemoryPartNumber(IN int index, OUT wstring& partNumber)
    {
        return m_pWMICoreManager->GetStringProperty(index, L"PartNumber", partNumber);
    }

    LVideoControllerManager::LVideoControllerManager()
    {
        m_pWMICoreManager = 0;
        m_pWMICoreManager = new LWMICoreManager();
        bool bRet = m_pWMICoreManager->BaseInit(NAMESPACE_ROOT_CIMV2);
        bRet = m_pWMICoreManager->WQLQuery(L"SELECT * FROM Win32_VideoController");
    }

    LVideoControllerManager::LVideoControllerManager(IN const wstring& pnpDeviceID)
    {
        // 查询语句中需要将"\\"字符替换为"\\\\"
        wstring pnpDeviceIDFormat;
        for (unsigned int i = 0; i < pnpDeviceID.length(); i++)
        {
            if (pnpDeviceID[i] == L'\\')
            {
                pnpDeviceIDFormat.push_back(L'\\');
                pnpDeviceIDFormat.push_back(L'\\');
            }
            else
            {
                pnpDeviceIDFormat.push_back(pnpDeviceID[i]);
            }
        }

        m_pWMICoreManager = 0;
        m_pWMICoreManager = new LWMICoreManager();
        bool bRet = m_pWMICoreManager->BaseInit(NAMESPACE_ROOT_CIMV2);

        wstring queryStatement = L"SELECT * FROM Win32_VideoController ";
        queryStatement += L"WHERE PNPDeviceID = \"";
        queryStatement += pnpDeviceIDFormat;
        queryStatement += L"\"";

        bRet = m_pWMICoreManager->WQLQuery(queryStatement.c_str());
    }

    LVideoControllerManager::~LVideoControllerManager()
    {
        delete m_pWMICoreManager;
        m_pWMICoreManager = 0;
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

    LDiskDriveManager::LDiskDriveManager()
    {
        m_pWMICoreManager = 0;
        m_pWMICoreManager = new LWMICoreManager();
        bool bRet = m_pWMICoreManager->BaseInit(NAMESPACE_ROOT_CIMV2);
        bRet = m_pWMICoreManager->WQLQuery(L"SELECT * FROM Win32_DiskDrive");
    }

    LDiskDriveManager::~LDiskDriveManager()
    {
        delete m_pWMICoreManager;
        m_pWMICoreManager = 0;
    }

    int LDiskDriveManager::GetDiskCount()
    {
        return m_pWMICoreManager->GetObjectsCount();
    }

    bool LDiskDriveManager::GetDiskModel(IN int index, OUT wstring& model)
    {
        return m_pWMICoreManager->GetStringProperty(index, L"Model", model);
    }

    bool LDiskDriveManager::GetDiskSerialNumber(IN int index, OUT wstring& serialNumber)
    {
        return m_pWMICoreManager->GetStringProperty(index, L"SerialNumber", serialNumber);
    }

    bool LDiskDriveManager::GetDiskSize(IN int index, OUT unsigned long& size)
    {
        LUINT64 size64;
        bool bRet = m_pWMICoreManager->GetUINT64Property(index, L"Size", size64);

        size64 = size64/1024; // K
        size64 = size64/1024; // M
        size64 = size64/1024; // G

        size = (unsigned long)size64;

        return bRet;
    }

    bool LDiskDriveManager::GetDiskDeviceID(IN int index, OUT wstring& deviceID)
    {
        return m_pWMICoreManager->GetStringProperty(index, L"DeviceID", deviceID);
    }

    bool LDiskDriveManager::GetDiskPNPDeviceID(IN int index, OUT wstring& pnpDeviceID)
    {
        return m_pWMICoreManager->GetStringProperty(index, L"PNPDeviceID", pnpDeviceID);
    }

    bool LDiskDriveManager::GetDiskInterfaceType(IN int index, OUT wstring& type)
    {
        return m_pWMICoreManager->GetStringProperty(index, L"InterfaceType", type);
    }

    bool LDiskDriveManager::GetDiskType(IN int index, OUT LDISK_TYPE& type)
    {
        bool bRet = false;
        HANDLE hDiskDrive = NULL;
        STORAGE_DEVICE_DESCRIPTOR* pDeviceDescriptor = NULL;

        wstring deviceId;
        wstring interfaceType;
        if (!this->GetDiskDeviceID(index, deviceId))
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        if (!this->GetDiskInterfaceType(index, interfaceType))
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        // 打开设备句柄
        hDiskDrive = CreateFileW(deviceId.c_str(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
        if (hDiskDrive == INVALID_HANDLE_VALUE)
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        STORAGE_PROPERTY_QUERY propertyQuery;
        ZeroMemory(&propertyQuery, sizeof(propertyQuery));
        propertyQuery.PropertyId = StorageDeviceProperty;
        propertyQuery.QueryType = PropertyStandardQuery;

        ULONG nBytes;
        STORAGE_DEVICE_DESCRIPTOR deviceDescriptor;
        BOOL dwRet = DeviceIoControl(
            hDiskDrive, 
            IOCTL_STORAGE_QUERY_PROPERTY, 
            &propertyQuery, 
            sizeof(propertyQuery), 
            &deviceDescriptor, 
            sizeof(deviceDescriptor), 
            &nBytes, 
            NULL);

        if (dwRet == FALSE)
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        pDeviceDescriptor = (PSTORAGE_DEVICE_DESCRIPTOR)malloc(nBytes);
        bRet = DeviceIoControl(
            hDiskDrive, 
            IOCTL_STORAGE_QUERY_PROPERTY, 
            &propertyQuery, 
            sizeof(propertyQuery), 
            pDeviceDescriptor, 
            nBytes, 
            &nBytes, 
            NULL);

        if (bRet == FALSE)
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        if (pDeviceDescriptor->RemovableMedia == TRUE)
        {
            switch (pDeviceDescriptor->DeviceTypeModifier)
            {
            case 13:
                type = SD_CARD_DISK;
                break;
            case 0:
                type = USB_FLASH_DISK;
                break;
            default:
                type = UNKNOWN_DISK;
                break;
            }
        }

        if (pDeviceDescriptor->RemovableMedia == FALSE)
        {
            switch (pDeviceDescriptor->BusType)
            {
            case BusTypeVirtual:
            case BusTypeFileBackedVirtual:
                type = VIRTUAL_DISK;
                break;
            default:
                if (interfaceType.find(L"USB") != wstring::npos)
                    type = EXTERNAL_USB_DISK;
                else if (interfaceType.find(L"IDE") != wstring::npos)
                    type = FIXED_IDE_DISK;
                else
                    type = UNKNOWN_DISK;
                break;
            }
        }

        bRet = true;

SAFE_EXIT:
        if (hDiskDrive != NULL)
        {
            CloseHandle(hDiskDrive);
            hDiskDrive = NULL;
        }

        if (pDeviceDescriptor != NULL)
        {
            free(pDeviceDescriptor);
            pDeviceDescriptor = NULL;
        }

        return bRet;
    }

    
}