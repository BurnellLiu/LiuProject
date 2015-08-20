

#ifndef _LMIOBJECT_H_
#define _LMIOBJECT_H_

#include <string>
using std::wstring;
using std::string;

typedef unsigned char LBYTE;
typedef unsigned int LUINT;
typedef unsigned __int64 LUINT64;

struct IWbemLocator;
struct IWbemServices;
struct IEnumWbemClassObject;
struct IWbemClassObject;

/// @brief ���ֽ��ַ���ת��Ϊ���ֽ��ַ���
/// @param[in] strSrc Դ�ַ���
/// @param[in] strDest Ŀ���ַ���
/// @return �ɹ�����true, ʧ�ܷ���false
bool LStringToWString(const string& strSrc, wstring& strDest);

/// @brief ���ֽ��ַ���ת��Ϊ���ֽ��ַ���
/// @param[in] strSrc Դ�ַ���
/// @param[in] strDest Ŀ���ַ���
/// @return �ɹ�����true, ʧ�ܷ���false
bool LWStringToString(const wstring& strSrc, string& strDest);

/// @brief WMI�������
///
///
class LWMIObject
{
public:
	LWMIObject();
	virtual ~LWMIObject();

public:
	/// @brief ��ʼ��
	/// @return �ɹ�����true, ʧ�ܷ���false
	virtual bool Init() = 0;

protected:
	/// @brief ��ʼ��
	/// @param[in] pNamespace ��Ҫ���ӵ����ֿռ�
	/// @return �ɹ�����true, ʧ�ܷ���false
	bool BaseInit(const wchar_t* pNamespace);

	/// @brief WQL��ѯ
	/// @param[in] pQuery ��ѯ���
	/// @return �ɹ�����true, ʧ�ܷ���false
	bool WQLQuery(const wchar_t* pQuery);

	/// @brief ��ȡ�ַ�������
	/// @param[in] objectIndex ��������
	/// @param[in] pPrppertyName ��������
	/// @param[out] strProperty �洢����ֵ
	/// @return �ɹ�����true, ʧ�ܷ���false
	bool GetStringProperty(int objectIndex, const wchar_t* pPropertyName, wstring& strProperty);

	/// @brief ��ȡ�޷�����������
	/// @param[in] objectIndex ��������
	/// @param[in] pPropertyName ��������
	/// @param[out] uiProperty �洢����ֵ
	/// @return �ɹ�����true, ʧ�ܷ���false
	bool GetUINT32Property(int objectIndex, const wchar_t* pPropertyName, LUINT& uiProperty);

	/// @brief ��ȡ�޷���64λ��������
	/// @param[in] objectIndex ��������
	/// @param[in] pPropertyName ��������
	/// @param[out] ui64Property �洢����ֵ
	/// @return �ɹ�����true, ʧ�ܷ���false
	bool GetUINT64Property(int objectIndex, const wchar_t* pPropertyName, LUINT64& ui64Property);

protected:
	IWbemLocator* m_pWbemLocator;
	IWbemServices* m_pWbemServices;

	IEnumWbemClassObject* m_pEnumObject;
	IWbemClassObject** m_pObjectArray;
	int m_objectCount;

private:
	/// @brief ������Դ
	void BaseCleanUp();

};

/// @brief WMI HDDSmart��
///
/// ʹ�ø������������ǰ,����ʹ�÷���Init()����ʼ������
/// ʹ�ø���,��ʹ�ù���ԱȨ�������г���
class LWMIHddSmart : public LWMIObject
{
public:
	LWMIHddSmart();
	virtual ~LWMIHddSmart();

public:
	/// @brief ��ȡHDD��Ŀ
	/// @return HDD��Ŀ
	int GetHDDNum();

	/// @brief ��ȡHDD�ĵ�ǰ�¶�
	/// @param[in] hddIndex HDD����
	/// @return �ɹ���ȡ���ص�ǰ�¶�,ʧ�ܷ���-1
	int GetHDDCurrentTemperature(int hddIndex);

	/// @brief ��ȡӲ��ʵ������
	/// @param[in] hddIndex Ӳ������
	/// @param[out] instanceName �洢ʵ������
	/// @return �ɹ�����true, ʧ�ܷ���false
	bool GetHDDInstanceName(int hddIndex, wstring& instanceName);

	/// @brief ��ʼ��
	/// @return �ɹ�����true, ʧ�ܷ���false
	virtual bool Init();

private:
	/// @breief SMART ���Խṹ
	struct SmartAttriubets
	{
		LBYTE Attrib;
		LBYTE Flags;
		LBYTE Worst;
		LBYTE Normal;
		LBYTE Curent;
		LBYTE Curent1;
		LBYTE Curent2;
		LBYTE Curent3;
		LBYTE Curent4;
		LBYTE Curent5;
		LBYTE Curent6;
		LBYTE Curent7;
	};

	/// @breief SMART ����
	enum SMART_ATTRIB
	{
		USE_TIME = 0x09,  ///< ʹ��ʱ��
		TEMPERATURE = 0xc2 ///< �¶�
	};

	/// @brief ��ȡHDD Smart����
	/// @param[in] hddIndex HDD����
	/// @param[in] attrib ��Ҫ��ȡ������
	/// @param[out] pSmartAttributes �洢Smart����
	/// @return �ɹ�����true, ʧ�ܷ���false
	bool GetSmartAttribute(int hddIndex, LBYTE attrib, SmartAttriubets* pSmartAttributes);

private:
	// ��ֹ��ֵ�������͸��ƹ��캯��
	LWMIHddSmart(const LWMIHddSmart&);
	LWMIHddSmart& operator = (const LWMIHddSmart&);
};

/// @brief WMI VideoController��
///
/// ʹ�ø������������ǰ,����ʹ�÷���Init()����ʼ������
class LWMIVideoController : public LWMIObject
{
public:
	LWMIVideoController();
	virtual ~LWMIVideoController();

public:
	/// @brief ��ʼ��
	/// @return �ɹ�����true, ʧ�ܷ���false
	virtual bool Init();

	/// @brief ��ȡVideo��Ŀ
	/// @return Video��Ŀ
	int GetVideoNum();

	/// @brief ��ȡvideo PNPDeviceID
	/// @param[in] videoIndex video����
	/// @param[out] pnpDeviceID
	/// @return �ɹ�����true, ʧ�ܷ���false
	bool GetPNPDeviceID(int videoIndex, wstring& pnpDeviceID);

	/// @brief ��ȡvideo AdapterRAM
	/// @param[in] videoIndex video����
	/// @param[out] adapterRAM �洢adapterRAM
	/// @return �ɹ�����true, ʧ�ܷ���false
	bool GetAdapterRAM(int videoIndex, LUINT& adapterRAM);

	/// @brief ��ȡvideo �豸����
	/// @param[in] videoIndex video����
	/// @param[out] description �洢�豸����
	/// @return �ɹ�����true, ʧ�ܷ���false
	bool GetDescription(int videoIndex, wstring& description);

private:
	// ��ֹ��ֵ�������͸��ƹ��캯��
	LWMIVideoController(const LWMIVideoController&);
	LWMIVideoController& operator = (const LWMIVideoController&);
};

class LWMIDiskDrive : public LWMIObject
{
public:
	LWMIDiskDrive();
	~LWMIDiskDrive();

public:
	/// @brief ��ʼ��
	/// @return �ɹ�����true, ʧ�ܷ���false
	virtual bool Init();

	/// @brief ��ȡDisk��Ŀ
	/// @return Disk��Ŀ
	int GetDiskNum();

	/// @brief ��ȡDiskModel
	/// @param[in] diskIndex Disk����
	/// @param[out] diskModel �洢DiskModel
	/// @return �ɹ�����true, ʧ�ܷ���false
	bool GetDiskModel(int diskIndex, wstring& diskModel);

	/// @brief ��ȡDiskSize
	/// @param[in] diskIndex Disk����
	/// @param[out] diskSize �洢DiskSize
	/// @return �ɹ�����true, ʧ�ܷ���false
	bool GetDiskSize(int diskIndex, LUINT64& diskSize);

private:
	// ��ֹ��ֵ�������͸��ƹ��캯��
	LWMIDiskDrive(const LWMIDiskDrive&);
	LWMIDiskDrive& operator = (const LWMIDiskDrive&);
};

#endif

