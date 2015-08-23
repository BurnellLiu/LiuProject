

#ifndef _LCPUID_H_
#define _LCPUID_H_

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

/// @brief CPUIDָ����
class LCPUID
{
public:
    LCPUID();
    ~LCPUID();

    /// @brief ��ȡCPU������Ϣ
    /// @param[out] vendor
    /// @return �ɹ�����true, ʧ�ܷ���false(CPU��֧��)
    bool GetVendor(OUT string& vendor);

    /// @brief ��ȡCPU�̱���Ϣ
    /// @param[out] brand
    /// @return �ɹ�����true, ʧ�ܷ���false(CPU��֧��)
    bool GetBrand(OUT string& brand);
    
private:
    /// @brief ��ȡCPU֧�ֵ��������
    /// @return CPU֧�ֵ��������
    unsigned int GetMaxCommand();
    
    /// @brief ��ȡCPU֧�ֵ������չ����
    /// @return CPU֧�ֵ������չ����
    unsigned int GetMaxExtendedCommand();

    /// @brief ִ��CPUID����
    /// @param[in] cmd ����ֵ
    /// @param[out] cpuInfo ���������
    /// @return �ɹ�����true, ʧ�ܷ���false(cpu��֧�ָ�����)
    bool ExecuteCPUID(IN unsigned int cmd, OUT unsigned int cpuInfo[4]);

private:
    unsigned int m_maxCommand; ///< CPU֧�ֵ��������
    unsigned int m_maxExtendedCommand; ///< CPU֧�ֵ������չ����
};

#endif