
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
        enum LPC_SYSTEM_TYPE
        {
            UNKNOWN = 0, ///< δ֪
            DESKTOP = 1, ///< ̨ʽ��
            NOTE_BOOK = 2, ///< �ʼǱ�
            TABLET = 3 ///< ƽ�����
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
        bool GetComputerSystemPCType(IN int index, OUT LPC_SYSTEM_TYPE& type);

        /// @brief ��ȡ�����ϵͳ����
        /// @param[in] index ����
        /// @param[out] type �洢����
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetComputerSystemType(IN int index, OUT wstring& type);

    private:
        LWMICoreManager* m_pWMICoreManager; ///< WMI���Ĺ�����
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
    };
}

#endif