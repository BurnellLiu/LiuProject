

#ifndef _LSMBIOSPASER_H_
#define _LSMBIOSPARSER_H_

#include <vector>
using std::vector;
#include <string>
using std::string;

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif



/// @brief SMBios BIOS ��Ϣ
struct SMBiosBIOSInfor
{
    string Vendor; /// <������
    string Version; ///< �汾
    string ReleaseDate; ///< ��������
    unsigned long RomSize; ///< ROM��С, ��λK
};

/// @brief SMBios ϵͳ��Ϣ
struct SMBiosSystemInfor
{
    string Manufacturer; ///< ����������
    string ProductName; ///< ��������
    string Version; ///< ���԰汾
    string SerialNumber; ///< �������к�
    unsigned char UUID[16]; ///< ����Ψһ��ʶ��
};

/// @brief SMBios ������Ϣ
struct SMBiosBaseBoardInfor
{
    string Manufacturer; ///< ����������
    string Product; ///< ������
    string Version; ///< ����汾
    string SerialNumber; ///< �������к�
};

class CSMBiosPaser;

/// @brief SMBios������
class LSMBiosPaser
{
public:
    /// @brief ���캯��
    /// @param[in] smBiosData SMBios����
    explicit LSMBiosPaser(IN const vector<unsigned char>& smBiosData);

    /// @brief ��������
    ~LSMBiosPaser();

    /// @brief ��ȡBIOS��Ϣ
    /// @param[out] biosInfor �洢BIOS��Ϣ
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GetBiosInfor(OUT SMBiosBIOSInfor& biosInfor);

    /// @brief ��ȡϵͳ��Ϣ
    /// @param[out] systemInfor �洢ϵͳ��Ϣ
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GetSystemInfor(OUT SMBiosSystemInfor& systemInfor);

    /// @brief ��ȡ������Ϣ
    /// @param[out] baseBoardInfor �洢������Ϣ
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GetBaseBoardInfor(OUT SMBiosBaseBoardInfor& baseBoardInfor);

private:
    CSMBiosPaser* m_pSMBiosPaser; ///< SMBios��������
};



#endif