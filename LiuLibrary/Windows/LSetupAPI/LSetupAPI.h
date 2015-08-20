#ifndef _LSETUPAPI_H_
#define _LSETUPAPI_H_

#include <string>
using std::wstring;
using std::string;

#include <Windows.h>
#include <devguid.h>
#include <SetupAPI.h>

/// @brief SetupAPI�豸�������
///
///
class LSADevObject
{
public:
	LSADevObject();
	virtual ~LSADevObject();

	/// @brief ��ʼ��
	/// @return �ɹ�����true ʧ�ܷ���false
	virtual bool Init() = 0;

	/// @brief ��ȡ�豸��Ŀ
	/// @return �豸��Ŀ
	int GetDevNum();

	/// @brief �����豸
	///
	/// �÷�����Ҫ����ԱȨ��
	/// @param[in] int index �豸����
	/// @return �ɹ�����true ʧ�ܷ���false
	bool Enable(IN int index);

	/// @brief �����豸
	///
	/// �÷�����Ҫ����ԱȨ��, ��Ҫ�����ſ�Disable���豸, �÷���Ҳ����true
	/// @param[in] index �豸����
	/// @param[out] bNeedRestart �Ƿ���Ҫ����
	/// @return �ɹ�����true ʧ�ܷ���false
	bool Disable(IN int index, OUT bool& bNeedRestart);

	/// @brief ��ȡ�豸����
	/// @param[in] index
	/// @param[out] devDesc
	/// @return �ɹ�����true ʧ�ܷ���false
	bool GetDevDesc(IN int index, OUT string& devDesc);

	/// @brief �豸�Ƿ��Ѿ�������
	/// @param[in] index �豸����
	/// @return �����÷���true ���򷵻�false
	bool IsDisabled(IN int index);

	/// @brief �豸�Ƿ���Ա�����
	/// @param[in] index �豸����
	/// @return ���Ա����÷���true ���򷵻�false
	bool IsDisableEnable(IN int index);

protected:
	/// @brief ��ʼ��
	/// @param[in] pGUID Ҫ��ʼ�����豸��GUID
	/// @return �ɹ�����true ʧ�ܷ���false
	bool BaseInit(IN const GUID* pGUID);

	/// @brief ��ȡ�豸����(�ַ���)
	/// @param[in] index ָ�����豸����
	/// @param[in] dwProperty ָ�����豸����
	/// @param[in] OUT strProperty �豸����
	/// @return �ɹ�����true ʧ�ܷ���false
	bool GetRegistryPropertyStr(IN int index, IN DWORD dwProperty, OUT string& strProperty);

	/// @brief �ı��豸״̬
	///
	/// �÷�����Ҫ����ԱȨ��
	/// @param[in] index �豸����
	/// @param[in] newSate �µ�״̬
	/// @return �ɹ�����true ʧ�ܷ���false
	bool ChangeState(IN int index, IN DWORD newSate, OUT bool& bNeedRestart);

protected:
	HDEVINFO m_hDevInfoSet; ///< �豸��Ϣ��
	int m_devCount; ///< �豸��Ŀ
	SP_DEVINFO_DATA* m_pDevInfoList; ///< �豸��Ϣ�б� 

private:
	/// @brief ������Դ
	void BaseCleanUp();
};

/// @brief ����ѧ�����豸
class LSAHumanInputDev : public LSADevObject
{
public:
	LSAHumanInputDev();
	~LSAHumanInputDev();

	/// @brief ��ʼ��
	virtual bool Init();
};

/// @brief ����豸
class LSAMouseDev : public LSADevObject
{
public:
	LSAMouseDev();
	~LSAMouseDev();

	/// @brief ��ʼ��
	virtual bool Init();
};

class LSABluetoothDev : public LSADevObject
{
public:
	LSABluetoothDev();
	~LSABluetoothDev();

	virtual bool Init();
};
#endif