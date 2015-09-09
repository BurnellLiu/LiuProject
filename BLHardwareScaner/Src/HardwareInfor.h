

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
struct BaseBoardInfor
{
    wstring Description; ///< ����
    wstring Manufacturer; ///< ������
    wstring SerialNumber; ///< ���к�

    wstring BiosSerialNumber; ///< Bios���к�
    wstring BiosVersion; ///< Bios�汾
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
struct DisplayCardInforArray
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
    wstring DeviceID[MAX_DISK_NUMBER]; ///< �����豸ID
    wstring PNPDeviceID[MAX_DISK_NUMBER]; ///< �����豸ʵ��·��
    wstring InterfaceType[MAX_DISK_NUMBER]; ///< ���̽ӿ�����(SCSI, HDC, IDE, USB 1394)
    DISK_TYPE DiskType[MAX_DISK_NUMBER]; ///< ��������
};

/// @brief �����ʾ������
#define MAX_MONITOR_NUMBER 8

/// @brief ��ʾ����Ϣ��
struct MonitorInforArray
{
    unsigned long Count; ///< ��ʾ������
    string Name[MAX_MONITOR_NUMBER]; ///< ��ʾ������
    string Date[MAX_MONITOR_NUMBER]; ////< ��ʾ����������
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
    const BaseBoardInfor& GetBaseBoardInfor() const;

    /// @brief ��ȡ��������Ϣ
    /// @return ��������Ϣ
    const ProcessorInfor& GetProcessorInfor() const;

    /// @brief ��ȡ�Կ���Ϣ
    /// @return �Կ���Ϣ
    const DisplayCardInforArray& GetDisplayCardInfor() const;

    /// @brief ��ȡ�����ڴ���Ϣ
    /// @return �����ڴ���Ϣ
    const PhysicalMemoryInforArray& GetPhysicalMemoryInfor() const;

    /// @brief ��ȡ������Ϣ
    /// @return ������Ϣ
    const DiskInforArray& GetDiskInfor();

    /// @brief ��ȡ��ʾ����Ϣ
    /// @return ��ʾ����Ϣ
    const MonitorInforArray& GetMonitorInfor();

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
    /// @param[out] baseBoardInfor ������Ϣ
    void ScanBaseBoardInfor(OUT BaseBoardInfor& baseBoardInfor);

    /// @brief ɨ�账������Ϣ
    /// @param[out] processorInfor �洢��������Ϣ
    void ScanProcessorInfor(OUT ProcessorInfor& processorInfor);

    /// @brief ɨ���Կ���Ϣ
    /// @param[out] displayCardInfor �洢�Կ���Ϣ
    void ScanDisplayCardInfor(OUT DisplayCardInforArray& displayCardInfor);

    /// @brief ɨ���Կ���Ϣ
    /// @param[out] physicalMemoryInfor �洢�����ڴ���Ϣ
    void ScanPhysicalMemoryInfor(OUT PhysicalMemoryInforArray& physicalMemoryInfor);

    /// @brief ɨ�������Ϣ
    /// @param[out] diskInfor �洢������Ϣ
    void ScanDiskInfor(OUT DiskInforArray& diskInfor);

    /// @brief ɨ����ʾ����Ϣ
    /// @param[out] monitorInfor �洢��ʾ����Ϣ
    void ScanMonitorInfor(OUT MonitorInforArray& monitorInfor);

    HardwareInfor(); // ��ֹ����
    HardwareInfor(const HardwareInfor&); // ��ֹĬ�Ͽ������캯��
    HardwareInfor& operator = (const HardwareInfor&); // ��ֹ��ֵ������

private:
    ComputerSystemInfor m_computerSystemInfor; ///< �����ϵͳ��Ϣ
    OperatingSystemInfor m_operatingSystemInfor; ///< ����ϵͳ��Ϣ
    BaseBoardInfor m_baseBoardInfor; ///< ������Ϣ
    ProcessorInfor m_processorInfor; ///< ��������Ϣ
    DisplayCardInforArray m_displayCardInfor; ///< �Կ���Ϣ
    PhysicalMemoryInforArray m_physicalMemoryInfor; ///< �����ڴ���Ϣ
    DiskInforArray m_diskInfor; ///< ������Ϣ
    MonitorInforArray m_monitorInfor; ////< ��ʾ����Ϣ
};

#endif