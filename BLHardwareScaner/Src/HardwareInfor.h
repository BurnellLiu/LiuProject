

#ifndef _HARDWAREINFOR_H_
#define _HARDWAREINFOR_H_

#include <string>
using std::string;
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

/// @brief ��ȡ���������
enum COMPUTER_TYPE
{
    COMPUTER_TYPE_UNKNOWN = 0, ///< δ֪
    COMPUTER_TYPE_DESKTOP = 1, ///< ̨ʽ��
    COMPUTER_TYPE_NOTE_BOOK = 2, ///< �ʼǱ�
    COMPUTER_TYPE_TABLET = 3 ///< ƽ�����
};

/// @brief �����ϵͳ��Ϣ�ṹ
struct ComputerSystemInfor
{
    wstring ModelName; ////< ������
    wstring Manufacturer; ///< ������
    COMPUTER_TYPE Type; ///< ���������
};

/// @brief ����ϵͳ��Ϣ�ṹ
struct OperatingSystemInfor
{
    wstring Caption; ////< ����
    wstring Architecture; ////< �ܹ�
    wstring Version; ////< �汾
    wstring SystemDrive; ////< ϵͳ��
};

/// @brief ������Ϣ�ṹ
struct MotherBoardInfor
{
    wstring ProductName; ///< ��Ʒ��
    wstring Manufacturer; ///< ������
    wstring SerialNumber; ///< ���к�

    wstring BiosSerialNumber; ///< Bios���к�
    wstring BiosVersion; ///< Bios�汾
    wstring BiosReleaseDate; ///< Bios��������
    wstring BiosVendor; ///< Bios����
};

/// @brief ��������Ϣ
struct ProcessorInfor
{
    wstring Name; ///< ����
    wstring Description; ///< ����
    wstring Manufacturer; ///< ������
    unsigned long CoresNumber; ///< ������
    unsigned long LogicalProcessorNumber; ///< �߼�����������
    unsigned long MaxClockSpeed; ///< ���Ƶ��(MHz)
};

/// @brief �Կ�����
enum DISPLAY_CARD_TYPE
{
    DISPLAY_CARD_UNKNOWN = 0, ///< δ֪
    DISPLAY_CARD_INTERNAL, ///< �����Կ�
    DISPLAY_CARD_EXTERNAL ///< �����Կ�
};

/// @brief ����Կ�����
#define MAX_DISPLAYCARD_NUMBER 4

/// @brief �Կ���Ϣ��
struct VideoCardInforArray
{
    unsigned long Count; ///< �Կ�����
    wstring Description[MAX_DISPLAYCARD_NUMBER]; ///< �Կ�����
    DISPLAY_CARD_TYPE Type[MAX_DISPLAYCARD_NUMBER]; ///< �Կ�����
    unsigned long RAMSize[MAX_DISPLAYCARD_NUMBER]; ///< �Դ��С, ��λ(M)
};

/// @brief ��������ڴ�����
#define MAX_PHYSICAL_MEMORY_NUMBER 8

/// @brief �ڴ���Ϣ��
struct PhysicalMemoryInforArray
{
    unsigned long Count; ///< �ڴ�����
    wstring Manufacturer[MAX_PHYSICAL_MEMORY_NUMBER]; ///< ������
    wstring SerialNumber[MAX_PHYSICAL_MEMORY_NUMBER]; ///< ���к�
    wstring PartNumbe[MAX_PHYSICAL_MEMORY_NUMBER]; ///< �ͺ�
    unsigned long Capacity[MAX_PHYSICAL_MEMORY_NUMBER]; ///< �ڴ�����, ��λ(M)
    unsigned long Speed[MAX_PHYSICAL_MEMORY_NUMBER]; ///< �ڴ�Ƶ��
};

/// @brief ����������������
#define MAX_DISK_NUMBER 8

/// @brief ��������
enum DISK_TYPE
{
    UNKNOWN_DISK = 0, // δ֪����
    FIXED_IDE_DISK = 1, // �̶�����(�籾��Ӳ��)
    EXTERNAL_USB_DISK = 2, // ��չ����(��USB�ƶ�Ӳ��)
    VIRTUAL_DISK = 3, // ����Ӳ��(��VHD)
    USB_FLASH_DISK = 4, // U��
    SD_CARD_DISK = 5// SD��
};

/// @brief ������Ϣ��
struct DiskInforArray 
{
    unsigned long Count; ///< ��������
    wstring Model[MAX_DISK_NUMBER]; ///< �����ͺ�
    wstring SerialNumber[MAX_DISK_NUMBER]; ///< �������к�
    unsigned long TotalSize[MAX_DISK_NUMBER]; ///< �����ܴ�С, ��λ(G)
    wstring InterfaceType[MAX_DISK_NUMBER]; ///< ���̽ӿ�����(SCSI, HDC, IDE, USB 1394)
    DISK_TYPE DiskType[MAX_DISK_NUMBER]; ///< ��������

    bool IsATA[MAX_DISK_NUMBER]; ///< ��ʶ�Ƿ�ΪATA�ӿ�Ӳ��
    struct  
    {
        unsigned long RotationRate; ///< ����ת��, ���ֵΪ1��ʾΪ��̬Ӳ��(SSD), 0��ʾ��ȡʧ��
        unsigned long SATAType; ///< SATA�ӿ�����, 1(SATA1.0 1.5Gb/s), 2(SATA2.0 3.0Gb/s), 3(SATA3.0 6.0Gb/s), 0(��ȡʧ��)
        unsigned long PowerOnHours; ///< ͨ����ʱ��, ��λhours(Сʱ)
    }ATAInfor[MAX_DISK_NUMBER]; ///< ֻ��IDE(ATA)�ӿڵ�Ӳ�̲�Ӧ��ʹ�øýṹ�е�ֵ
};

/// @brief �����ʾ������
#define MAX_MONITOR_NUMBER 8

/// @brief ��ʾ����Ϣ��
struct MonitorInforArray
{
    unsigned long Count; ///< ��ʾ������
    wstring Name[MAX_MONITOR_NUMBER]; ///< ��ʾ������
    wstring Date[MAX_MONITOR_NUMBER]; ////< ��ʾ����������
};

/// @brief ��ؾ�̬��Ϣ
struct BatteryStaticInfor
{
    bool Exist; ///< ��ʶ�Ƿ���ڵ��
    wstring Name; ///< �����
    wstring Manufacturer; ///< ���������
    wstring SerialNumber; ///< ������к�

    unsigned long DesignedCapacity; ///< �������, ��λmWh
    unsigned long FullChargedCapacity; ///< ����������. ��λmWh
    unsigned long DesignedVoltage; ///< ��Ƶ�ѹ, ��λmV
};

/// @brief �����������
#define MAX_NETWORKCARD_NUMBER 16

/// @brief ��������
enum NETWORKCARD_TYPE
{
    UNKNOWN_NETCARD = 0, ///< δ֪����
    WIFI_NETCARD = 1, ///< ��������
    ETHERNET_NETCARD = 2, ///< ����
    BLUETOOTH_NETCARD = 3 ///< ������ 
};

/// @brief ������Ϣ��
struct NetworkCardInforArray
{
    unsigned long Count; ///< ��������
    NETWORKCARD_TYPE Type[MAX_NETWORKCARD_NUMBER]; ///< ��������
    wstring Name[MAX_NETWORKCARD_NUMBER]; ///< ��������
    wstring Manufacturer[MAX_NETWORKCARD_NUMBER]; ///< ����������
    wstring MACAddress[MAX_NETWORKCARD_NUMBER]; ///< MAC��ַ
};

/// @brief �����������
#define MAX_CDROMDRIVE_NUMBER 8

/// @brief ������Ϣ��
struct CDRomDriveInforArray
{
    unsigned long Count; ///< ��������
    wstring Name[MAX_CDROMDRIVE_NUMBER]; ///< ��������
};

/// @brief Ӳ����Ϣ��
///
/// �����ȡ�Ķ��ǹ̶���Ϣ
class HardwareInfor
{
public:
    /// @brief ��ȡʵ������
    /// @return ʵ������
    static HardwareInfor& GetInstance();

    /// @brief ��ȡ�����ϵͳ��Ϣ
    /// @return �����ϵͳ��Ϣ
    const ComputerSystemInfor& GetComputerSystemInfor() const; 

    /// @brief ��ȡ����ϵͳ��Ϣ
    /// @return ����ϵͳ��Ϣ
    const OperatingSystemInfor& GetOperatingSystemInfor() const;

    /// @brief ��ȡ������Ϣ
    /// @return ������Ϣ
    const MotherBoardInfor& GetMotherBoardInfor() const;

    /// @brief ��ȡ��������Ϣ
    /// @return ��������Ϣ
    const ProcessorInfor& GetProcessorInfor() const;

    /// @brief ��ȡ�Կ���Ϣ
    /// @return �Կ���Ϣ
    const VideoCardInforArray& GetVideoCardInfor() const;

    /// @brief ��ȡ�����ڴ���Ϣ
    /// @return �����ڴ���Ϣ
    const PhysicalMemoryInforArray& GetPhysicalMemoryInfor() const;

    /// @brief ��ȡ������Ϣ
    /// @return ������Ϣ
    const DiskInforArray& GetDiskInfor();

    /// @brief ��ȡ��ʾ����Ϣ
    /// @return ��ʾ����Ϣ
    const MonitorInforArray& GetMonitorInfor();

    /// @brief ��ȡ��ؾ�̬��Ϣ
    /// @return ��ؾ�̬��Ϣ
    const BatteryStaticInfor& GetBatteryStaticInfor();

    /// @brief ��ȡ������Ϣ
    /// @return ������Ϣ
    const NetworkCardInforArray& GetNetworkCardInfor();

    /// @brief ��ȡ������Ϣ
    /// @return ������Ϣ
    const CDRomDriveInforArray& GetCDRomDriveInfor();

    /// @brief ��������
    ~HardwareInfor();

private:
    /// @brief ɨ��Ӳ����Ϣ
    void Scan();
    
    /// @brief ɨ������ϵͳ��Ϣ
    /// @param[out] computerSystemInfor �洢�����ϵͳ��Ϣ
    void ScanComputerSystemInfor(OUT ComputerSystemInfor& computerSystemInfor);

    /// @brief ɨ�����ϵͳ��Ϣ
    /// @param[out] operatingSystemInfor ����ϵͳ��Ϣ
    void ScanOperatingSystemInfor(OUT OperatingSystemInfor& operatingSystemInfor);

    /// @brief ɨ��������Ϣ
    /// @param[out] motherBoardInfor ������Ϣ
    void ScanMotherBoardInfor(OUT MotherBoardInfor& baseBoardInfor);

    /// @brief ɨ�账������Ϣ
    /// @param[out] processorInfor �洢��������Ϣ
    void ScanProcessorInfor(OUT ProcessorInfor& processorInfor);

    /// @brief ɨ���Կ���Ϣ
    /// @param[out] videoCardInfor �洢�Կ���Ϣ
    void ScanVideoCardInfor(OUT VideoCardInforArray& displayCardInfor);

    /// @brief ɨ���Կ���Ϣ
    /// @param[out] physicalMemoryInfor �洢�����ڴ���Ϣ
    void ScanPhysicalMemoryInfor(OUT PhysicalMemoryInforArray& physicalMemoryInfor);

    /// @brief ɨ�������Ϣ
    /// @param[out] diskInfor �洢������Ϣ
    void ScanDiskInfor(OUT DiskInforArray& diskInfor);

    /// @brief ɨ����ʾ����Ϣ
    /// @param[out] monitorInfor �洢��ʾ����Ϣ
    void ScanMonitorInfor(OUT MonitorInforArray& monitorInfor);

    /// @brief ɨ���ؾ�̬��Ϣ
    /// @param[out] batteryStaticInfor �洢��ؾ�̬��Ϣ
    void ScanBatteryStaticInfor(OUT BatteryStaticInfor& batteryStaticInfor);

    /// @brief ɨ��������Ϣ
    /// @param[out] networkCardInfor �洢������Ϣ
    void ScanNetworkCardInfor(OUT NetworkCardInforArray& networkCardInfor);

    /// @brief ɨ�������Ϣ
    /// @param[out] cdRomDrive �洢������Ϣ
    void ScanCDRomDriveInfor(OUT CDRomDriveInforArray& cdRomDriveInfor);

    HardwareInfor(); // ��ֹ����
    HardwareInfor(const HardwareInfor&); // ��ֹĬ�Ͽ������캯��
    HardwareInfor& operator = (const HardwareInfor&); // ��ֹ��ֵ������

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
};

#endif