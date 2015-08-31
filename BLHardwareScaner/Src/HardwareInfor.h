

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

/// @brief ������Ϣ�ṹ
struct BaseBoardInfor
{
    wstring Description; ///< ����
    wstring Manufacturer; ///< ������
    wstring SerialNumber; ///< ���к�
};

/// @brief ��������Ϣ
struct ProcessorInfor
{
    wstring Name; ///< ����
    wstring Description; ///< ����
    wstring Manufacturer; ///< ������
    unsigned long CoresNumber; ///< ������
    unsigned long LogicalProcessorNumber; ///< �߼�����������
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

    /// @brief ��ȡ������Ϣ
    /// @return ������Ϣ
    const BaseBoardInfor& GetBaseBoardInfor() const;

    /// @brief ��ȡ��������Ϣ
    /// @return ��������Ϣ
    const ProcessorInfor& GetProcessorInfor() const;

    /// @brief ��������
    ~HardwareInfor();

private:
    /// @brief ɨ��Ӳ����Ϣ
    void Scan();
    
    /// @brief ɨ������ϵͳ��Ϣ
    /// @param[out] computerSystemInfor �洢�����ϵͳ��Ϣ
    void ScanComputerSystemInfor(OUT ComputerSystemInfor& computerSystemInfor);

    /// @brief ɨ�����ϵͳ��Ϣ
    /// @param[out] operatingSystemInfor ����ϵͳ��Ϣ
    void ScanOperatingSystemInfor(OUT OperatingSystemInfor& operatingSystemInfor);

    /// @brief ɨ��������Ϣ
    /// @param[out] baseBoardInfor ������Ϣ
    void ScanBaseBoardInfor(OUT BaseBoardInfor& baseBoardInfor);

    /// @brief ɨ�账������Ϣ
    /// @param[out] processorInfor �洢��������Ϣ
    void ScanProcessorInfor(OUT ProcessorInfor& processorInfor);

    HardwareInfor(); // ��ֹ����
    HardwareInfor(const HardwareInfor&); // ��ֹĬ�Ͽ������캯��
    HardwareInfor& operator = (const HardwareInfor&); // ��ֹ��ֵ������

private:
    ComputerSystemInfor m_computerSystemInfor; ///< �����ϵͳ��Ϣ
    OperatingSystemInfor m_operatingSystemInfor; ///< ����ϵͳ��Ϣ
    BaseBoardInfor m_baseBoardInfor; ///< ������Ϣ
    ProcessorInfor m_processorInfor; ///< ��������Ϣ
};

#endif