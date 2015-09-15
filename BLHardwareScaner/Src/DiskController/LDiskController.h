

#ifndef _LDISKCONTROLLER_H_
#define _LDISKCONTROLLER_H_


#include <string>
using std::wstring;


#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

class CIDEDiskController;

/// @brief IDE(ATA)���̿�����
///
/// ʹ�ø�����Ҫ����ԱȨ��
class LIDEDiskController
{
public:
    /// @brief ���캯��
    /// @param[in] devicePath �豸·��
    /// �豸·����ʽΪ(C����)"////.//DeviceName"
    /// �豸������: PhysicalDrive0, PhysicalDrive1, ...
    LIDEDiskController(IN const wstring& devicePath);
    ~LIDEDiskController();

    /// @brief �ж��豸�Ƿ����
    /// @return ���ڷ���true, �����ڷ���false
    bool DeviceExist();

    /// @brief ��ȡ�궨ת��
    /// @return �궨ת��(��λRPM), �������󷵻�0, ����ֵΪ1��ʾӲ��ΪSSD
    unsigned long GetRotationRate();

private:
    CIDEDiskController* m_pIDEDiskController; ///< IDE���̿�����ʵ�ֶ���
};

#endif