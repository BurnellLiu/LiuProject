

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

/// @brief SMART���ݳ���
#ifndef SMART_DATA_LENGTH
#define SMART_DATA_LENGTH 362
#endif

/// @brief ��ȡ�߼�����ʣ��ռ�
/// @param[in] logicalDrive �߼���������
/// @param[out] pFreeSpace �洢ʣ��ռ�, ��λ�ֽ�
/// @return �ɹ�����true, ʧ�ܷ���false 
bool GetLogicalDriveFreeSpace(IN const wstring& logicalDrive, unsigned long long* pFreeSpace);

class CIDEDiskController;

/// @brief IDE(ATA)���̿�����
///
/// ʹ�ø�����Ҫ����ԱȨ��
class LIDEDiskController
{
public:
    /// @brief ���캯��
    /// @param[in] devicePath �豸·��
    /// �豸·����ʽΪ(C����)"\\\\.\\DeviceName"
    /// �豸������: PhysicalDrive0, PhysicalDrive1, ...
    LIDEDiskController(IN const wstring& devicePath);
    ~LIDEDiskController();

    /// @brief �ж��豸�Ƿ����
    /// @return ���ڷ���true, �����ڷ���false
    bool DeviceExist();

    /// @brief ��ȡ�궨ת��
    /// @return �궨ת��(��λRPM), �������󷵻�0, ����ֵΪ1��ʾӲ��ΪSSD
    unsigned long GetRotationRate();

    /// @brief ��ȡSATA����
    /// @return SATA�ӿ�����, 1(SATA1.0 1.5Gb/s), 2(SATA2.0 3.0Gb/s), 3(SATA3.0 6.0Gb/s), 0(��ȡʧ��)
    unsigned long GetSATAType();

    /// @brief ��ȡSMART����
    /// @param[out] smartData[362] �洢362���ֽڵ�SMART����
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GetSMARTData(OUT unsigned char smartData[SMART_DATA_LENGTH]);

private:
    CIDEDiskController* m_pIDEDiskController; ///< IDE���̿�����ʵ�ֶ���
};

#endif