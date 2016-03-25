/// @file LCpuStress.h
/// @brief CPUѹ������ͷ�ļ� 
/// 
/// Detail:
/// @author Burnell Liu Email:burnell_liu@outlook.com
/// @version   
/// @date 2016:3:25

#ifndef _LCPUSTRESS_H_
#define _LCPUSTRESS_H_

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#define MAX_CPU_LOGICAL_CORE_NUM 64

/// @brief CPUѹ������״̬
struct LCpuStressTestState 
{
    bool TestDone; ///< ��ǲ����Ƿ��Ѿ�����
    unsigned int LogicalCoreNum; ///< CPU�߼���������
    unsigned long Score[MAX_CPU_LOGICAL_CORE_NUM]; ///< ÿ��CPU�߼����ĵĵ÷�(һ��ʱ���ڼ���Բ����PI��λ��)
};

class CCpuStressTest;

/// @brief CPUѹ������
class LCpuStressTest
{
public:
    /// @brief ���캯��
    LCpuStressTest();

    /// @brief ��������
    ~LCpuStressTest();

    /// @brief ��ʼ����
    /// @param[in] timeOut ��ʱʱ��, ��λ��
    /// @return �ɹ�����true, ʧ�ܷ���false, �������ڽ����л᷵��false
    bool Start(IN unsigned int timeOut);

    /// @brief ��ȡ����״̬
    /// @return ����״̬
    const LCpuStressTestState& GetState();

    /// @brief ֹͣ����
    void Stop();

private:
    CCpuStressTest* m_pCpuStressTest; ///< CPUѹ������ʵ�ֶ���

private:
    // ��ֹ�������캯���͸�ֵ������
    LCpuStressTest(const LCpuStressTest&);
    LCpuStressTest& operator = (const LCpuStressTest&);
};

#endif