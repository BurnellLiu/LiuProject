

#ifndef _HARDWAREINFOR_H_
#define _HARDWAREINFOR_H_

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

/// @brief ��ȡ���������
enum PC_SYSTEM_TYPE
{
    UNKNOWN = 0, ///< δ֪
    DESKTOP = 1, ///< ̨ʽ��
    NOTE_BOOK = 2, ///< �ʼǱ�
    TABLET = 3 ///< ƽ�����
};

/// @brief �����ϵͳ��Ϣ�ṹ
struct ComputerSystemInfor
{
    wstring ModelName; ////< ������
    PC_SYSTEM_TYPE Type; ///< ���������
    wstring Manufacturer; ///< ������
    
};

/// @brief ����ϵͳ��Ϣ�ṹ
struct OperatingSystemInfor
{
    wstring Caption; ////< ����
    wstring Architecture; ////< �ܹ�
    wstring Version; ////< �汾
    wstring SystemDrive; ////< ϵͳ��
};

/// @brief Ӳ����Ϣ��
///
/// �����ȡ�Ķ��ǹ̶���Ϣ
class HardwareInfor
{
public:
    /// @brief ��ȡʵ������
    /// @return ʵ������
    static HardwareInfor& GetInstance();

    /// @brief ��ȡ�����ϵͳ��Ϣ
    /// @return �����ϵͳ��Ϣ
    const ComputerSystemInfor& GetComputerSystemInfor() const; 

    /// @brief ��ȡ����ϵͳ��Ϣ
    /// @return ����ϵͳ��Ϣ
    const OperatingSystemInfor& GetOperatingSystemInfor() const;

    /// @brief ��������
    ~HardwareInfor();

private:
    /// @brief ɨ��Ӳ����Ϣ
    void Scan();
    
    /// @brief ɨ������ϵͳ��Ϣ
    /// @param[in] computerSystemInfor �洢�����ϵͳ��Ϣ
    void ScanComputerSystemInfor(OUT ComputerSystemInfor& computerSystemInfor);

    /// @brief ɨ�����ϵͳ��Ϣ
    /// @param[in] operatingSystemInfor ����ϵͳ��Ϣ
    void ScanOperatingSystemInfor(OUT OperatingSystemInfor& operatingSystemInfor);

    HardwareInfor(); // ��ֹ����
    HardwareInfor(const HardwareInfor&); // ��ֹĬ�Ͽ������캯��
    HardwareInfor& operator = (const HardwareInfor&); // ��ֹ��ֵ������

private:
    ComputerSystemInfor m_computerSystemInfor; ///< �����ϵͳ��Ϣ
    OperatingSystemInfor m_operatingSystemInfor; ///< ����ϵͳ��Ϣ
};

#endif