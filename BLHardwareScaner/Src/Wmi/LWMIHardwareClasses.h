

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

    private:
        LWMICoreManager* m_pWMICoreManager; ///< WMI���Ĺ�����
    };

    /// @brief ����������(�Կ�)��������
    class LVideoControllerManager
    {
    public:
        LVideoControllerManager();
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
    };

}

#endif