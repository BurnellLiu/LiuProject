
#ifndef _LWMI_SYSTEM_CLASSES_H_
#define _LWMI_SYSTEM_CLASSES_H_


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

    /// @brief �����ϵͳ��������(�ǲ���ϵͳ������Ӧ�ð�����Ӳ��)
    class LComputerSystemManager
    {
    public:
        LComputerSystemManager();
        ~LComputerSystemManager();

        /// @brief ��ȡ���������
        enum LCOMPUTER_SYSTEM_TYPE
        {
            COMPUTER_SYSTEM_UNKNOWN = 0, ///< δ֪
            COMPUTER_SYSTEM_DESKTOP = 1, ///< ̨ʽ��
            COMPUTER_SYSTEM_NOTE_BOOK = 2, ///< �ʼǱ�
            COMPUTER_SYSTEM_TABLET = 3 ///< ƽ�����
        };

        /// @brief ��ȡ�����ϵͳ����
        /// @return �����ϵͳ����(Ӧ��Ϊ1)
        int GetComputerSystemCount();

        /// @brief ��ȡ�����ϵͳ�ͺ�
        /// @param[in] index ����
        /// @param[out] model �洢�ͺ�
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetComputerSystemModel(IN int index, OUT wstring& model);

        /// @brief ��ȡ�����ϵͳ������
        /// @param[in] index ����
        /// @param[out] manufacturer �洢������
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetComputerSystemManufacturer(IN int index, OUT wstring& manufacturer);

        /// @brief ��ȡ���������
        /// @param[in] index ����
        /// @param[out] type �洢����
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetComputerSystemPCType(IN int index, OUT LCOMPUTER_SYSTEM_TYPE& type);

        /// @brief ��ȡ�����ϵͳ����
        ///
        /// ��: "x64-based PC"
        /// @param[in] index ����
        /// @param[out] type �洢����
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetComputerSystemType(IN int index, OUT wstring& type);

    private:
        LWMICoreManager* m_pWMICoreManager; ///< WMI���Ĺ�����
    private:
        LComputerSystemManager(const LComputerSystemManager&);
        LComputerSystemManager& operator = (const LComputerSystemManager&);
    };

    /// @brief ����ϵͳ��������
    class LOperatingSystemManager
    {
    public:
        LOperatingSystemManager();
        ~LOperatingSystemManager();

        /// @brief ��ȡ����ϵͳ����
        /// @return ����ϵͳ����(Ӧ��Ϊ1)
        int GetOSCount();

        /// @brief ��ȡ����ϵͳ����
        /// @param[in] index ����
        /// @param[out] caption ����
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetOSCaption(IN int index, OUT wstring& caption);

        /// @brief ��ȡ����ϵͳ�ܹ�
        /// @param[in] index ����
        /// @param[out] architecture ����ϵͳ�ܹ�
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetOSArchitecture(IN int index, OUT wstring& architecture);

        /// @brief ��ȡ����ϵͳ�汾
        /// @param[in] index ����
        /// @param[out] version �汾
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetOSVersion(IN int index, OUT wstring& version);

        /// @brief ��ȡ����ϵͳϵͳ�̷�
        /// @param[in] index ����
        /// @param[out] drive ϵͳ�̷�
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetOSSystemDrive(IN int index, OUT wstring& drive);

    private:
        LWMICoreManager* m_pWMICoreManager; ///< WMI���Ĺ�����

    private:
        LOperatingSystemManager(const LOperatingSystemManager&);
        LOperatingSystemManager& operator = (const LOperatingSystemManager&);
    };

    /// @brief ϵͳ��Ϣ��������
    class LMS_SystemInformationManager
    {
    public:
        LMS_SystemInformationManager();
        ~LMS_SystemInformationManager();

        /// @brief ��ȡϵͳ��Ϣ����
        /// @return ϵͳ��Ϣ����(Ӧ��Ϊ1)
        int GetSystemInforCount();

        /// @brief ��ȡ����������
        /// @param[in] index
        /// @param[out] manufacturer �洢��������Ϣ
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetBaseBoardManufacturer(IN int index, OUT wstring& manufacturer);

        /// @brief ��ȡ�����Ʒ����
        /// @param[in] index
        /// @param[out] product �洢��Ʒ����
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetBaseBoardProductName(IN int index, OUT wstring& product);

        /// @brief ��ȡBIOS��������
        /// @param[in] index
        /// @param[out] releaseDate �洢��������
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetBIOSReleaseDate(IN int index, OUT wstring& releaseDate);

        /// @brief ��ȡBIOS������Ϣ
        /// @param[in] index
        /// @param[out] vendor �洢������Ϣ 
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetBIOSVendor(IN int index, OUT wstring& vendor);

        /// @brief ��ȡBIOS�汾
        /// @param[in] index 
        /// @param[out] version �洢BIOS�汾
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetBIOSVersion(IN int index, OUT wstring& version);

        /// @brief ��ȡϵͳ����
        /// @param[in] index
        /// @param[out] family �洢ϵͳ����
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetSystemFamily(IN int index, OUT wstring& family);

        /// @brief ��ȡϵͳ��������Ϣ
        /// @param[in] index
        /// @param[in] manufacturer �洢��������Ϣ
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetSystemManufacturer(IN int index, OUT wstring& manufacturer);

        /// @brief ��ȡϵͳ��Ʒ����
        /// @param[in] index
        /// @param[out] productName �洢��Ʒ����
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetSystemProductName(IN int index, OUT wstring& productName);

        /// @brief ��ȡϵͳSKU��Ϣ
        ///
        /// ��: ASUS-NotebookSKU
        /// @param[in] index
        /// @param[in] sku �洢SKU��Ϣ
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetSystemSKU(IN int index, OUT wstring& sku);

    private:
        LWMICoreManager* m_pWMICoreManager; ///< WMI���Ĺ�����

    private:
        LMS_SystemInformationManager(const LMS_SystemInformationManager&);
        LMS_SystemInformationManager& operator = (const LMS_SystemInformationManager&);
    };

    /// @brief ϵͳ��Ϣ��������
    class LPerfRawData_PerfOS_MemoryManager
    {
    public:
        LPerfRawData_PerfOS_MemoryManager();
        ~LPerfRawData_PerfOS_MemoryManager();

        /// @brief ��ȡ�ڴ�������������
        /// @return �ڴ�������������(Ӧ��Ϊ1)
        int GetMemoryPerfDataCount();

        /// @brief ��ȡ�ڴ���ô�С
        /// @param[in] index ����
        /// @param[out] availableBytes �����ڴ��С, ��λM
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetMemoryAvailableMBytes(IN int index, OUT unsigned long& availableBytes);

        /// @brief ��ȡ�ڴ�δʹ�ô�С
        /// @param[in] index ����
        /// @param[out] unusedBytes �����ڴ��С, ��λM
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetMemoryUnusedMBytes(IN int index, OUT unsigned long& unusedBytes);

    private:
        LWMICoreManager* m_pWMICoreManager; ///< WMI���Ĺ�����

    private:
        LPerfRawData_PerfOS_MemoryManager(const LPerfRawData_PerfOS_MemoryManager&);
        LPerfRawData_PerfOS_MemoryManager& operator = (const LPerfRawData_PerfOS_MemoryManager&);
    };
}

#endif