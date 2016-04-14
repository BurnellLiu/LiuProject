#ifndef _LSETUPAPI_H_
#define _LSETUPAPI_H_

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

class CSADevObject;

/// <SUMMARY>
/// �豸����ӿ�
/// </SUMMARY>
class LSetupDev
{
public:
    virtual ~LSetupDev();

    /// <SUMMARY>
    /// ��ȡ�豸��Ŀ
    /// </SUMMARY>
    /// <RETURNS>
    /// �豸��Ŀ
    /// </RETURNS>
    int GetDevNum();

    /// <SUMMARY>
    /// �����豸
    /// �÷���������Ҫ����ԱȨ��
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// �豸����
    /// </PARAM>
    /// <RETURNS>
    /// �ɹ�����0 ʧ�ܷ���ErrorCode, ����ϵͳ��GetLastError()
    /// </RETURNS>
    unsigned long Enable(IN int index);

    /// <SUMMARY>
    /// �����豸
    /// �÷�����Ҫ����ԱȨ��, ��Ҫ�����ſ�Disable���豸, �÷���Ҳ����0
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// �豸����
    /// </PARAM>
    /// <RETURNS>
    /// �ɹ�����0 ʧ�ܷ���ErrorCode, ����ϵͳ��GetLastError()
    /// </RETURNS>
    unsigned long Disable(IN int index);

    /// <SUMMARY>
    /// ��ȡ�豸����
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// �豸����
    /// </PARAM>
    /// <PARAM name = "devDesc" dir = "OUT">
    /// �����豸����
    /// </PARAM>
    /// <RETURNS>
    /// �ɹ�����0 ʧ�ܷ���ErrorCode, ����ϵͳ��GetLastError()
    /// </RETURNS>
    unsigned long GetDevDesc(IN int index, OUT wstring& devDesc);

    /// <SUMMARY>
    /// ��ȡ�豸Ӳ��ID
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// �豸����
    /// </PARAM>
    /// <PARAM name = "devHardwareId" dir = "OUT">
    /// �����豸Ӳ��ID
    /// </PARAM>
    /// <RETURNS>
    /// �ɹ�����0 ʧ�ܷ���ErrorCode, ����ϵͳ��GetLastError()
    /// </RETURNS>
    unsigned long GetHardwareId(IN int index, OUT wstring& devHardwareId);

    /// <SUMMARY>
    /// ��ȡ�豸�Ѻ�����
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// �豸����
    /// </PARAM>
    /// <PARAM name = "devFrindName" dir = "OUT">
    /// �����豸�Ѻ�����
    /// </PARAM>
    /// <RETURNS>
    /// �ɹ�����0 ʧ�ܷ���ErrorCode, ����ϵͳ��GetLastError()
    /// </RETURNS>
    unsigned long GetFriendlyName(IN int index, OUT wstring& devFrindName);

    /// <SUMMARY>
    /// ��ȡ�豸λ����Ϣ
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// �豸����
    /// </PARAM>
    /// <PARAM name = "devLocationInfo" dir = "OUT">
    /// �����豸λ����Ϣ
    /// </PARAM>
    /// <RETURNS>
    /// �ɹ�����0 ʧ�ܷ���ErrorCode, ����ϵͳ��GetLastError()
    /// </RETURNS>
    unsigned long GetLoctionInfo(IN int index, OUT wstring& devLocationInfo);

    /// <SUMMARY>
    /// ��ȡ�豸ʵ��ID(��д)
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// �豸����
    /// </PARAM>
    /// <PARAM name = "devLocationInfo" dir = "OUT">
    /// �����豸ʵ��ID
    /// </PARAM>
    /// <RETURNS>
    /// �ɹ�����0 ʧ�ܷ���ErrorCode, ����ϵͳ��GetLastError()
    /// </RETURNS>
    unsigned long GetInstanceID(IN int index, OUT wstring& devInstanceID);

    /// <SUMMARY>
    /// ��ȡ��ϵ�豸ʵ��ID(��д)
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// �豸����
    /// </PARAM>
    /// <PARAM name = "devLocationInfo" dir = "OUT">
    /// ���游ϵ�豸ʵ��ID
    /// </PARAM>
    /// <RETURNS>
    /// �ɹ�����0 ʧ�ܷ���ErrorCode, ����ϵͳ��GetLastError()
    /// </RETURNS>
    unsigned long GetParentInstanceId(IN int index, OUT wstring& devInstanceID);

    /// <SUMMARY>
    /// ��ȡ�Ӵ��豸��Ŀ
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// �豸����
    /// </PARAM>
    /// <PARAM name = "number" dir = "OUT">
    /// �����Ӵ��豸��Ŀ
    /// </PARAM>
    /// <RETURNS>
    /// �ɹ�����0 ʧ�ܷ���ErrorCode, ����ϵͳ��GetLastError()
    /// </RETURNS>
    unsigned long GetChildrenNumber(IN int index, OUT int& number);

    /// <SUMMARY>
    /// ��ȡ�Ӵ��豸ʵ��ID�б�(��д)
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// �豸����
    /// </PARAM>
    /// <PARAM name = "listSize" dir = "IN">
    /// �б��С
    /// </PARAM>
    /// <PARAM name = "devInstanceIDList" dir = "OUT">
    /// �����Ӵ��豸ʵ��ID�б�(����ʹ��GetChildrenNumber�������Ҫ�Ŀռ�)
    /// </PARAM>
    /// <RETURNS>
    /// �ɹ�����0, ʧ�ܷ���ErrorCode, ����ϵͳ��GetLastError()
    /// </RETURNS>
    unsigned long GetChildrenInstanceIdList(IN int index, IN int listSize,OUT wstring* devInstanceIDList);

    /// <SUMMARY>
    /// ��ȡ�豸����������
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// �豸����
    /// </PARAM>
    /// <PARAM name = "driverKeyName" dir = "OUT">
    /// �����豸����������
    /// </PARAM>
    /// <RETURNS>
    /// �ɹ�����0 ʧ�ܷ���ErrorCode, ����ϵͳ��GetLastError()
    /// </RETURNS>
    unsigned long GetDriverKeyName(IN int index, OUT wstring& driverKeyName);

    /// <SUMMARY>
    /// ��ȡ�豸����ַ���
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// �豸����
    /// </PARAM>
    /// <PARAM name = "strClass" dir = "OUT">
    /// �����豸����ַ���
    /// </PARAM>
    /// <RETURNS>
    /// �ɹ�����0 ʧ�ܷ���ErrorCode, ����ϵͳ��GetLastError()
    /// </RETURNS>
    unsigned long GetClass(IN int index, OUT wstring& strClass);

    /// <SUMMARY>
    /// ��ȡ�豸���GUID�ַ���
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// �豸����
    /// </PARAM>
    /// <PARAM name = "strClassGuid" dir = "OUT">
    /// �����豸���GUID�ַ���
    /// </PARAM>
    /// <RETURNS>
    /// �ɹ�����0 ʧ�ܷ���ErrorCode, ����ϵͳ��GetLastError()
    /// </RETURNS>
    unsigned long GetClassGuid(IN int index, OUT wstring& strClassGuid);

    /// <SUMMARY>
    /// ��ȡ�豸���ߺ�
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// �豸����
    /// </PARAM>
    /// <PARAM name = "busNumber" dir = "OUT">
    /// �����豸���ߺ�
    /// </PARAM>
    /// <RETURNS>
    /// �ɹ�����0 ʧ�ܷ���ErrorCode, ����ϵͳ��GetLastError()
    /// </RETURNS>
    unsigned long GetBusNumber(IN int index, OUT unsigned int& busNumber);

    /// <SUMMARY>
    /// ��ȡ�豸������
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// �豸����
    /// </PARAM>
    /// <PARAM name = "manufacturer" dir = "OUT">
    /// �����豸������
    /// </PARAM>
    /// <RETURNS>
    /// �ɹ�����0 ʧ�ܷ���ErrorCode, ����ϵͳ��GetLastError()
    /// </RETURNS>
    unsigned long GetManufacturer(IN int index, OUT wstring& manufacturer);

    /// <SUMMARY>
    /// ��ȡƥ���豸ID
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// �豸����
    /// </PARAM>
    /// <PARAM name = "matchingDeviceId" dir = "OUT">
    /// ����ƥ���豸ID
    /// </PARAM>
    /// <RETURNS>
    /// �ɹ�����0 ʧ�ܷ���ErrorCode, ����ϵͳ��GetLastError()
    /// </RETURNS>
    unsigned long GetMatchingDeviceId(IN int index, OUT wstring& matchingDeviceId);

protected:
    LSetupDev();

protected:
    CSADevObject* m_pSADevObject;
};

/// <SUMMARY>
/// �����豸��
/// </SUMMARY>
class LSetupDevAll : public LSetupDev
{
public:
    LSetupDevAll();
    ~LSetupDevAll();
};

/// <SUMMARY>
/// �Կ��豸��
/// </SUMMARY>
class LSetupDisplayCard : public LSetupDev
{
public:
    LSetupDisplayCard();
    ~LSetupDisplayCard();
};

/// <SUMMARY>
/// ��ʾ����չ��Ϣ
/// </SUMMARY>
struct LMonitorExtendInfor
{
    wstring Name; ///< ����
    wstring VendorID; ///< ����ID
    wstring ProductID; ///< ��ƷID
    wstring Date; ///< �������� (��.��)
};

struct LMonitorEDID;

/// <SUMMARY>
/// ��ʾ���豸��
/// </SUMMARY>
class LSetupMonitor : public LSetupDev
{
public:
    LSetupMonitor();
    ~LSetupMonitor();

    /// <SUMMARY>
    /// ��ȡ��ʾ����չ��Ϣ
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// �豸����
    /// </PARAM>
    /// <PARAM name = "extendInfor" dir = "OUT">
    /// �洢��չ��Ϣ
    /// </PARAM>
    /// <RETURNS>
    /// �ɹ�����true, ʧ�ܷ���false
    /// </RETURNS>
    bool GetExtendInfor(IN int index, OUT LMonitorExtendInfor& extendInfor);

private:
    /// <SUMMARY>
    /// ��ȡ��ʾ����չ��ʾ��ʶ����
    /// </SUMMARY>
    /// <PARAM name = "index" dir = "IN">
    /// �豸����
    /// </PARAM>
    /// <PARAM name = "extendInfor" dir = "OUT">
    /// �洢��չ��ʾ��ʶ����
    /// </PARAM>
    /// <RETURNS>
    /// �ɹ�����true, ʧ�ܷ���false
    /// </RETURNS>
    bool GetEDID(IN int index, OUT LMonitorEDID& edid);
};

/// <SUMMARY>
/// SCSI��RAID�������豸��
/// </SUMMARY>
class LSetupSCSIController : public LSetupDev
{
public:
    LSetupSCSIController();
    ~LSetupSCSIController();
};

/// <SUMMARY>
/// ͼ���豸��
/// </SUMMARY>
class LSetupImage : public LSetupDev
{
public:
    LSetupImage();
    ~LSetupImage();
};

#endif