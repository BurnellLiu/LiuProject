
#include "LDiskController.h"


#include <string>
using std::string;

#include <Windows.h>
#include <ntddscsi.h>


/// @brief �洢�豸����
struct SStorageDeviceProperty
{
    bool RemovableMedia; // �Ƿ�Ϊ���ƶ�ý��
    UCHAR DeviceType; // �豸����, ��SCSI�淶����
    UCHAR DeviceTypeModifier; // �豸��������, ��SCSI�淶����, Ϊ0��ʾû�и�ֵ
    STORAGE_BUS_TYPE BusType; // ��������
    string VendorId; // ����ID
    string ProductId; // ��ƷID
    string ProductRevision; // ��Ʒ�޶�
    string SerialNumber; // ���к�
};

// ���½ṹȡ���ڴ����
#pragma pack (1) // ȡ���ڴ����

/// @brief ATA8 ID�ṹ
///
/// ��512�ֽ�(256��WORD)�������������һЩ����Ȥ����(����������ATA8-ACS)
/// ע��:�ýṹ�е��ַ����ַ������ߵ� ԭ����ATA/ATAPI�е�WORD����Windows���õ��ֽ�˳���෴
struct SATA8IdentifyData
{
    USHORT GeneralConfig; // WORD 0: ������Ϣ��
    USHORT Obsolete1; // WORD 1: ����
    USHORT SpecConfig; // WORD 2: ��������
    USHORT Obsolete3; // WORD 3: ����
    USHORT Obsolete4; // WORD 4: ����
    USHORT Obsolete5; // WORD 5: ����
    USHORT Obsolete6; // WORD 6: ����
    USHORT CompactFlashReserved[2]; // WORD 7-8: ����
    USHORT Obsolete9; // WORD 9: ����
    CHAR SerialNumber[20]; // WORD 10-19:���к�
    USHORT Obsolete20[3]; // WORD 20-22: ����
    CHAR FirmwareRev[8]; // WORD 23-26: �̼��汾
    CHAR ModelNumber[40]; // WORD 27-46: �ͺ�
    USHORT Reserved47; // WORD 47: ����
    USHORT Reserved48; // WORD 48: ����
    struct 
    {
        USHORT Obsolete0:8; // bit 0-7: ����
        USHORT DMASupport:1; // bit 8: 1=֧��DMA
        USHORT LBASupport:1; // bit 9: 1=֧��LBA
        USHORT IORDYDisabled:1; // bit 10: 1=IORDY���Ա�����
        USHORT IORDYSupport:1; // bit 11: 1=֧��IORDY
        USHORT Reserved12:4; // bit 12-15: ����
    }Capabilities; // WORD 49: һ������
    USHORT Reserved50; // WORD 50: ����
    USHORT Obsolete51; // WORD 51: ����
    USHORT Obsolete52; // WORD 52: ����
    USHORT Reserved53; // WORD 53: ����
    USHORT Obsolete54[5]; // WORD 54-58: ����
    USHORT Reserved59; // WORD 59: ����
    ULONG LBATotalSectors; // WORD 60-61: LBA��Ѱַ��������
    USHORT Obsolete62; // WORD 62: ����
    struct 
    {
        USHORT Mode0:1; // bit 0: 1=֧��ģʽ0 (4.17Mb/s)
        USHORT Mode1:1; // bit 1: 1=֧��ģʽ1 (13.3Mb/s)
        USHORT Mode2:1; // bit 2: 1=֧��ģʽ2 (16.7Mb/s)
        USHORT Reserved5:5; // bit 3-7: ����
        USHORT Mode0Sel:1; // bit8: 1=��ѡ��ģʽ0
        USHORT Mode1Sel:1; // bit9: 1=��ѡ��ģʽ1
        USHORT Mode2Sel:1; // bit10: 1=��ѡ��ģʽ2
        USHORT Reserved11:5; // bit 11-15: ����
    } MultiWordDMA; // WORD 63: ���ֽ�DMA֧������
    struct 
    {
        USHORT AdvPOIModes:8; // bit 0-7: ֧�ָ߼�POIģʽ��
        USHORT Reserved8:8; // bit 8-15: ����
    } PIOCapacity; // WORD 64: �߼�PIO֧������
    USHORT MinMultiWordDMACycle; // WORD 65: ���ֽ�DMA�������ڵ���Сֵ
    USHORT RecMultiWordDMACycle; // WORD 66: ���ֽ�DMA�������ڵĽ���ֵ
    USHORT MinPIONoFlowCycle; // WORD 67: ��������ʱPIO�������ڵ���Сֵ
    USHORT MinPIOFlowCycle; // WORD 68: ��������ʱPIO�������ڵ���Сֵ
    USHORT Reserved69[7]; // WORD 69-75: ����
    struct
    {
        USHORT Reserved0:1; // bit 0: ����
        USHORT SATAGen1:1; // bit1: 1=֧��SATA Gen1(1.5Gb/s)
        USHORT SATAGen2:1; // bit2: 1=֧��SATA Gen2(3.0Gb/s)
        USHORT SATAGen3:1; // bit3: 1=֧��SATA Gen3(6.0Gb/s)
        USHORT Reserved4:12; // bit4-15: ����
    }SATACapabilities; // WORD 76: SATA����
    USHORT Reserved77; // WORD 77: ����
    struct
    {
        USHORT Reserved0: 1; // bit0: Ӧ��Ϊ0
        USHORT NoneZeroBufferOffsets: 1; // bit1: 1=�豸֧�ַ�0����ƫ��
        USHORT DMASetupAutoActivation: 1; // bit2:
        USHORT InitiatePowerManagement: 1; // bit3: 1=�豸֧�ַ����Դ����
        USHORT InorderDataDelivery: 1; // bit4:
        USHORT Reserved11: 11; // bit5-15: ����
    }SATAFeaturesSupported; // WORD 78: SATA����֧��
    struct
    {
        USHORT Reserved0: 1; // bit0: Ӧ��Ϊ0
        USHORT NoneZeroBufferOffsets: 1; // bit1: 1=��0����ƫ�ƿ���
        USHORT DMASetupAutoActivation: 1; // bit2:
        USHORT InitiatePowerManagement: 1; // bit3: 1=�����Դ������
        USHORT InorderDataDelivery: 1; // bit4:
        USHORT Reserved11: 11; // bit5-15: ����
    }SATAFeaturesEnabled; // WORD 79: SATA��������
    struct 
    {
        USHORT Reserved0:1; // bit0: ����
        USHORT Obsolete1:3; // bit1-3: ����
        USHORT ATA4:1; // bit4: 1=֧��ATA/ATAPI-4
        USHORT ATA5:1; // bit5: 1=֧��ATA/ATAPI-5
        USHORT ATA6:1; // bit6: 1=֧��ATA/ATAPI-6
        USHORT ATA7:1; // bit7: 1=֧��ATA/ATAPI-7
        USHORT ATA8:1; // bit8: 1=֧��ATA8-ACS
        USHORT Reserved9:7; // bit9-15: ����
    } MajorVersion; // WORD 80: ���汾
    USHORT MinorVersion; // WORD 81: ���汾
    USHORT Reserved82;// WORD 82: ����
    struct 
    {
        USHORT Reserved0:3; // bit0-2: ����
        USHORT AdvancedPowerManagementFeatureSetSupported:1; // bit3: 1=�߼���Դ����������֧��
        USHORT Reserved4:12; // bit4-15: ����
    }CommandSetsSupported; // WORD 83: ���֧��
    USHORT Reserved84[2]; // WORD 84-85: ����
    struct 
    {
        USHORT Reserved0:3; // bit0-2: ����
        USHORT AdvancedPowerManagementFeatureSetEnabled:1; // bit3: 1=�߼���Դ��������������
        USHORT Reserved4:12; // bit4-15: ����
    }CommandSetFeatureEnabledSupported;  // WORD 86: ���������������֧��
    USHORT Reserved87; // WORD 87: ����
    struct 
    {
        USHORT Mode0:1;                // 1=֧��ģʽ0 (16.7Mb/s)
        USHORT Mode1:1;                // 1=֧��ģʽ1 (25Mb/s)
        USHORT Mode2:1;                // 1=֧��ģʽ2 (33Mb/s)
        USHORT Mode3:1;                // 1=֧��ģʽ3 (44Mb/s)
        USHORT Mode4:1;                // 1=֧��ģʽ4 (66Mb/s)
        USHORT Mode5:1;                // 1=֧��ģʽ5 (100Mb/s)
        USHORT Mode6:1;                // 1=֧��ģʽ6 (133Mb/s)
        USHORT Reserved7:1;          // ����
        USHORT Mode0Sel:1;             // 1=��ѡ��ģʽ0
        USHORT Mode1Sel:1;             // 1=��ѡ��ģʽ1
        USHORT Mode2Sel:1;             // 1=��ѡ��ģʽ2
        USHORT Mode3Sel:1;             // 1=��ѡ��ģʽ3
        USHORT Mode4Sel:1;             // 1=��ѡ��ģʽ4
        USHORT Mode5Sel:1;             // 1=��ѡ��ģʽ5
        USHORT Mode6Sel:1;             // 1=��ѡ��ģʽ6
        USHORT Reserved15:1;          // ����
    } UltraDMA; // WORD 88:  Ultra DMA֧������
    USHORT Reserved89[2];         // WORD 89-90: ����
    struct
    {
        BYTE LevelValue; // �߼���Դ������ֵ
        BYTE Reserved;
    }AdvancePowerManagementLevel; // WORD 91: �߼���Դ������
    USHORT Reserved92[125];         // WORD 92-216
    USHORT NominalMediaRotationRate; //  WORD 217 �궨ת��(RPM), ���ֵΪ1��ʾΪSSD��������
    USHORT Reserved218[38]; // WORD 218-255 ����
};

/// @brief SMARTÿ�����Խṹ
struct SSMARTAttribute
{
    BYTE Id; // ����ID
    WORD StatusFlags; // ״ֵ̬
    BYTE Current; // ��ǰֵ
    BYTE Worst; // ���ֵ
    BYTE RawValue[6]; // ��ʵֵ
    BYTE Reserved; // ����
};

/// @brief SMART���ݽṹ
struct SSMARTData
{
    WORD Version; // SMART�汾
    SSMARTAttribute AttributeArray[30]; // SMART��������, ���30��
};

#pragma pack () // �ָ��ڴ����

/// @brief ͨ�ô洢������
///
///https://msdn.microsoft.com/en-us/library/windows/hardware/ff553891(v=vs.85).aspx
class CGeneralStorageController
{
public:
    /// @brief ���캯��
    /// @param[in] devicePath �豸·��
    /// �豸·����ʽΪ(C����)"////.//DeviceName"
    /// �豸������: 
    /// Ӳ���߼�����: C:, D:, E:, ...
    /// ����������: PhysicalDrive0, PhysicalDrive1, ...
    /// CD-ROM, DVD/ROM: CDROM0, CDROM1, ...
    explicit CGeneralStorageController(IN const wstring& devicePath);
    virtual ~CGeneralStorageController();

    /// @brief �ж��豸�Ƿ����
    /// @return ���ڷ���true, �����ڷ���false
    bool DeviceExist();

    /// @brief �����豸·��
    /// @param[in] devicePath �豸·��
    void ResetDeviceId(IN const wstring& devicePath);

    /// @brief ��ȡ�豸����
    /// @param[out] devicePrperty �洢�豸����
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GetDeviceProperty(OUT SStorageDeviceProperty& devicePrperty);

    /// @brief ��ȡý������
    /// @param[in] mediaInfo �洢ý����Ϣ
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GetMediaType(OUT DEVICE_MEDIA_INFO& mediaInfo);

protected:
    /// @brief ���豸���
    /// @return �����豸���
    HANDLE OpenDeviceHandle();

private:
    wstring m_devicePath; ///< �豸����

private:
    CGeneralStorageController(const CGeneralStorageController&);
    CGeneralStorageController& operator = (const CGeneralStorageController&);
};

/// @brief ���̿�����
///
///https://msdn.microsoft.com/en-us/library/windows/hardware/ff552626(v=vs.85).aspx
class CDiskController : public CGeneralStorageController
{
public:
    /// @brief ���캯��
    /// @param[in] devicePath �豸·��
    /// �豸·����ʽΪ(C����)"////.//DeviceName"
    /// �豸������: PhysicalDrive0, PhysicalDrive1, ...
    CDiskController(IN const wstring& devicePath);
    virtual ~CDiskController();

    /// @brief ��ȡ���̼�����Ϣ
    /// @param[in] geometry �洢������Ϣ
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GetGeometry(OUT DISK_GEOMETRY& geometry);

    /// @brief ��ȡ�汾��Ϣ
    ///
    /// �÷���Ҫ�����֧��SMART
    /// @param[in] versionParams �洢������Ϣ
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GetVersionInfor(OUT GETVERSIONINPARAMS& versionParams);
};

/// @brief IDE(ATA)���̿�����
///
/// https://msdn.microsoft.com/en-us/library/windows/hardware/ff559105(v=vs.85).aspx
/// ATA8-ACS
class CIDEDiskController : public CDiskController
{
public:
    /// @brief ���캯��
    /// @param[in] devicePath �豸·��
    /// �豸·����ʽΪ(C����)"////.//DeviceName"
    /// �豸������: PhysicalDrive0, PhysicalDrive1, ...
    CIDEDiskController(IN const wstring& devicePath);
    virtual ~CIDEDiskController();

    bool ATACmdCheckMediaCardType();

    /// @brief ����Դģʽ
    /// @param[out] mode �洢ģʽ��ֵ
    /// mode ��ֵ����: (����ATA8 SPE)
    /// 0x00 Device is in Standby mode [ʡ��ģʽ]
    /// 0x40 Device is in NV Cache Power Mode and spindle is spun down or spinning down
    /// 0x41 Device is in NV Cache Power Mode and spindle is spun up or spinning up
    /// 0x80 Device is in Idle mode
    /// 0xFF Device is in Active mode or Idle mode
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool ATACmdCheckPowerMode(OUT BYTE& mode);

    /// @brief ֱ�����õ�ԴģʽΪIDLE
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool ATACmdIDLEImmediate();

    /// @brief ֱ�����õ�ԴģʽΪStandby
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool ATACmdStandbyImmediate();

    /// @brief ˯������
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool ATACmdSleep();

    /// @brief ��������
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool ATACmdDeviceReset();

    /// @brief �������
    /// @param[out] state �洢״ֵ̬
    /// state ��ֵΪ0x01��0x81���ʾ�豸�Ǻõ�, �����ʾ�豸�ǻ��Ļ���û������
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool ATACmdExecuteDeviceDiagnostic(OUT BYTE& state);

    /// @brief �����������չ����(û�з��ض�ȡ������, For LBA48)
    /// @param[in] lbaAddress ��Ҫ��ȡ����ʼ������ʼ��ַ
    /// @param[in] sectorCount ��Ҫ��ȡ������������
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool ATACmdReadVerifySectorExt(IN ULONGLONG lbaAddress, IN unsigned long sectorCount);

    /// @brief ��ȡID��Ϣ����
    /// @param[out] identifyData �洢ID��Ϣ
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool ATACmdIdentifyDevice(OUT SATA8IdentifyData& identifyData);

    /// @brief ��ȡSMART��������
    ///
    /// SMART���Զ���: http://en.wikipedia.org/wiki/S.M.A.R.T.
    /// @param[out] smartData �洢SMART����
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool ATACmdSMARTReadData(OUT SSMARTData& smartData);
private:
    /// @brief ATA����
    enum ATA_COMMAND
    {
        ATA_DEVICE_RESET = 0x08, // ��������
        ATA_READ_VERIFY_SECTOR_EXT = 0x42, // �������չ����
        ATA_EXECUTE_DEVICE_DIAGNOSTIC = 0x90, // �������
        ATA_CHECK_MEDIA_CARD_TYPE = 0xD1, // ���ý����������
        ATA_STANDBY_IMMEDIATE = 0xE0, // �������õ�ԴģʽΪSTANDBY����
        ATA_IDELE_IMMEDIATE = 0xE1, // �������õ�ԴģʽΪIDELE����
        ATA_IDENTIFY_DEVICE = 0xEC, // ��ȡID��Ϣ����
        ATA_CHECK_POWER_MODE = 0xE5, // ����Դģʽ����
        ATA_SLEEP = 0xE6, // ��������
        ATA_SET_FEATURE = 0xFE // ������������
    };

    /// @brief ATA������������������
    enum SET_FEATURE_SUB_COMMAND
    {
        SET_FEATURE_ENABLE_ADVANCED_POWER_MANAGEMENT = 0x05, // �����߼���Դ����
        SET_FEATURE_DISABLE_ADVANCED_POWER_MANAGEMENT = 0x85 // �رո߼���Դ����
    };
};

CGeneralStorageController::CGeneralStorageController(IN const wstring& devicePath)
    : m_devicePath(devicePath)
{
}

CGeneralStorageController::~CGeneralStorageController()
{

}

bool CGeneralStorageController::DeviceExist()
{
    HANDLE hDevice =  this->OpenDeviceHandle();

    if (hDevice == INVALID_HANDLE_VALUE || hDevice == NULL)
    {
        return false;
    }

    if (hDevice != INVALID_HANDLE_VALUE && hDevice != NULL)
    {
        CloseHandle(hDevice);
        hDevice = NULL;

        return true;
    }

    return true;
}

void CGeneralStorageController::ResetDeviceId(IN const wstring& devicePath)
{
    m_devicePath = devicePath;
}

bool CGeneralStorageController::GetDeviceProperty(OUT SStorageDeviceProperty& devicePrperty)
{
    BOOL nRet = FALSE; // ϵͳAPI����ֵ
    bool bRet = false; // ��������ֵ
    HANDLE hDevice = NULL; // �豸���
    ULONG nBytes; // �洢���ص��ֽ���
    STORAGE_PROPERTY_QUERY propertyQuery; // ��ѯ������
    STORAGE_DEVICE_DESCRIPTOR deviceDescriptor; // ���������ṹ
    STORAGE_DEVICE_DESCRIPTOR* pDeviceDescriptor = NULL; // �洢��ѯ��������

    // ���豸
    hDevice = this->OpenDeviceHandle();
    if (hDevice == INVALID_HANDLE_VALUE || hDevice == NULL)
    {
        bRet = false;
        goto SAFE_EXIT;
    }

    // �����Ҫ��ѯ������
    ZeroMemory(&propertyQuery, sizeof(propertyQuery));
    propertyQuery.PropertyId = StorageDeviceProperty;
    propertyQuery.QueryType = PropertyStandardQuery;

    // ��һ�β�ѯ, ��ȡ������Ҫ���ֽ���
    nRet = DeviceIoControl(
        hDevice, 
        IOCTL_STORAGE_QUERY_PROPERTY, 
        &propertyQuery, 
        sizeof(propertyQuery), 
        &deviceDescriptor, 
        sizeof(deviceDescriptor), 
        &nBytes, 
        NULL);

    if (nRet == FALSE)
    {
        bRet = false;
        goto SAFE_EXIT;
    }

    pDeviceDescriptor = (STORAGE_DEVICE_DESCRIPTOR*)malloc(deviceDescriptor.Size);

    // �ڶ��β�ѯ, ��ȡ����
    nRet = DeviceIoControl(
        hDevice, 
        IOCTL_STORAGE_QUERY_PROPERTY, 
        &propertyQuery, 
        sizeof(propertyQuery), 
        pDeviceDescriptor, 
        deviceDescriptor.Size, 
        &nBytes, 
        NULL);

    if (nRet == FALSE)
    {
        bRet = false;
        goto SAFE_EXIT;
    }

    bRet = true;

    devicePrperty.BusType = pDeviceDescriptor->BusType;
    devicePrperty.DeviceType = pDeviceDescriptor->DeviceType;
    devicePrperty.DeviceTypeModifier = pDeviceDescriptor->DeviceTypeModifier;
    devicePrperty.RemovableMedia = pDeviceDescriptor->RemovableMedia == TRUE ? true : false;

    // ���к��Լ�����������Ҫ����ƫ����ȡ��, ���ƫ��ֵΪ0, ���ʾû�и�ֵ
    if (pDeviceDescriptor->SerialNumberOffset != 0)
    {
        devicePrperty.SerialNumber = (char*)pDeviceDescriptor + pDeviceDescriptor->SerialNumberOffset;
    }
    if (pDeviceDescriptor->VendorIdOffset != 0)
    {
        devicePrperty.VendorId = (char*)pDeviceDescriptor + pDeviceDescriptor->VendorIdOffset;
    }
    if (pDeviceDescriptor->ProductIdOffset != 0)
    {
        devicePrperty.ProductId = (char*)pDeviceDescriptor + pDeviceDescriptor->ProductIdOffset;
    }
    if (pDeviceDescriptor->ProductRevisionOffset != 0)
    {
        devicePrperty.ProductRevision = (char*)pDeviceDescriptor + pDeviceDescriptor->ProductRevisionOffset;
    }

SAFE_EXIT:

    if (hDevice != INVALID_HANDLE_VALUE && hDevice != NULL)
    {
        CloseHandle(hDevice);
        hDevice = NULL;
    }

    if (pDeviceDescriptor != NULL)
    {
        free(pDeviceDescriptor);
        pDeviceDescriptor = NULL;
    }

    return bRet;
}

bool CGeneralStorageController::GetMediaType(OUT DEVICE_MEDIA_INFO& mediaInfo)
{
    BOOL nRet = FALSE; // ϵͳAPI����ֵ
    bool bRet = false; // ��������ֵ
    HANDLE hDevice = NULL; // �豸���
    ULONG nBytes = 0; // �洢���ص��ֽ���
    ULONG requiredBytes = 0; // �洢��Ҫ���ֽ���

    GET_MEDIA_TYPES mediaTypes; // ý�����ͽṹ
    GET_MEDIA_TYPES* pMediaTypes = NULL; // �洢ý������

    // ���豸
    hDevice = this->OpenDeviceHandle();
    if (hDevice == INVALID_HANDLE_VALUE || hDevice == NULL)
    {
        bRet = false;
        goto SAFE_EXIT;
    }

    // ��һ�β�ѯ��Ҫ�Ĵ�С
    nRet = DeviceIoControl(
        hDevice, 
        IOCTL_STORAGE_GET_MEDIA_TYPES_EX, 
        NULL, 
        0, 
        &mediaTypes, 
        sizeof(mediaTypes), 
        &nBytes, 
        NULL);

    if (nRet == FALSE)
    {
        bRet = false;
        goto SAFE_EXIT;
    }

    requiredBytes = sizeof(GET_MEDIA_TYPES) + mediaTypes.MediaInfoCount * sizeof(DEVICE_MEDIA_INFO);
    // �����㹻�Ŀռ�
    pMediaTypes = (GET_MEDIA_TYPES*)malloc(requiredBytes);

    // �ڶ��β�ѯ, ��ȡ����
    nRet = DeviceIoControl(
        hDevice, 
        IOCTL_STORAGE_GET_MEDIA_TYPES_EX, 
        NULL, 
        0, 
        pMediaTypes, 
        requiredBytes, 
        &nBytes, 
        NULL);

    // ��Ȼ���ܻ�ȡ������ý����Ϣ, ������ֻȡ��һ��, �Ժ��������������, 
    // ��������ر�����
    mediaInfo = *(pMediaTypes->MediaInfo);

    if (nRet == FALSE)
    {
        bRet = false;
        goto SAFE_EXIT;
    }

    bRet = true;

SAFE_EXIT:

    if (hDevice != INVALID_HANDLE_VALUE && hDevice != NULL)
    {
        CloseHandle(hDevice);
        hDevice = NULL;
    }

    if (pMediaTypes != NULL)
    {
        free(pMediaTypes);
        pMediaTypes = NULL;
    }

    return bRet;
}

HANDLE CGeneralStorageController::OpenDeviceHandle()
{
    HANDLE hDevice =  CreateFileW(
		m_devicePath.c_str(), 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ | FILE_SHARE_WRITE, 
		NULL, OPEN_EXISTING, 
		0, 
		NULL);
    return hDevice;
}

CDiskController::CDiskController(IN const wstring& devicePath)
    : CGeneralStorageController(devicePath)
{

}

CDiskController::~CDiskController()
{

}

bool CDiskController::GetGeometry(OUT DISK_GEOMETRY& geometry)
{
    HANDLE hDevice = this->OpenDeviceHandle();
    if (hDevice == INVALID_HANDLE_VALUE || hDevice == NULL)
        return false;

    DWORD nBytes = 0;
    BOOL nRet = DeviceIoControl(
        hDevice, 
        IOCTL_DISK_GET_DRIVE_GEOMETRY, 
        NULL, 
        0, 
        &geometry, 
        sizeof(geometry), 
        &nBytes, 
        NULL);

    bool bRet = false;
    if (nRet == FALSE)
    {
        bRet = false;
    }
    else
    {
        bRet = true;
    }


    if (hDevice != INVALID_HANDLE_VALUE && hDevice != NULL)
    {
        CloseHandle(hDevice);
        hDevice = NULL;
    }

    return bRet;
}

bool CDiskController::GetVersionInfor(OUT GETVERSIONINPARAMS& versionParams)
{
    HANDLE hDevice = this->OpenDeviceHandle();
    if (hDevice == INVALID_HANDLE_VALUE || hDevice == NULL)
        return false;

    DWORD nBytes = 0;
    BOOL nRet = DeviceIoControl(
        hDevice, 
        SMART_GET_VERSION, 
        NULL, 
        0, 
        &versionParams, 
        sizeof(versionParams), 
        &nBytes, 
        NULL);

    bool bRet = false;
    if (nRet == FALSE)
    {
        bRet = false;
    }
    else
    {
        bRet = true;
    }


    if (hDevice != INVALID_HANDLE_VALUE && hDevice != NULL)
    {
        CloseHandle(hDevice);
        hDevice = NULL;
    }

    return bRet;
}

CIDEDiskController::CIDEDiskController(IN const wstring& devicePath)
    : CDiskController(devicePath)
{

}

CIDEDiskController::~CIDEDiskController()
{

}

bool CIDEDiskController::ATACmdCheckMediaCardType()
{
    BOOL nRet = FALSE; // ϵͳAPI����ֵ
    bool bRet = false; // ��������ֵ
    HANDLE hDevice = NULL; // �豸���
    ULONG nBytes = 0; // �洢���ص��ֽ���

    const int DATA_BUFFER_LEN = 512; // ���ݻ��峤��
    BYTE paramBuffer[sizeof(ATA_PASS_THROUGH_EX) + DATA_BUFFER_LEN] = {0}; // ����������, ATA����+���������
    ATA_PASS_THROUGH_EX* pATACmd = (ATA_PASS_THROUGH_EX*)paramBuffer; // ATA�������

    hDevice = this->OpenDeviceHandle();
    if (hDevice == INVALID_HANDLE_VALUE || hDevice == NULL)
    {
        bRet = false;
        goto SAFE_EXIT;
    }

    
    pATACmd->Length = sizeof(ATA_PASS_THROUGH_EX);
    pATACmd->AtaFlags = ATA_FLAGS_DATA_IN | ATA_FLAGS_USE_DMA; // ��ȡ����
    pATACmd->DataBufferOffset = sizeof(ATA_PASS_THROUGH_EX); // ���ݻ�������ƫ��ֵ
    pATACmd->DataTransferLength = DATA_BUFFER_LEN; // ���ݻ������ĳ���
    pATACmd->TimeOutValue = 3; // ����ִ�еĳ�ʱʱ��(��)
    pATACmd->CurrentTaskFile[6] = ATA_CHECK_MEDIA_CARD_TYPE; // ����Ĵ���
   
    nRet = DeviceIoControl(
        hDevice,
        IOCTL_ATA_PASS_THROUGH,
        pATACmd,
        sizeof(ATA_PASS_THROUGH_EX),
        paramBuffer,
        sizeof(ATA_PASS_THROUGH_EX) + DATA_BUFFER_LEN,
        &nBytes,
        NULL);

    if (nRet == FALSE)
    {
        bRet = false;
        goto SAFE_EXIT;
    }

    // ִ�������TaskFile�еĵ�7���ֽ�ֵΪ״̬�Ĵ�����ֵ, ����ֵ����֪�������ִ�н��
    // ���״̬�Ĵ�����ֵ��0λΪ1, ��ʾ�����˴���
    if (pATACmd->CurrentTaskFile[6] & 0x1)
    {
        bRet = false;
        goto SAFE_EXIT;
    }
   
    bRet = true;

SAFE_EXIT:
    if (hDevice != INVALID_HANDLE_VALUE && hDevice != NULL)
    {
        CloseHandle(hDevice);
        hDevice = NULL;
    }

    return bRet;
}

bool CIDEDiskController::ATACmdCheckPowerMode(OUT BYTE& mode)
{
    BOOL nRet = FALSE; // ϵͳAPI����ֵ
    bool bRet = false; // ��������ֵ
    HANDLE hDevice = NULL; // �豸���
    ULONG nBytes = 0; // �洢���ص��ֽ���

    const int DATA_BUFFER_LEN = 512; // ���ݻ��峤��
    BYTE paramBuffer[sizeof(ATA_PASS_THROUGH_EX) + DATA_BUFFER_LEN] = {0}; // ����������, ATA����+���������
    ATA_PASS_THROUGH_EX* pATACmd = (ATA_PASS_THROUGH_EX*)paramBuffer; // ATA�������

    hDevice = this->OpenDeviceHandle();
    if (hDevice == INVALID_HANDLE_VALUE || hDevice == NULL)
    {
        bRet = false;
        goto SAFE_EXIT;
    }


    pATACmd->Length = sizeof(ATA_PASS_THROUGH_EX);
    pATACmd->AtaFlags = ATA_FLAGS_DATA_IN | ATA_FLAGS_USE_DMA; // ��ȡ����
    pATACmd->DataBufferOffset = sizeof(ATA_PASS_THROUGH_EX); // ���ݻ�������ƫ��ֵ
    pATACmd->DataTransferLength = DATA_BUFFER_LEN; // ���ݻ������ĳ���
    pATACmd->TimeOutValue = 3; // ����ִ�еĳ�ʱʱ��(��)
    pATACmd->CurrentTaskFile[6] = ATA_CHECK_POWER_MODE; // ����Ĵ���

    nRet = DeviceIoControl(
        hDevice,
        IOCTL_ATA_PASS_THROUGH,
        pATACmd,
        sizeof(ATA_PASS_THROUGH_EX),
        paramBuffer,
        sizeof(ATA_PASS_THROUGH_EX) + DATA_BUFFER_LEN,
        &nBytes,
        NULL);

    if (nRet == FALSE)
    {
        bRet = false;
        goto SAFE_EXIT;
    }

    // ִ�������TaskFile�еĵ�7���ֽ�ֵΪ״̬�Ĵ�����ֵ, ����ֵ����֪�������ִ�н��
    // ���״̬�Ĵ�����ֵ��0λΪ1, ��ʾ�����˴���
    if (pATACmd->CurrentTaskFile[6] & 0x1)
    {
        bRet = false;
        goto SAFE_EXIT;
    }

    // ����ATA8 SPEC ��֪, ִ�������TaskFile�еĵ�2���ֽ�ֵΪ��Դģʽ��ֵ
    mode = pATACmd->CurrentTaskFile[1];

    bRet = true;

SAFE_EXIT:
    if (hDevice != INVALID_HANDLE_VALUE && hDevice != NULL)
    {
        CloseHandle(hDevice);
        hDevice = NULL;
    }

    return bRet;
}

bool CIDEDiskController::ATACmdIDLEImmediate()
{
    BOOL nRet = FALSE; // ϵͳAPI����ֵ
    bool bRet = false; // ��������ֵ
    HANDLE hDevice = NULL; // �豸���
    ULONG nBytes = 0; // �洢���ص��ֽ���

    const int DATA_BUFFER_LEN = 512; // ���ݻ��峤��
    BYTE paramBuffer[sizeof(ATA_PASS_THROUGH_EX) + DATA_BUFFER_LEN] = {0}; // ����������, ATA����+���������
    ATA_PASS_THROUGH_EX* pATACmd = (ATA_PASS_THROUGH_EX*)paramBuffer; // ATA�������

    hDevice = this->OpenDeviceHandle();
    if (hDevice == INVALID_HANDLE_VALUE || hDevice == NULL)
    {
        bRet = false;
        goto SAFE_EXIT;
    }


    pATACmd->Length = sizeof(ATA_PASS_THROUGH_EX);
    pATACmd->AtaFlags = ATA_FLAGS_DATA_IN | ATA_FLAGS_USE_DMA; // ��ȡ����
    pATACmd->DataBufferOffset = sizeof(ATA_PASS_THROUGH_EX); // ���ݻ�������ƫ��ֵ
    pATACmd->DataTransferLength = DATA_BUFFER_LEN; // ���ݻ������ĳ���
    pATACmd->TimeOutValue = 3; // ����ִ�еĳ�ʱʱ��(��)
    pATACmd->CurrentTaskFile[6] = ATA_IDELE_IMMEDIATE; // ����Ĵ���

    nRet = DeviceIoControl(
        hDevice,
        IOCTL_ATA_PASS_THROUGH,
        pATACmd,
        sizeof(ATA_PASS_THROUGH_EX),
        paramBuffer,
        sizeof(ATA_PASS_THROUGH_EX) + DATA_BUFFER_LEN,
        &nBytes,
        NULL);

    if (nRet == FALSE)
    {
        bRet = false;
        goto SAFE_EXIT;
    }

    // ִ�������TaskFile�еĵ�7���ֽ�ֵΪ״̬�Ĵ�����ֵ, ����ֵ����֪�������ִ�н��
    // ���״̬�Ĵ�����ֵ��0λΪ1, ��ʾ�����˴���
    if (pATACmd->CurrentTaskFile[6] & 0x1)
    {
        bRet = false;
        goto SAFE_EXIT;
    }

    bRet = true;

SAFE_EXIT:
    if (hDevice != INVALID_HANDLE_VALUE && hDevice != NULL)
    {
        CloseHandle(hDevice);
        hDevice = NULL;
    }

    return bRet;
}

bool CIDEDiskController::ATACmdStandbyImmediate()
{
    BOOL nRet = FALSE; // ϵͳAPI����ֵ
    bool bRet = false; // ��������ֵ
    HANDLE hDevice = NULL; // �豸���
    ULONG nBytes = 0; // �洢���ص��ֽ���

    const int DATA_BUFFER_LEN = 512; // ���ݻ��峤��
    BYTE paramBuffer[sizeof(ATA_PASS_THROUGH_EX) + DATA_BUFFER_LEN] = {0}; // ����������, ATA����+���������
    ATA_PASS_THROUGH_EX* pATACmd = (ATA_PASS_THROUGH_EX*)paramBuffer; // ATA�������

    hDevice = this->OpenDeviceHandle();
    if (hDevice == INVALID_HANDLE_VALUE || hDevice == NULL)
    {
        bRet = false;
        goto SAFE_EXIT;
    }


    pATACmd->Length = sizeof(ATA_PASS_THROUGH_EX);
    pATACmd->AtaFlags = ATA_FLAGS_DATA_IN | ATA_FLAGS_USE_DMA; // ��ȡ����
    pATACmd->DataBufferOffset = sizeof(ATA_PASS_THROUGH_EX); // ���ݻ�������ƫ��ֵ
    pATACmd->DataTransferLength = DATA_BUFFER_LEN; // ���ݻ������ĳ���
    pATACmd->TimeOutValue = 3; // ����ִ�еĳ�ʱʱ��(��)
    pATACmd->CurrentTaskFile[6] = ATA_STANDBY_IMMEDIATE; // ����Ĵ���

    nRet = DeviceIoControl(
        hDevice,
        IOCTL_ATA_PASS_THROUGH,
        pATACmd,
        sizeof(ATA_PASS_THROUGH_EX),
        paramBuffer,
        sizeof(ATA_PASS_THROUGH_EX) + DATA_BUFFER_LEN,
        &nBytes,
        NULL);

    if (nRet == FALSE)
    {
        bRet = false;
        goto SAFE_EXIT;
    }

    // ִ�������TaskFile�еĵ�7���ֽ�ֵΪ״̬�Ĵ�����ֵ, ����ֵ����֪�������ִ�н��
    // ���״̬�Ĵ�����ֵ��0λΪ1, ��ʾ�����˴���
    if (pATACmd->CurrentTaskFile[6] & 0x1)
    {
        bRet = false;
        goto SAFE_EXIT;
    }

    bRet = true;

SAFE_EXIT:
    if (hDevice != INVALID_HANDLE_VALUE && hDevice != NULL)
    {
        CloseHandle(hDevice);
        hDevice = NULL;
    }

    return bRet;
}

bool CIDEDiskController::ATACmdSleep()
{
    BOOL nRet = FALSE; // ϵͳAPI����ֵ
    bool bRet = false; // ��������ֵ
    HANDLE hDevice = NULL; // �豸���
    ULONG nBytes = 0; // �洢���ص��ֽ���

    const int DATA_BUFFER_LEN = 512; // ���ݻ��峤��
    BYTE paramBuffer[sizeof(ATA_PASS_THROUGH_EX) + DATA_BUFFER_LEN] = {0}; // ����������, ATA����+���������
    ATA_PASS_THROUGH_EX* pATACmd = (ATA_PASS_THROUGH_EX*)paramBuffer; // ATA�������

    hDevice = this->OpenDeviceHandle();
    if (hDevice == INVALID_HANDLE_VALUE || hDevice == NULL)
    {
        bRet = false;
        goto SAFE_EXIT;
    }


    pATACmd->Length = sizeof(ATA_PASS_THROUGH_EX);
    pATACmd->AtaFlags = ATA_FLAGS_DATA_IN | ATA_FLAGS_USE_DMA; // ��ȡ����
    pATACmd->DataBufferOffset = sizeof(ATA_PASS_THROUGH_EX); // ���ݻ�������ƫ��ֵ
    pATACmd->DataTransferLength = DATA_BUFFER_LEN; // ���ݻ������ĳ���
    pATACmd->TimeOutValue = 3; // ����ִ�еĳ�ʱʱ��(��)
    pATACmd->CurrentTaskFile[6] = ATA_SLEEP; // ����Ĵ���

    nRet = DeviceIoControl(
        hDevice,
        IOCTL_ATA_PASS_THROUGH,
        pATACmd,
        sizeof(ATA_PASS_THROUGH_EX),
        paramBuffer,
        sizeof(ATA_PASS_THROUGH_EX) + DATA_BUFFER_LEN,
        &nBytes,
        NULL);

    if (nRet == FALSE)
    {
        bRet = false;
        goto SAFE_EXIT;
    }

    // ִ�������TaskFile�еĵ�7���ֽ�ֵΪ״̬�Ĵ�����ֵ, ����ֵ����֪�������ִ�н��
    // ���״̬�Ĵ�����ֵ��0λΪ1, ��ʾ�����˴���
    if (pATACmd->CurrentTaskFile[6] & 0x1)
    {
        bRet = false;
        goto SAFE_EXIT;
    }

    bRet = true;

SAFE_EXIT:
    if (hDevice != INVALID_HANDLE_VALUE && hDevice != NULL)
    {
        CloseHandle(hDevice);
        hDevice = NULL;
    }

    return bRet;
}

bool CIDEDiskController::ATACmdDeviceReset()
{
    BOOL nRet = FALSE; // ϵͳAPI����ֵ
    bool bRet = false; // ��������ֵ
    HANDLE hDevice = NULL; // �豸���
    ULONG nBytes = 0; // �洢���ص��ֽ���

    const int DATA_BUFFER_LEN = 512; // ���ݻ��峤��
    BYTE paramBuffer[sizeof(ATA_PASS_THROUGH_EX) + DATA_BUFFER_LEN] = {0}; // ����������, ATA����+���������
    ATA_PASS_THROUGH_EX* pATACmd = (ATA_PASS_THROUGH_EX*)paramBuffer; // ATA�������

    hDevice = this->OpenDeviceHandle();
    if (hDevice == INVALID_HANDLE_VALUE || hDevice == NULL)
    {
        bRet = false;
        goto SAFE_EXIT;
    }


    pATACmd->Length = sizeof(ATA_PASS_THROUGH_EX);
    pATACmd->AtaFlags = ATA_FLAGS_DATA_IN | ATA_FLAGS_USE_DMA; // ��ȡ����
    pATACmd->DataBufferOffset = sizeof(ATA_PASS_THROUGH_EX); // ���ݻ�������ƫ��ֵ
    pATACmd->DataTransferLength = DATA_BUFFER_LEN; // ���ݻ������ĳ���
    pATACmd->TimeOutValue = 3; // ����ִ�еĳ�ʱʱ��(��)
    pATACmd->CurrentTaskFile[6] = ATA_DEVICE_RESET; // ����Ĵ���

    nRet = DeviceIoControl(
        hDevice,
        IOCTL_ATA_PASS_THROUGH,
        pATACmd,
        sizeof(ATA_PASS_THROUGH_EX),
        paramBuffer,
        sizeof(ATA_PASS_THROUGH_EX) + DATA_BUFFER_LEN,
        &nBytes,
        NULL);

    if (nRet == FALSE)
    {
        bRet = false;
        goto SAFE_EXIT;
    }

    // ִ�������TaskFile�еĵ�7���ֽ�ֵΪ״̬�Ĵ�����ֵ, ����ֵ����֪�������ִ�н��
    // ���״̬�Ĵ�����ֵ��0λΪ1, ��ʾ�����˴���
    if (pATACmd->CurrentTaskFile[6] & 0x1)
    {
        bRet = false;
        goto SAFE_EXIT;
    }

    bRet = true;

SAFE_EXIT:
    if (hDevice != INVALID_HANDLE_VALUE && hDevice != NULL)
    {
        CloseHandle(hDevice);
        hDevice = NULL;
    }

    return bRet;
}

bool CIDEDiskController::ATACmdExecuteDeviceDiagnostic(OUT BYTE& state)
{
    BOOL nRet = FALSE; // ϵͳAPI����ֵ
    bool bRet = false; // ��������ֵ
    HANDLE hDevice = NULL; // �豸���
    ULONG nBytes = 0; // �洢���ص��ֽ���

    const int DATA_BUFFER_LEN = 512; // ���ݻ��峤��
    BYTE paramBuffer[sizeof(ATA_PASS_THROUGH_EX) + DATA_BUFFER_LEN] = {0}; // ����������, ATA����+���������
    ATA_PASS_THROUGH_EX* pATACmd = (ATA_PASS_THROUGH_EX*)paramBuffer; // ATA�������

    hDevice = this->OpenDeviceHandle();
    if (hDevice == INVALID_HANDLE_VALUE || hDevice == NULL)
    {
        bRet = false;
        goto SAFE_EXIT;
    }


    pATACmd->Length = sizeof(ATA_PASS_THROUGH_EX);
    pATACmd->AtaFlags = ATA_FLAGS_DATA_IN | ATA_FLAGS_USE_DMA; // ��ȡ����
    pATACmd->DataBufferOffset = sizeof(ATA_PASS_THROUGH_EX); // ���ݻ�������ƫ��ֵ
    pATACmd->DataTransferLength = DATA_BUFFER_LEN; // ���ݻ������ĳ���
    pATACmd->TimeOutValue = 3; // ����ִ�еĳ�ʱʱ��(��)
    pATACmd->CurrentTaskFile[6] = ATA_EXECUTE_DEVICE_DIAGNOSTIC; // ����Ĵ���

    nRet = DeviceIoControl(
        hDevice,
        IOCTL_ATA_PASS_THROUGH,
        pATACmd,
        sizeof(ATA_PASS_THROUGH_EX),
        paramBuffer,
        sizeof(ATA_PASS_THROUGH_EX) + DATA_BUFFER_LEN,
        &nBytes,
        NULL);

    if (nRet == FALSE)
    {
        bRet = false;
        goto SAFE_EXIT;
    }

    // ִ�������TaskFile�еĵ�7���ֽ�ֵΪ״̬�Ĵ�����ֵ, ����ֵ����֪�������ִ�н��
    // ���״̬�Ĵ�����ֵ��0λΪ1, ��ʾ�����˴���
    if (pATACmd->CurrentTaskFile[6] & 0x1)
    {
        bRet = false;
        goto SAFE_EXIT;
    }

    // ��ȡ����Ĵ����е�ֵ
    // ִ�������TaskFile�еĵ�1���ֽ�ֵΪ����Ĵ�����ֵ
    state = pATACmd->CurrentTaskFile[0];
    bRet = true;

SAFE_EXIT:
    if (hDevice != INVALID_HANDLE_VALUE && hDevice != NULL)
    {
        CloseHandle(hDevice);
        hDevice = NULL;
    }

    return bRet;
}





bool CIDEDiskController::ATACmdReadVerifySectorExt(IN ULONGLONG lbaAddress, IN unsigned long sectorCount)
{
    BOOL nRet = FALSE; // ϵͳAPI����ֵ
    bool bRet = false; // ��������ֵ
    HANDLE hDevice = NULL; // �豸���
    ULONG nBytes = 0; // �洢���ص��ֽ���

    const int DATA_BUFFER_LEN = 512; // ���ݻ��峤��
    BYTE paramBuffer[sizeof(ATA_PASS_THROUGH_EX) + DATA_BUFFER_LEN] = {0}; // ����������, ATA����+���������
    ATA_PASS_THROUGH_EX* pATACmd = (ATA_PASS_THROUGH_EX*)paramBuffer; // ATA�������
    IDEREGS* pCurrentTaskFile = (IDEREGS*)pATACmd->CurrentTaskFile;
    IDEREGS* pPreviousTaskFile = (IDEREGS*)pATACmd->PreviousTaskFile;

    hDevice = this->OpenDeviceHandle();
    if (hDevice == INVALID_HANDLE_VALUE || hDevice == NULL)
    {
        bRet = false;
        goto SAFE_EXIT;
    }


    pATACmd->Length = sizeof(ATA_PASS_THROUGH_EX);
    pATACmd->AtaFlags = ATA_FLAGS_DATA_IN | ATA_FLAGS_USE_DMA | ATA_FLAGS_48BIT_COMMAND; // ��ȡ����
    pATACmd->DataBufferOffset = sizeof(ATA_PASS_THROUGH_EX); // ���ݻ�������ƫ��ֵ
    pATACmd->DataTransferLength = DATA_BUFFER_LEN; // ���ݻ������ĳ���
    pATACmd->TimeOutValue = 3; // ����ִ�еĳ�ʱʱ��(��)

    // ��������Ĵ���
    pCurrentTaskFile->bCommandReg = pPreviousTaskFile->bCommandReg = ATA_READ_VERIFY_SECTOR_EXT;

     /*	
    |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
    +-----+-----+-----+-----+-----+-----+-----+-----+
    |  1  |  L  |  1  | DRV | HS3 | HS2 | HS1 | HS0 |
    +-----+-----+-----+-----+-----+-----+-----+-----+
    |           |   \_____________________/
    |           |              |
    |           |              `------------ If L=0, Head Select.
    |           |                                   These four bits select the head number.
    |           |                                   HS0 is the least significant.
    |           |                            If L=1, HS0 through HS3 contain bit 24-27 of the LBA.
    |           `--------------------------- Drive. When DRV=0, drive 0 (master) is selected. 
    |                                               When DRV=1, drive 1 (slave) is selected.
    `--------------------------------------- LBA mode. This bit selects the mode of operation.
    When L=0, addressing is by 'CHS' mode.
    When L=1, addressing is by 'LBA' mode.
    */

    // ����������ͷ�Ĵ���
    pCurrentTaskFile->bDriveHeadReg = pPreviousTaskFile->bDriveHeadReg = 0xE0; // ������ͷ�Ĵ�������Ϊ0xE0��ʾʹ��LBAѰַ��ʽ
   

    // ���ö�ȡ������Ŀ�Ĵ���
    pCurrentTaskFile->bSectorCountReg = (BYTE)sectorCount;
    pPreviousTaskFile->bSectorCountReg = (BYTE)(sectorCount >> 8);

    // ������ʼ�Ĵ���LBA��ַ
    pCurrentTaskFile->bSectorNumberReg = (BYTE)lbaAddress;
    pCurrentTaskFile->bCylLowReg = (BYTE)(lbaAddress >> 8);
    pCurrentTaskFile->bCylHighReg = (BYTE)(lbaAddress >> 16);
    pPreviousTaskFile->bSectorNumberReg = (BYTE)(lbaAddress >> 24);
    pPreviousTaskFile->bCylLowReg = (BYTE)(lbaAddress >> 32);
    pPreviousTaskFile->bCylHighReg = (BYTE)(lbaAddress >> 40);

    nRet = DeviceIoControl(
        hDevice,
        IOCTL_ATA_PASS_THROUGH,
        pATACmd,
        sizeof(ATA_PASS_THROUGH_EX),
        paramBuffer,
        sizeof(ATA_PASS_THROUGH_EX) + DATA_BUFFER_LEN,
        &nBytes,
        NULL);

    if (nRet == FALSE)
    {
        bRet = false;
        goto SAFE_EXIT;
    }

    // ִ�������TaskFile�еĵ�7���ֽ�ֵΪ״̬�Ĵ�����ֵ, ����ֵ����֪�������ִ�н��
    // ���״̬�Ĵ�����ֵ��0λΪ1, ��ʾ�����˴���
    if (pATACmd->CurrentTaskFile[6] & 0x1)
    {
        bRet = false;
        goto SAFE_EXIT;
    }

    bRet = true;

SAFE_EXIT:
    if (hDevice != INVALID_HANDLE_VALUE && hDevice != NULL)
    {
        CloseHandle(hDevice);
        hDevice = NULL;
    }

    return bRet;
}

bool CIDEDiskController::ATACmdIdentifyDevice(OUT SATA8IdentifyData& identifyData)
{
    BOOL nRet = FALSE; // ϵͳAPI����ֵ
    bool bRet = false; // ��������ֵ
    HANDLE hDevice = NULL; // �豸���
    ULONG nBytes = 0; // �洢���ص��ֽ���

    const int DATA_BUFFER_LEN = 512; // ���ݻ��峤��
    BYTE paramBuffer[sizeof(ATA_PASS_THROUGH_EX) + DATA_BUFFER_LEN] = {0}; // ����������, ATA����+���������
    ATA_PASS_THROUGH_EX* pATACmd = (ATA_PASS_THROUGH_EX*)paramBuffer; // ATA�������

    hDevice = this->OpenDeviceHandle();
    if (hDevice == INVALID_HANDLE_VALUE || hDevice == NULL)
    {
        bRet = false;
        goto SAFE_EXIT;
    }


    pATACmd->Length = sizeof(ATA_PASS_THROUGH_EX);
    pATACmd->AtaFlags = ATA_FLAGS_DATA_IN | ATA_FLAGS_USE_DMA ; // ��ȡ����
    pATACmd->DataBufferOffset = sizeof(ATA_PASS_THROUGH_EX); // ���ݻ�������ƫ��ֵ
    pATACmd->DataTransferLength = DATA_BUFFER_LEN; // ���ݻ������ĳ���
    pATACmd->TimeOutValue = 3; // ����ִ�еĳ�ʱʱ��(��)
    pATACmd->CurrentTaskFile[6] = ATA_IDENTIFY_DEVICE; // ����Ĵ���

    nRet = DeviceIoControl(
        hDevice,
        IOCTL_ATA_PASS_THROUGH,
        pATACmd,
        sizeof(ATA_PASS_THROUGH_EX),
        paramBuffer,
        sizeof(ATA_PASS_THROUGH_EX) + DATA_BUFFER_LEN,
        &nBytes,
        NULL);

    if (nRet == FALSE)
    {
        bRet = false;
        goto SAFE_EXIT;
    }

    // ִ�������TaskFile�еĵ�7���ֽ�ֵΪ״̬�Ĵ�����ֵ, ����ֵ����֪�������ִ�н��
    // ���״̬�Ĵ�����ֵ��0λΪ1, ��ʾ�����˴���
    if (pATACmd->CurrentTaskFile[6] & 0x1)
    {
        bRet = false;
        goto SAFE_EXIT;
    }

    memcpy(&identifyData, paramBuffer + sizeof(ATA_PASS_THROUGH_EX), DATA_BUFFER_LEN);

    bRet = true;

SAFE_EXIT:
    if (hDevice != INVALID_HANDLE_VALUE && hDevice != NULL)
    {
        CloseHandle(hDevice);
        hDevice = NULL;
    }

    return bRet;
}

bool CIDEDiskController::ATACmdSMARTReadData(OUT SSMARTData& smartData)
{
     BOOL nRet = FALSE; // ϵͳAPI����ֵ
    bool bRet = false; // ��������ֵ
    HANDLE hDevice = NULL; // �豸���
    ULONG nBytes = 0; // �洢���ص��ֽ���

    const int DATA_BUFFER_LEN = 512; // ���ݻ��峤��
    BYTE paramBuffer[sizeof(ATA_PASS_THROUGH_EX) + DATA_BUFFER_LEN] = {0}; // ����������, ATA����+���������
    ATA_PASS_THROUGH_EX* pATACmd = (ATA_PASS_THROUGH_EX*)paramBuffer; // ATA�������
    IDEREGS* pCurrentTaskFile = (IDEREGS*)pATACmd->CurrentTaskFile;

    hDevice = this->OpenDeviceHandle();
    if (hDevice == INVALID_HANDLE_VALUE || hDevice == NULL)
    {
        bRet = false;
        goto SAFE_EXIT;
    }


    pATACmd->Length = sizeof(ATA_PASS_THROUGH_EX);
    pATACmd->AtaFlags = ATA_FLAGS_DATA_IN | ATA_FLAGS_USE_DMA; // ��ȡ����
    pATACmd->DataBufferOffset = sizeof(ATA_PASS_THROUGH_EX); // ���ݻ�������ƫ��ֵ
    pATACmd->DataTransferLength = DATA_BUFFER_LEN; // ���ݻ������ĳ���
    pATACmd->TimeOutValue = 3; // ����ִ�еĳ�ʱʱ��(��)

    // ��������Ĵ���
    pCurrentTaskFile->bCommandReg = SMART_CMD;

     /*	
    |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
    +-----+-----+-----+-----+-----+-----+-----+-----+
    |  1  |  L  |  1  | DRV | HS3 | HS2 | HS1 | HS0 |
    +-----+-----+-----+-----+-----+-----+-----+-----+
    |           |   \_____________________/
    |           |              |
    |           |              `------------ If L=0, Head Select.
    |           |                                   These four bits select the head number.
    |           |                                   HS0 is the least significant.
    |           |                            If L=1, HS0 through HS3 contain bit 24-27 of the LBA.
    |           `--------------------------- Drive. When DRV=0, drive 0 (master) is selected. 
    |                                               When DRV=1, drive 1 (slave) is selected.
    `--------------------------------------- LBA mode. This bit selects the mode of operation.
    When L=0, addressing is by 'CHS' mode.
    When L=1, addressing is by 'LBA' mode.
    */

    // ����������ͷ�Ĵ���
    pCurrentTaskFile->bDriveHeadReg = 0xA0; // ������ͷ�Ĵ�������Ϊ0xA0��ʾʹ��CHSѰַ��ʽ
   

    // ���������Ĵ���
    pCurrentTaskFile->bFeaturesReg = READ_ATTRIBUTES;

    pCurrentTaskFile->bCylLowReg = SMART_CYL_LOW;
    pCurrentTaskFile->bCylHighReg = SMART_CYL_HI;

    nRet = DeviceIoControl(
        hDevice,
        IOCTL_ATA_PASS_THROUGH,
        pATACmd,
        sizeof(ATA_PASS_THROUGH_EX),
        paramBuffer,
        sizeof(ATA_PASS_THROUGH_EX) + DATA_BUFFER_LEN,
        &nBytes,
        NULL);

    if (nRet == FALSE)
    {
        bRet = false;
        goto SAFE_EXIT;
    }

    // ִ�������TaskFile�еĵ�7���ֽ�ֵΪ״̬�Ĵ�����ֵ, ����ֵ����֪�������ִ�н��
    // ���״̬�Ĵ�����ֵ��0λΪ1, ��ʾ�����˴���
    if (pATACmd->CurrentTaskFile[6] & 0x1)
    {
        bRet = false;
        goto SAFE_EXIT;
    }

    // ���ص�������, ǰ362������ΪSMART��������
    memcpy(&smartData, paramBuffer + sizeof(ATA_PASS_THROUGH_EX), sizeof(SSMARTData));

    bRet = true;

SAFE_EXIT:
    if (hDevice != INVALID_HANDLE_VALUE && hDevice != NULL)
    {
        CloseHandle(hDevice);
        hDevice = NULL;
    }

    return bRet;
}

LIDEDiskController::LIDEDiskController(IN const wstring& devicePath)
{
    m_pIDEDiskController = new CIDEDiskController(devicePath);
}

LIDEDiskController::~LIDEDiskController()
{
    delete m_pIDEDiskController;
    m_pIDEDiskController = 0;
}

bool LIDEDiskController::DeviceExist()
{
    return m_pIDEDiskController->DeviceExist();
}

unsigned long LIDEDiskController::GetRotationRate()
{
    SATA8IdentifyData identifyData;
    bool bRet = this->m_pIDEDiskController->ATACmdIdentifyDevice(identifyData);
    if (!bRet)
    {
        return 0;
    }

    return identifyData.NominalMediaRotationRate;
}

unsigned long LIDEDiskController::GetSATAType()
{
    SATA8IdentifyData identifyData;
    bool bRet = this->m_pIDEDiskController->ATACmdIdentifyDevice(identifyData);
    if (!bRet)
    {
        return 0;
    }

    /*
    �߰汾�İ����Ͱ汾��ֵ������Ҫ�Ӹߵ����ж�
    */
    if (identifyData.SATACapabilities.SATAGen3)
    {
        return 3;
    }

    if (identifyData.SATACapabilities.SATAGen2)
    {
        return 2;
    }

    if (identifyData.SATACapabilities.SATAGen1)
    {
        return 1;
    }

    return 0;
}