

#ifndef _LWMIHARDWARECLASSES_H_
#define _LWMIHARDWARECLASSES_H_

#include <string>
using std::wstring;

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

namespace LWMI
{
    class LWMICoreManager;

    /// @brief �����������
    class LBaseBoardManager
    {
    public:
        LBaseBoardManager();
        ~LBaseBoardManager();

        /// @brief ��ȡ��������
        /// @return ��������(Ӧ��Ϊ1)
        int GetBaseBoardCount();

        /// @brief ��ȡ�������
        /// @param[in] index ��������
        /// @param[out] caption �洢�������
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetBaseBoardCaption(IN int index, OUT wstring& caption);

        /// @brief ��ȡ��������
        /// @param[in] index ��������
        /// @param[out] caption �洢��������
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetBaseBoardDescription(IN int index, OUT wstring& description);

        /// @brief ��ȡ����������
        /// @param[in] index ��������
        /// @param[out] manufacturer �洢����������
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetBaseBoardManufacturer(IN int index, OUT wstring& manufacturer);

        /// @brief ��ȡ�������к�
        /// @param[in] index ��������
        /// @param[�洢���к�] serialNumber
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetBaseBoardSerialNumber(IN int index, OUT wstring& serialNumber);

    private:
        LWMICoreManager* m_pWMICoreManager; ///< WMI���Ĺ�����

    private:
        // ��ֹ�������캯���͸�ֵ������
        LBaseBoardManager(const LBaseBoardManager&);
        LBaseBoardManager& operator = (const LBaseBoardManager&);
    };

    /// @brief BIOS��������
    class LBIOSManager
    {
    public:
        LBIOSManager();
        ~LBIOSManager();

        /// @brief ��ȡBIOS����
        /// @return BIOS����(Ӧ��Ϊ1)
        int GetBIOSCount();

        /// @brief ��ȡBIOS����
        /// @param[in] index BIOS����
        /// @param[out] description �洢BIOS����
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetBIOSDescription(IN int index, OUT wstring& description);

        /// @brief ��ȡBIOS������
        /// @param[in] index BIOS����
        /// @param[out] manufacturer �洢BIOS������
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetBIOSManufacturer(IN int index, OUT wstring& manufacturer);

        /// @brief ��ȡBIOS���к�
        /// @param[in] index BIOS����
        /// @param[out] serialNumber �洢BIOS���к�
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetBIOSSerialNumber(IN int index, OUT wstring& serialNumber);

        /// @brief ��ȡBIOS�汾
        /// @param[in] index BIOS����
        /// @param[in] version �洢BIOS�汾
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetBIOSVersion(IN int index, OUT wstring& version);

        /// @brief ��ȡSMBIOS BIOS�汾
        /// @param[in] index BIOS����
        /// @param[in] version �洢SMBIOS BIOS�汾
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetSMBIOSBIOSVersion(IN int index, OUT wstring& version);

    private:
        LWMICoreManager* m_pWMICoreManager; ///< WMI���Ĺ�����

    private:
        // ��ֹ�������캯���͸�ֵ������
        LBIOSManager(const LBIOSManager&);
        LBIOSManager& operator = (const LBIOSManager&);
    };

    /// @brief ��������������
    class LProcessorManager
    {
    public:
        LProcessorManager();
        ~LProcessorManager();

        /// @brief �������ܹ�
        enum LPROCESSOR_ARCHITECTURE
        {
            X86 = 0,
            MIPS = 1,
            ALPHA = 2,
            POWER_PC = 3,
            ARM = 5,
            IA64 = 6,
            X64 = 9
        };

        /// @brief ��ȡ����������
        /// @return ����������(һ��Ϊ1)
        int GetProcessorCount();

        /// @brief ��ȡ����������
        /// @param[in] index ����������
        /// @param[out] name �洢����
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetProcessorName(IN int index, OUT wstring& name);

        /// @brief ��ȡ����������
        /// @param[in] index ����������
        /// @param[out] description �洢������Ϣ
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetProcessorDesription(IN int index, OUT wstring& description);

        /// @brief ��ȡ�������ܹ���Ϣ
        /// @param[in] index ����������
        /// @param[out] architecture �洢�ܹ���Ϣ
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetProcessorArchitecture(IN int index, OUT LPROCESSOR_ARCHITECTURE& architecture);

        /// @brief ��ȡ������������
        /// @param[in] index ����������
        /// @param[out] manufacturer �洢������
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetProcessorManufacturer(IN int index, OUT wstring& manufacturer);

        /// @brief ��ȡ���������Ƶ��(��λMHz)
        /// @param[in] index ����������
        /// @param[in] speed �洢Ƶ��
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetProcessorMaxClockSpeed(IN int index, OUT unsigned long& speed);

        /// @brief ��ȡ������������
        /// @param[in] index ����������
        /// @param[in] coresNumber �洢������
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetProcessorCoresNumber(IN int index, OUT unsigned long& coresNumber);

        /// @brief ��ȡ�������߼�����������(�߳���)
        /// @param[in] index ����������
        /// @param[in] logicalProcessorsNumber �߼�����������
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetProcessorLogicalProcessorNumber(IN int index, OUT unsigned long& logicalProcessorsNumber);

    private:
        LWMICoreManager* m_pWMICoreManager; ///< WMI���Ĺ�����

    private:
        // ��ֹ�������캯���͸�ֵ������
        LProcessorManager(const LProcessorManager&);
        LProcessorManager& operator = (const LProcessorManager&);
    };

    /// @brief �����ڴ��������
    class LPhysicalMemoryManager
    {
    public:
        LPhysicalMemoryManager();
        ~LPhysicalMemoryManager();

        /// @brief ��ȡ�����ڴ�����
        /// @return �����ڴ�����
        int GetPhysicalMemoryCount();

        /// @brief ��ȡ�����ڴ�����
        /// @param[in] index �ڴ�����
        /// @param[out] description �洢����
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetPhysicalMemoryDescription(IN int index, OUT wstring& description);

        /// @brief ��ȡ�����ڴ�������
        /// @param[in] index �ڴ�����
        /// @param[out] manufacturer �洢������
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetPhysicalMemoryManufacturer(IN int index, OUT wstring& manufacturer);

        /// @brief ��ȡ�����ڴ����к�
        /// @param[in] index �����ڴ�����
        /// @param[out] serialNumber �洢���к�
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetPhysicalMemorySerialNumber(IN int index, OUT wstring& serialNumber);

        /// @brief ��ȡ�����ڴ�����(��λM)
        /// @param[in] index �����ڴ�����
        /// @param[out] capacity �洢����
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetPhysicalMemoryCapacity(IN int index, OUT unsigned long& capacity);

        /// @brief ��ȡ�����ڴ�Ƶ��(��λMHz)
        /// @param[in] index �����ڴ�����
        /// @param[out] speed �洢Ƶ��
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetPhysicalMemorySpeed(IN int index, OUT unsigned long& speed);

        /// @brief ��ȡ�����ڴ��ͺ�
        /// @param[in] index ����
        /// @param[out] partNumber �洢�ͺ�
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetPhysicalMemoryPartNumber(IN int index, OUT wstring& partNumber);

    private:
        LWMICoreManager* m_pWMICoreManager; ///< WMI���Ĺ�����

    private:
        // ��ֹ�������캯���͸�ֵ������
        LPhysicalMemoryManager(const LPhysicalMemoryManager&);
        LPhysicalMemoryManager& operator = (const LPhysicalMemoryManager&);
    };

    /// @brief ����������(�Կ�)��������
    class LVideoControllerManager
    {
    public:
        LVideoControllerManager();
        LVideoControllerManager(IN const wstring& pnpDeviceID);
        ~LVideoControllerManager();

        /// @brief ��ȡ��������������
        /// @return ��������������
        int GetVideoControllerCount();

        /// @brief ��ȡ��������������
        /// @param[in] index ��������������
        /// @param[in] description �洢��������������
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetVideoControllerDescription(IN int index, OUT wstring& description);

        /// @brief ��ȡ��������������������������(�Դ�����)(��λM)
        /// @param[in] index ��������������
        /// @param[in] ramCapacity ����������
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetVideoControllerAdapterRAMCapacity(IN int index, OUT unsigned long& ramCapacity);

    private:
        LWMICoreManager* m_pWMICoreManager; ///< WMI���Ĺ�����

    private:
        // ��ֹ�������캯���͸�ֵ������
        LVideoControllerManager(const  LVideoControllerManager&);
        LVideoControllerManager& operator = (const  LVideoControllerManager&);
    };

    /// @brief ������������������
    class LDiskDriveManager
    {
    public:
        LDiskDriveManager();
        ~LDiskDriveManager();

        /// @brief ��������
        enum LDISK_TYPE
        {
            UNKNOWN_DISK = 0, // δ֪����
            FIXED_IDE_DISK = 1, // �̶�����(�籾��Ӳ��)
            EXTERNAL_USB_DISK = 2, // ��չ����(��USB�ƶ�Ӳ��)
            VIRTUAL_DISK = 3, // ����Ӳ��(��VHD)
            USB_FLASH_DISK = 4, // U��
            SD_CARD_DISK = 5// SD��
        };

        /// @brief ��ȡ��������������
        /// @return ��������
        int GetDiskCount();

        /// @brief ��ȡ�����ͺ�
        /// @param[in] index ��������
        /// @param[out] model �����ͺ�
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetDiskModel(IN int index, OUT wstring& model);

        /// @brief ��ȡ�������к�
        ///  
        /// @param[in] index ��������
        /// @param[out] serialNumber ���к�
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetDiskSerialNumber(IN int index, OUT wstring& serialNumber);

        /// @brief ��ȡ���̴�С
        /// @param[in] index ��������
        /// @param[out] size �洢���̴�С, ��λ(G)
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetDiskSize(IN int index, OUT unsigned long& size);

        /// @brief ��ȡ�����豸ID
        /// @param[in] index ��������
        /// @param[out] deviceID �洢�����豸ID
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetDiskDeviceID(IN int index, OUT wstring& deviceID);

        /// @brief ��ȡ����ʵ��·��
        /// @param[in] index ��������
        /// @param[out] pnpDeviceID �洢����ʵ��·��
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetDiskPNPDeviceID(IN int index, OUT wstring& pnpDeviceID);

        /// @brief ��ȡ���̽ӿ�����
        /// @param[in] index ��������
        /// @param[out] type ���̽ӿ�����(SCSI, HDC, IDE, USB, 1394)
        /// @return
        bool GetDiskInterfaceType(IN int index, OUT wstring& type);

        /// @brief ��ȡ��������
        /// @param[in] index ��������
        /// @param[out] type �洢��������
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetDiskType(IN int index, OUT LDISK_TYPE& type);

    private:
        LWMICoreManager* m_pWMICoreManager; ///< WMI���Ĺ�����

    private:
        // ��ֹ�������캯���͸�ֵ������
        LDiskDriveManager(const  LDiskDriveManager&);
        LDiskDriveManager& operator = (const  LDiskDriveManager&);
    };

    /// @brief ��ع�������
    class LBatteryManager
    {
    public:
        LBatteryManager();
        ~LBatteryManager();

        /// @brief ��ȡ�������
        /// @return �������
        int GetBatteryCount();

        /// @brief ��ȡ�������
        /// @param[in] index
        /// @param[out] name �洢����
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetBatteryName(IN int index, OUT wstring& name);

        /// @brief ��ȡ����豸ID
        /// @param[in] index
        /// @param[out] deviceID �洢�豸ID
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetBatteryDeviceID(IN int index, OUT wstring& deviceID);

        /// @brief ��ȡ�����Ƶ�ѹ
        /// @param[in] index 
        /// @param[out] voltage �洢��Ƶ�ѹ(��λmV)
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetBatteryDesignVoltage(IN int index, OUT unsigned long& voltage);

    private:
        LWMICoreManager* m_pWMICoreManager; ///< WMI���Ĺ�����
    private:
        LBatteryManager(const LBatteryManager&);
        LBatteryManager& operator = (const LBatteryManager&);
    };

    /// @brief ��ؾ�̬���ݹ�������
    class LBatteryStaticDataManager
    {
    public:
        LBatteryStaticDataManager();
        ~LBatteryStaticDataManager();

        /// @brief ��ȡ�����Ŀ
        /// @return �����Ŀ
        int GetBatteryCount();

        /// @brief ��ȡ�������������
        /// @param[in] index
        /// @param[in] manufacturer �洢����������
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetBatteryManufacturerName(IN int index, OUT wstring& manufacturer);

        /// @brief ��ȡ���ΨһID
        /// @param[in] index
        /// @param[out] uniqueID �洢ΨһID
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetBatteryUniqueID(IN int index, OUT wstring& uniqueID);

        /// @brief ��ȡ������к�
        /// @param[in] index 
        /// @param[out] serialNumber �洢���к�
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetBatterySerialNumber(IN int index, OUT wstring& serialNumber);

        /// @brief ��ȡ�����Ƶ���
        /// @param[in] index
        /// @param[out] capacity �洢����ֵ, ��λmWh
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetBatteryDesignedCapacity(IN int index, OUT unsigned long& capacity);

    private:
        LWMICoreManager* m_pWMICoreManager; ///< WMI���Ĺ�����
    private:
        LBatteryStaticDataManager(const LBatteryStaticDataManager&);
        LBatteryStaticDataManager& operator = (const LBatteryStaticDataManager&);
    };

    /// @brief �����������������
    class LBatteryFullCapacityManager
    {
    public:
        LBatteryFullCapacityManager();
        ~LBatteryFullCapacityManager();

        /// @brief ��ȡ�������
        /// @return �������
        int GetBatteryCount();

        /// @brief ��ȡ�������������
        /// @param[in] index
        /// @param[out] capacity ����, ��λmWh
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetBatteryFullChargedCapacity(IN int index, OUT unsigned long& capacity);

    private:
        LWMICoreManager* m_pWMICoreManager; ///< WMI���Ĺ�����
    private:
        LBatteryFullCapacityManager(const LBatteryFullCapacityManager&);
        LBatteryFullCapacityManager& operator = (const LBatteryFullCapacityManager&);
    };

    /// @brief ����������������
    class LNetworkAdapterManager
    {
    public:
        LNetworkAdapterManager();
        ~LNetworkAdapterManager();

        /// @brief  ��ȡ��������
        /// @return ��������
        int GetNetworkCardCount();

        /// @brief ��ȡ��������
        /// @param[in] index ��������
        /// @param[out] name �洢��������
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetNetworkCardName(IN int index, OUT wstring& name);

        /// @brief ��ȡ����������
        /// @param[in] idex ��������
        /// @param[out] manufacturer �洢����������
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetNetworkCardManufacturer(IN int index, OUT wstring& manufacturer);

        /// @brief ��ȡ����MAC��ַ
        /// @param[in] index ��������
        /// @param[out] mac �洢MAC��ַ
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetNetworkCardMACAddress(IN int index, OUT wstring& mac);

        /// @brief ��ȡ��������ID
        /// ��Wi-Fi, Ethernet
        /// @param[in] index ��������
        /// @param[out] connectionID ����ID
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetNetworkCardConnectionID(IN int index, OUT wstring& connectionID);

        /// @brief ��ȡ����GUID
        /// @param[in] index ��������
        /// @param[out] guid �洢����GUID
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetNetworkCardGUID(IN int index, OUT wstring& guid);

        /// @brief ��ȡ����PNP�豸ID
        /// @param[in] index ��������
        /// @param[in] OUT wstring & pnpDeviceID
        /// @return
        bool GetNetworkCardPNPDeviceID(IN int index, OUT wstring& pnpDeviceID);

    private:
        LWMICoreManager* m_pWMICoreManager; ///< WMI���Ĺ�����
    private:
        LNetworkAdapterManager(const LNetworkAdapterManager&);
        LNetworkAdapterManager& operator = (const LNetworkAdapterManager&);
    };

}

#endif