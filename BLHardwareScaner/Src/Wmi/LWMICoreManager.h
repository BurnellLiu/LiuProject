

#ifndef _LWMICOREMANAGER_H_
#define _LWMICOREMANAGER_H_



#include <string>
using std::wstring;
#include <vector>
using std::vector;

#ifndef NAMESPACE_ROOT_WMI
#define NAMESPACE_ROOT_WMI L"ROOT\\WMI"
#endif

#ifndef NAMESPACE_ROOT_CIMV2
#define NAMESPACE_ROOT_CIMV2 L"ROOT\\cimv2"
#endif


typedef unsigned char LBYTE;
typedef unsigned short LUINT16;
typedef unsigned int LUINT;
typedef unsigned __int64 LUINT64;

struct IWbemLocator;
struct IWbemServices;
struct IEnumWbemClassObject;
struct IWbemClassObject;
struct IWbemRefresher;
struct IWbemConfigureRefresher;

namespace LWMI
{
    class LInitCom;

    /// @brief WMI���Ķ���
    class LWMICoreManager
    {
    public:
        LWMICoreManager();
        ~LWMICoreManager();

    public:
        /// @brief ��ʼ��
        /// @param[in] pNamespace ��Ҫ���ӵ����ֿռ�
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool BaseInit(const wchar_t* pNamespace);

        /// @brief WQL��ѯ
        /// @param[in] pQuery ��ѯ���
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool WQLQuery(const wchar_t* pQuery);

        /// @brief ��ȡ��������
        /// @return ���������
        int GetObjectsCount();

        /// @brief ��ȡ�ַ�������
        /// @param[in] objectIndex ��������
        /// @param[in] pPrppertyName ��������
        /// @param[out] strProperty �洢����ֵ
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetStringProperty(int objectIndex, const wchar_t* pPropertyName, wstring& strProperty);

        /// @brief ��ȡ�ַ�������(ˢ�º�)
        /// @param[in] objectIndex ��������
        /// @param[in] pPrppertyName ��������
        /// @param[out] strProperty �洢����ֵ
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetStringPropertyRefreshed(int objectIndex, const wchar_t* pPropertyName, wstring& strProperty);

        /// @brief ��ȡ�޷�����������
        /// @param[in] objectIndex ��������
        /// @param[in] pPropertyName ��������
        /// @param[out] ui8Property �洢����ֵ
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetUINT8Property(int objectIndex, const wchar_t* pPropertyName, LBYTE& ui8Property);

        /// @brief ��ȡ�ֽ���������
        /// @param[in] objectIndex ��������
        /// @param[in] pPrppertyName ��������
        /// @param[out] arrayProperty �洢�ֽ���������
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetUINT8ArrayProperty(int objectIndex, const wchar_t* pPropertyName, vector<LBYTE>& arrayProperty);

        /// @brief ��ȡ�޷�����������
        /// @param[in] objectIndex ��������
        /// @param[in] pPropertyName ��������
        /// @param[out] uiProperty �洢����ֵ
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetUINT16Property(int objectIndex, const wchar_t* pPropertyName, LUINT16& ui16Property);

        /// @brief ��ȡ�޷�����������(ˢ�º�)
        /// @param[in] objectIndex ��������
        /// @param[in] pPropertyName ��������
        /// @param[out] uiProperty �洢����ֵ
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetUINT16PropertyRefreshed(int objectIndex, const wchar_t* pPropertyName, LUINT16& ui16Property);

        /// @brief ��ȡ�޷�����������
        /// @param[in] objectIndex ��������
        /// @param[in] pPropertyName ��������
        /// @param[out] uiProperty �洢����ֵ
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetUINT32Property(int objectIndex, const wchar_t* pPropertyName, LUINT& uiProperty);

        /// @brief ��ȡ�޷�����������(ˢ�º�)
        /// @param[in] objectIndex ��������
        /// @param[in] pPropertyName ��������
        /// @param[out] uiProperty �洢����ֵ
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetUINT32PropertyRefreshed(int objectIndex, const wchar_t* pPropertyName, LUINT& uiProperty);

        /// @brief ��ȡ�޷���64λ��������
        /// @param[in] objectIndex ��������
        /// @param[in] pPropertyName ��������
        /// @param[out] ui64Property �洢����ֵ
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetUINT64Property(int objectIndex, const wchar_t* pPropertyName, LUINT64& ui64Property);

        /// @brief ��ȡ�޷���64λ��������(ˢ�º�)
        /// @param[in] objectIndex ��������
        /// @param[in] pPropertyName ��������
        /// @param[out] ui64Property �洢����ֵ
        /// @return �ɹ�����true, ʧ�ܷ���false
        bool GetUINT64PropertyRefreshed(int objectIndex, const wchar_t* pPropertyName, LUINT64& ui64Property);

    private:
        /// @brief ������Դ
        void BaseCleanUp();

    private:
        IWbemLocator* m_pWbemLocator;
        IWbemServices* m_pWbemServices;

        IEnumWbemClassObject* m_pEnumObject;
        IWbemClassObject** m_pObjectArray;
        int m_objectCount;

        IWbemRefresher* m_pWbemRefresher;
        IWbemConfigureRefresher* m_pWbemConfigRefresher;
        IWbemClassObject** m_pRefreshAbleObjectArray;

        LInitCom* m_pInitComObject;
    };

}


#endif

