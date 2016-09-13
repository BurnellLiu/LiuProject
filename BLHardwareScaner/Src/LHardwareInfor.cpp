
#include "LHardwareInfor.h"

#include <algorithm>
using std::transform;

#include "Wmi/LWMISystemClasses.h"
#include "Wmi/LWMIHardwareClasses.h"

#include "SetupApi/LSetupAPI.h"

#include "WifiNetwork/LWifiNetwork.h"

#include "DiskController/LDiskController.h"

#include "SMARTPaser/LSMARTPaser.h"

#include "Log/LLog.h"

#include "SMBiosPaser/LSMBiosPaser.h"

#include "Webcam/LWebcam.h"





/// @brief Ӳ����Ϣ��
///
/// �����ȡ�Ķ��ǹ̶���Ϣ
class CHardwareInfor
{
public:
    /// @brief ��ȡʵ������
    /// @return ʵ������
    static CHardwareInfor& GetInstance()
    {
        static CHardwareInfor s_instance;
        return s_instance;
    }

    /// @brief ��ȡ�����ϵͳ��Ϣ
    /// @return �����ϵͳ��Ϣ
    const ComputerSystemInfor& GetComputerSystemInfor()
    {
        static bool s_bScanDone = false; // ��ʶ�Ƿ����ɨ��Ӳ����Ϣ
        if (!s_bScanDone)
        {
            this->ScanComputerSystemInfor(m_computerSystemInfor);
            s_bScanDone = true;
        }

        return m_computerSystemInfor;
    }

    /// @brief ��ȡ����ϵͳ��Ϣ
    /// @return ����ϵͳ��Ϣ
    const OperatingSystemInfor& GetOperatingSystemInfor()
    {
        static bool s_bScanDone = false; // ��ʶ�Ƿ����ɨ��Ӳ����Ϣ
        if (!s_bScanDone)
        {
            this->ScanOperatingSystemInfor(m_operatingSystemInfor);
            s_bScanDone = true;
        }

        return m_operatingSystemInfor;
    }

    /// @brief ��ȡ������Ϣ
    /// @return ������Ϣ
    const MotherBoardInfor& GetMotherBoardInfor()
    {
        static bool s_bScanDone = false; // ��ʶ�Ƿ����ɨ��Ӳ����Ϣ
        if (!s_bScanDone)
        {
            this->ScanMotherBoardInfor(m_motherBoardInfor);
            s_bScanDone = true;
        }

        return m_motherBoardInfor;
    }

    /// @brief ��ȡ��������Ϣ
    /// @return ��������Ϣ
    const ProcessorInfor& GetProcessorInfor()
    {
        static bool s_bScanDone = false; // ��ʶ�Ƿ����ɨ��Ӳ����Ϣ
        if (!s_bScanDone)
        {
            this->ScanProcessorInfor(m_processorInfor);
            s_bScanDone = true;
        }

        return m_processorInfor;
    }

    /// @brief ��ȡ�Կ���Ϣ
    /// @return �Կ���Ϣ
    const VideoCardInforArray& GetVideoCardInfor()
    {
        static bool s_bScanDone = false; // ��ʶ�Ƿ����ɨ��Ӳ����Ϣ
        if (!s_bScanDone)
        {
            this->ScanVideoCardInfor(m_videoCardInfor);
            s_bScanDone = true;
        }

        return m_videoCardInfor;
    }

    /// @brief ��ȡ�����ڴ���Ϣ
    /// @return �����ڴ���Ϣ
    const PhysicalMemoryInforArray& GetPhysicalMemoryInfor()
    {
        static bool s_bScanDone = false; // ��ʶ�Ƿ����ɨ��Ӳ����Ϣ
        if (!s_bScanDone)
        {
            this->ScanPhysicalMemoryInfor(m_physicalMemoryInfor);
            s_bScanDone = true;
        }

        return m_physicalMemoryInfor;
    }

    /// @brief ��ȡ������Ϣ
    /// @return ������Ϣ
    const DiskInforArray& GetDiskInfor()
    {
        /*
        ������Ϣ�ᷢ���仯������Ҫ��ÿ�λ�ȡ��ʱ������ɨ��
        */
        this->ScanDiskInfor(m_diskInfor);

        return m_diskInfor;
    }

    /// @brief ��ȡ��ʾ����Ϣ
    /// @return ��ʾ����Ϣ
    const MonitorInforArray& GetMonitorInfor()
    {
        /*
        ��ʾ����Ϣ�ᷢ���仯������Ҫ��ÿ�λ�ȡ��ʱ������ɨ��
        */
        this->ScanMonitorInfor(m_monitorInfor);

        return m_monitorInfor;
    }

    /// @brief ��ȡ��ؾ�̬��Ϣ
    /// @return ��ؾ�̬��Ϣ
    const BatteryStaticInfor& GetBatteryStaticInfor()
    {
        this->ScanBatteryStaticInfor(m_batteryStaticInfor);

        return m_batteryStaticInfor;
    }

    /// @brief ��ȡ������Ϣ
    /// @return ������Ϣ
    const NetworkCardInforArray& GetNetworkCardInfor()
    {
        this->ScanNetworkCardInfor(m_networkCardInfor);

        return m_networkCardInfor;
    }

    /// @brief ��ȡ������Ϣ
    /// @return ������Ϣ
    const CDRomDriveInforArray& GetCDRomDriveInfor()
    {
        this->ScanCDRomDriveInfor(m_cdRomDriveInfor);

        return m_cdRomDriveInfor;
    }

    /// @brief ��ȡ�������Ϣ
    /// @return �������Ϣ
    const CameraInforArray& GetCameraInfor()
    {
        static bool s_bScanDone = false;
        if (!s_bScanDone)
        {
            this->ScanCameraInfor(m_cameraInfor);
            s_bScanDone = true;
        }
        

        return m_cameraInfor;
    }

    /// @brief ��������
    ~CHardwareInfor()
    {

    }

private:
    /// @brief ɨ������ϵͳ��Ϣ
    /// @param[out] computerSystemInfor �洢�����ϵͳ��Ϣ
    void ScanComputerSystemInfor(OUT ComputerSystemInfor& computerSystemInfor)
    {
        computerSystemInfor.Type = COMPUTER_TYPE_UNKNOWN;

        LWMI::LComputerSystemManager computerSytemManager;
        computerSytemManager.GetComputerSystemManufacturer(0, computerSystemInfor.Manufacturer);
        LWMI::LComputerSystemManager::LCOMPUTER_SYSTEM_TYPE pcType;
        computerSytemManager.GetComputerSystemPCType(0, pcType);
        computerSystemInfor.Type = (COMPUTER_TYPE)pcType;
        computerSytemManager.GetComputerSystemModel(0, computerSystemInfor.ModelName);

        LWMI::LMSSmBios_RawSMBiosTablesManager smBios;
        vector<unsigned char> data;
        smBios.GetSMBiosData(0, data);
        LSMBiosPaser smBiosPaser(data);
        SMBiosSystemInfor smBiosSystemInfor;
        smBiosPaser.GetSystemInfor(smBiosSystemInfor);
        computerSystemInfor.Version = smBiosSystemInfor.Version;
        computerSystemInfor.SerialNumber = smBiosSystemInfor.SerialNumber;
        for (unsigned int i = 0; i < 16; i++)
        {
            computerSystemInfor.UUID[i] = smBiosSystemInfor.UUID[i];
        }
    }


    /// @brief ɨ�����ϵͳ��Ϣ
    /// @param[out] operatingSystemInfor ����ϵͳ��Ϣ
    void ScanOperatingSystemInfor(OUT OperatingSystemInfor& operatingSystemInfor)
    {
        LWMI::LOperatingSystemManager operatingSystemManager;
        operatingSystemManager.GetOSCaption(0, operatingSystemInfor.Caption);
        operatingSystemManager.GetOSArchitecture(0, operatingSystemInfor.Architecture);
        operatingSystemManager.GetOSVersion(0, operatingSystemInfor.Version);
        operatingSystemManager.GetOSSystemDrive(0, operatingSystemInfor.SystemDrive);
    }

    /// @brief ɨ��������Ϣ
    /// @param[out] motherBoardInfor ������Ϣ
    void ScanMotherBoardInfor(OUT MotherBoardInfor& motherBoardInfor)
    {
        LWMI::LMSSmBios_RawSMBiosTablesManager smBios;
        vector<unsigned char> data;
        smBios.GetSMBiosData(0, data);
        LSMBiosPaser smBiosPaser(data);

        SMBiosBaseBoardInfor baseBoardInfor;
        smBiosPaser.GetBaseBoardInfor(baseBoardInfor);

        SMBiosBIOSInfor biosInfor;
        smBiosPaser.GetBiosInfor(biosInfor);

        motherBoardInfor.Manufacturer = baseBoardInfor.Manufacturer;
        motherBoardInfor.ProductName = baseBoardInfor.Product;
        motherBoardInfor.Version = baseBoardInfor.Version;
        motherBoardInfor.SerialNumber = baseBoardInfor.SerialNumber;

        motherBoardInfor.BiosVendor = biosInfor.Vendor;
        motherBoardInfor.BiosVersion = biosInfor.Version;
        motherBoardInfor.BiosReleaseDate = biosInfor.ReleaseDate;
        motherBoardInfor.BiosRomSize = biosInfor.RomSize;
    }

    /// @brief ɨ�账������Ϣ
    /// @param[out] processorInfor �洢��������Ϣ
    void ScanProcessorInfor(OUT ProcessorInfor& processorInfor)
    {
        LWMI::LProcessorManager processorManager;
        processorManager.GetProcessorName(0, processorInfor.Name);
        processorManager.GetProcessorDesription(0, processorInfor.Description);
        processorManager.GetProcessorManufacturer(0, processorInfor.Manufacturer);
        processorManager.GetProcessorCoresNumber(0, processorInfor.CoresNumber);
        processorManager.GetProcessorLogicalProcessorNumber(0, processorInfor.LogicalProcessorNumber);
        processorManager.GetProcessorMaxClockSpeed(0, processorInfor.MaxClockSpeed);
    }

    /// @brief ɨ���Կ���Ϣ
    /// @param[out] videoCardInfor �洢�Կ���Ϣ
    void ScanVideoCardInfor(OUT VideoCardInforArray& videoCardInfor)
    {
        videoCardInfor.Count = 0;

        LSetupDisplayCard displayCard;
        int cardCount = displayCard.GetDevNum();
        if (cardCount < 1)
            return;

        videoCardInfor.Count = (unsigned long)cardCount;

        for (int i = 0; i < cardCount && i < MAX_VIDEOCARD_NUMBER; i++)
        {
            videoCardInfor.Type[i] = VIDEO_CARD_UNKNOWN;

            wstring instanceID;
            unsigned long dwRet = displayCard.GetInstanceID(i, instanceID);
            if (0 != dwRet)
                continue;

            unsigned int busNumber;
            dwRet = displayCard.GetBusNumber(i, busNumber);
            if (0 != dwRet)
                continue;

            // �����Կ�����PCI��ۿ����������ߺŲ�Ϊ0
            // �����Կ�����PCI�����ߺ�Ϊ0
            if (0 == busNumber)
                videoCardInfor.Type[i] = VIDEO_CARD_INTERNAL;
            else
                videoCardInfor.Type[i] = VIDEO_CARD_EXTERNAL;


            LWMI::LVideoControllerManager videoControllerManager(instanceID);
            if (videoControllerManager.GetVideoControllerCount() != 1)
                continue;

            videoControllerManager.GetVideoControllerDescription(0, videoCardInfor.Description[i]);
            videoControllerManager.GetVideoControllerAdapterRAMCapacity(0, videoCardInfor.RAMSize[i]);
        }

    }

    /// @brief ɨ�������ڴ���Ϣ
    /// @param[out] physicalMemoryInfor �洢�����ڴ���Ϣ
    void ScanPhysicalMemoryInfor(OUT PhysicalMemoryInforArray& physicalMemoryInfor)
    {
        physicalMemoryInfor.Count = 0;

        LWMI::LPhysicalMemoryManager memoryManager;
        physicalMemoryInfor.Count = (unsigned long)memoryManager.GetPhysicalMemoryCount();
        for (int i = 0; i < (int)physicalMemoryInfor.Count && i < MAX_PHYSICAL_MEMORY_NUMBER; i++)
        {
            memoryManager.GetPhysicalMemoryManufacturer(i, physicalMemoryInfor.Manufacturer[i]);
            memoryManager.GetPhysicalMemoryCapacity(i, physicalMemoryInfor.Capacity[i]);
            memoryManager.GetPhysicalMemoryPartNumber(i, physicalMemoryInfor.PartNumbe[i]);
            memoryManager.GetPhysicalMemorySerialNumber(i, physicalMemoryInfor.SerialNumber[i]);
            memoryManager.GetPhysicalMemorySpeed(i, physicalMemoryInfor.Speed[i]);
        }
    }

    /// @brief ɨ�������Ϣ
    /// @param[out] diskInfor �洢������Ϣ
    void ScanDiskInfor(OUT DiskInforArray& diskInfor)
    {
        PrintLogW(L"Scan Disk Infor Function");

        diskInfor.Count  = 0;
        LWMI::LDiskDriveManager diskDriveManager;
        diskInfor.Count = (unsigned long)diskDriveManager.GetDiskCount();
        for (int i = 0; i < (int)diskInfor.Count && i < MAX_DISK_NUMBER; i++)
        {
            PrintLogW(L"Disk Index: %d", i);

            diskDriveManager.GetDiskModel(i, diskInfor.Model[i]);
            diskDriveManager.GetDiskSerialNumber(i, diskInfor.SerialNumber[i]);
            diskDriveManager.GetDiskSize(i, diskInfor.TotalSize[i]);
            diskDriveManager.GetDiskLogicalName(i, diskInfor.LogicalName[i]);

            LWMI::LDiskDriveManager::LDISK_TYPE diskType;
            diskDriveManager.GetDiskType(i, diskType);
            diskInfor.DiskType[i] = (DISK_TYPE )diskType;
            diskDriveManager.GetDiskInterfaceType(i, diskInfor.InterfaceType[i]);

            diskInfor.IsATA[i] = false;
            diskInfor.ATAInfor[i].RotationRate = 0;
            diskInfor.ATAInfor[i].SATAType = 0;
            diskInfor.ATAInfor[i].PowerOnHours = 0;

            diskInfor.FixedDiskType[i] = FIXED_DISK_UNKNOWN;

            if (diskInfor.DiskType[i] != FIXED_DISK)
                continue;

            // ��ȡ�̶�Ӳ�̵���������

            wstring pnpDeviceID;
            wstring parentInstanceID;
            diskDriveManager.GetDiskPNPDeviceID(i, pnpDeviceID);
            PrintLogW(L"Disk InstanceID: %s", pnpDeviceID.c_str());
            if (pnpDeviceID.empty())
                continue;

            // ��ȡ������������ʵ��ID
            LSetupDevAll devAll;
            for (int j = 0; j < devAll.GetDevNum(); j++)
            {
                wstring instanceID;
                devAll.GetInstanceID(j, instanceID);
                if (pnpDeviceID == instanceID)
                {
                    devAll.GetParentInstanceId(j, parentInstanceID);
                    break;
                }
            }

            PrintLogW(L"Disk Controller InstanceID: %s", parentInstanceID.c_str());
            if (parentInstanceID.empty())
                continue;

            // ��ȡ����������������ƥ���豸ID
            wstring diskControllerClass;
            wstring diskControllerMatchingDeviceID;
            for (int j = 0; j < devAll.GetDevNum(); j++)
            {
                wstring instanceID;
                devAll.GetInstanceID(j, instanceID);
                if (parentInstanceID == instanceID)
                {
                    devAll.GetClass(j,diskControllerClass);
                    devAll.GetMatchingDeviceId(j, diskControllerMatchingDeviceID);
                    break;
                }
            }

            PrintLogW(L"Disk Controller Class: %s", diskControllerClass.c_str());
            PrintLogW(L"Disk Controller Matching DeviceID: %s", diskControllerMatchingDeviceID.c_str());
            if (diskControllerClass.empty())
                continue;

            if (diskControllerClass.compare(L"hdc") == 0 ||
                diskControllerClass.compare(L"HDC") == 0) // Hard Disk Controller
            {
                diskInfor.IsATA[i] = true;
                wstring deviceID;
                diskDriveManager.GetDiskDeviceID(i, deviceID);
                LIDEDiskController ideDiskController(deviceID);
                diskInfor.ATAInfor[i].RotationRate = ideDiskController.GetRotationRate();
                diskInfor.ATAInfor[i].SATAType = ideDiskController.GetSATAType();
                if (diskInfor.ATAInfor[i].RotationRate == 1)
                    diskInfor.FixedDiskType[i] = FIXED_DISK_SSD;
                else
                    diskInfor.FixedDiskType[i] = FIXED_DISK_HDD;

                unsigned char smartData[SMART_DATA_LENGTH] = {0}; // �洢SMART����
                ideDiskController.GetSMARTData(smartData);
                LSMARTParser smartParser(smartData);
                diskInfor.ATAInfor[i].PowerOnHours = 0;
                smartParser.GetPowerOnHours(diskInfor.ATAInfor[i].PowerOnHours);
            }
            else if (diskControllerClass.compare(L"SCSIAdapter") == 0) // SCSI and RAID Controllers
            {
                if (diskControllerMatchingDeviceID.compare(L"SD\\CLASS_MMC") == 0)
                    diskInfor.FixedDiskType[i] = FIXED_DISK_EMMC;
                else
                    diskInfor.FixedDiskType[i] = FIXED_DISK_RAID;
            }
            else
            {
                diskInfor.FixedDiskType[i] = FIXED_DISK_UNKNOWN;
            }
        }
    }

    /// @brief ɨ����ʾ����Ϣ
    /// @param[out] monitorInfor �洢��ʾ����Ϣ
    void ScanMonitorInfor(OUT MonitorInforArray& monitorInfor)
    {
        monitorInfor.Count = 0;
        LSetupMonitor setupMonitor;
        monitorInfor.Count = (unsigned long)setupMonitor.GetDevNum();
        for (unsigned long i = 0; i < monitorInfor.Count && i < MAX_MONITOR_NUMBER; i++)
        {
            LMonitorExtendInfor extendInfor;
            setupMonitor.GetExtendInfor(i, extendInfor);
            monitorInfor.Name[i] = extendInfor.Name;
            monitorInfor.Date[i] = extendInfor.Date;
        }
    }

    /// @brief ɨ���ؾ�̬��Ϣ
    /// @param[out] batteryStaticInfor �洢��ؾ�̬��Ϣ
    void ScanBatteryStaticInfor(OUT BatteryStaticInfor& batteryStaticInfor)
    {
        LWMI::LBatteryManager batteryManager;
        LWMI::LBatteryStaticDataManager batteryStaticDataManager;
        LWMI::LBatteryFullCapacityManager batteryFullCapacityManager;

        if (batteryManager.GetBatteryCount() < 1)
        {
            batteryStaticInfor.Exist = false;
            return;
        }

        batteryStaticInfor.Exist = true;

        batteryManager.GetBatteryName(0, batteryStaticInfor.Name);
        batteryStaticDataManager.GetBatteryManufacturerName(0, batteryStaticInfor.Manufacturer);
        batteryStaticDataManager.GetBatterySerialNumber(0, batteryStaticInfor.SerialNumber);

        batteryStaticDataManager.GetBatteryDesignedCapacity(0, batteryStaticInfor.DesignedCapacity);
        batteryFullCapacityManager.GetBatteryFullChargedCapacity(0, batteryStaticInfor.FullChargedCapacity);
        batteryManager.GetBatteryDesignVoltage(0, batteryStaticInfor.DesignedVoltage);
    }

    /// @brief ɨ��������Ϣ
    /// @param[out] networkCardInfor �洢������Ϣ
    void ScanNetworkCardInfor(OUT NetworkCardInforArray& networkCardInfor)
    {
        // ��ȡ��������GUID
        LWifiNetwork wifiNetwork;
        wstring wlanGUID;
        wifiNetwork.GetWLANGUID(wlanGUID);
        wlanGUID = WStringToUpper(wlanGUID);


        LWMI::LNetworkAdapterManager networkCardManager;
        networkCardInfor.Count = 0;

        for (int i = 0; i < networkCardManager.GetNetworkCardCount() && i < MAX_NETWORKCARD_NUMBER; i++)
        {
            wstring pnpDeviceID;
            networkCardManager.GetNetworkCardPNPDeviceID(i, pnpDeviceID);
            pnpDeviceID = WStringToUpper(pnpDeviceID);

            wstring cardGUID;
            networkCardManager.GetNetworkCardGUID(i, cardGUID);
            cardGUID = WStringToUpper(cardGUID);

            if (wcsncmp(pnpDeviceID.c_str(), L"PCI\\", 4) == 0 ||
                wcsncmp(pnpDeviceID.c_str(), L"USB\\", 4) == 0 ||
                wcsncmp(pnpDeviceID.c_str(), L"SD\\", 3) == 0)
            {
                if (cardGUID.compare(wlanGUID) == 0)
                    networkCardInfor.Type[networkCardInfor.Count] = WIFI_NETCARD;
                else
                    networkCardInfor.Type[networkCardInfor.Count] = ETHERNET_NETCARD;
            }
            else if (wcsncmp(pnpDeviceID.c_str(), L"BTH", 3) == 0)
            {
                networkCardInfor.Type[networkCardInfor.Count] = BLUETOOTH_NETCARD;
            }
            else
            {
                continue;
            }

            networkCardManager.GetNetworkCardName(networkCardInfor.Count, networkCardInfor.Name[i]);
            networkCardManager.GetNetworkCardManufacturer(networkCardInfor.Count, networkCardInfor.Manufacturer[i]);
            networkCardManager.GetNetworkCardMACAddress(networkCardInfor.Count, networkCardInfor.MACAddress[i]);

            networkCardInfor.Count++;
        }
    }

    /// @brief ɨ�������Ϣ
    /// @param[out] cdRomDrive �洢������Ϣ
    void ScanCDRomDriveInfor(OUT CDRomDriveInforArray& cdRomDriveInfor)
    {
        LWMI::LCDRomDriveManager cdRomDriveManager;
        cdRomDriveInfor.Count = (unsigned long)cdRomDriveManager.GetCDRomDriveCount();

        for (int i = 0; i < cdRomDriveManager.GetCDRomDriveCount(); i++)
        {
            cdRomDriveManager.GetCDRomDriveName(i, cdRomDriveInfor.Name[i]);
        }
    }

    /// @brief ɨ���������Ϣ
    /// @param[out] cameraInfor �洢�������Ϣ
    void ScanCameraInfor(OUT CameraInforArray& cameraInfor)
    {
        PrintLogW(L"Scan Camera Infor Function");
        vector<LWebcam*> webcamVec;
        string error;
        LEnumWebcam(webcamVec, error);

        LSetupImage imageDevice;
        cameraInfor.Count = 0;
        for (int i = 0; i < imageDevice.GetDevNum(); i++)
        {
            wstring instanceId;
            unsigned long lRet = imageDevice.GetInstanceID(i, instanceId);
            PrintLogW(L"Image Device Index: %d, InstanceID: %s", i, instanceId.c_str());
            if (lRet != 0 || instanceId.empty())
                continue;

            for (unsigned int j = 0; j < webcamVec.size(); j++)
            {
                wstring displayName;
                bool bRet = webcamVec[j]->GetDisplayName(displayName);
                PrintLogW(L"Webcam Index: %u, DisplayName: %s", j, displayName.c_str());
                if (!bRet)
                    continue;

                // ��ת��Ϊ��д, ���滻'#'Ϊ'\\'
                displayName = WStringToUpper(displayName);
                for (unsigned int k = 0; k < displayName.size(); k++)
                {
                    if (displayName[k] == L'#')
                    {
                        displayName[k] = L'\\';
                    }
                }
                if (displayName.find(instanceId) == wstring::npos)
                    continue;

                wstring friendlyName;
                webcamVec[j]->GetFriendlyName(friendlyName);

                cameraInfor.Name[cameraInfor.Count] = friendlyName;
                cameraInfor.Count++;
            }
          
        }

        // �ͷ���Դ
        for (unsigned int i = 0; i < webcamVec.size(); i++)
        {
            if (webcamVec[i] != 0)
            {
                webcamVec[i]->Release();
                webcamVec[i] = 0;
            }
            
        }
    }

    /// @brief ���ַ�����Сд��ĸת��Ϊ��д
    ///  
    /// ��Ҫ���Խ���uicode�ַ���ת����Сд, ��Ϊ�������ڶ��ֽڱ�����ʹ�������ֽڱ�ʾ
    /// @param[in] str ԭʼ�ַ���
    /// @return ת����Ĵ�д�ַ���
    wstring WStringToUpper(IN const wstring& str)
    {
        wstring newStr = str;
        transform(newStr.begin(), newStr.end(), newStr.begin(), towupper);
        return newStr;
    }

    CHardwareInfor()
    {

    }
    CHardwareInfor(const CHardwareInfor&); // ��ֹĬ�Ͽ������캯��
    CHardwareInfor& operator = (const CHardwareInfor&); // ��ֹ��ֵ������

private:
    ComputerSystemInfor m_computerSystemInfor; ///< �����ϵͳ��Ϣ
    OperatingSystemInfor m_operatingSystemInfor; ///< ����ϵͳ��Ϣ
    MotherBoardInfor m_motherBoardInfor; ///< ������Ϣ
    ProcessorInfor m_processorInfor; ///< ��������Ϣ
    VideoCardInforArray m_videoCardInfor; ///< �Կ���Ϣ
    PhysicalMemoryInforArray m_physicalMemoryInfor; ///< �����ڴ���Ϣ
    DiskInforArray m_diskInfor; ///< ������Ϣ
    MonitorInforArray m_monitorInfor; ////< ��ʾ����Ϣ
    BatteryStaticInfor m_batteryStaticInfor; ///< ��ؾ�̬��Ϣ
    NetworkCardInforArray m_networkCardInfor; ///< ������Ϣ
    CDRomDriveInforArray m_cdRomDriveInfor; ///< ������Ϣ
    CameraInforArray m_cameraInfor; ///< �������Ϣ
};


LHardwareInfor::LHardwareInfor()
{

}

LHardwareInfor::~LHardwareInfor()
{

}

const ComputerSystemInfor& LHardwareInfor::GetComputerSystemInfor()
{
    return CHardwareInfor::GetInstance().GetComputerSystemInfor();
}

const OperatingSystemInfor& LHardwareInfor::GetOperatingSystemInfor()
{
    return CHardwareInfor::GetInstance().GetOperatingSystemInfor();
}

const MotherBoardInfor& LHardwareInfor::GetMotherBoardInfor()
{
    return CHardwareInfor::GetInstance().GetMotherBoardInfor();
}

const ProcessorInfor& LHardwareInfor::GetProcessorInfor()
{
    return CHardwareInfor::GetInstance().GetProcessorInfor();
}

const VideoCardInforArray& LHardwareInfor::GetVideoCardInfor()
{
    return CHardwareInfor::GetInstance().GetVideoCardInfor();
}

const PhysicalMemoryInforArray& LHardwareInfor::GetPhysicalMemoryInfor()
{
    return CHardwareInfor::GetInstance().GetPhysicalMemoryInfor();
}

const DiskInforArray& LHardwareInfor::GetDiskInfor()
{
    return CHardwareInfor::GetInstance().GetDiskInfor();
}

const MonitorInforArray& LHardwareInfor::GetMonitorInfor()
{
    return CHardwareInfor::GetInstance().GetMonitorInfor();
}

const BatteryStaticInfor& LHardwareInfor::GetBatteryStaticInfor()
{
    return CHardwareInfor::GetInstance().GetBatteryStaticInfor();
}

const NetworkCardInforArray& LHardwareInfor::GetNetworkCardInfor()
{
    return CHardwareInfor::GetInstance().GetNetworkCardInfor();
}

const CDRomDriveInforArray& LHardwareInfor::GetCDRomDriveInfor()
{
    return CHardwareInfor::GetInstance().GetCDRomDriveInfor();
}

const CameraInforArray& LHardwareInfor::GetCameraInfor()
{
    return CHardwareInfor::GetInstance().GetCameraInfor();
}






















