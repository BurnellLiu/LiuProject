

#ifndef _PERFORMANCECOUNTER_H_
#define _PERFORMANCECOUNTER_H_

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


/// @brief �ڴ�����
struct MemoryPerformance
{
    unsigned long TotalSize; ///< ���ڴ��С, ��λM
    unsigned long AvailableSize; ///< �����ڴ��С, ��λM
    unsigned long LoadPercentage; ///< �ڴ�ʹ����, ��Χ(1-100)
};

/// @brief ����������
struct ProcessorPerformance
{
    unsigned long LoadPercentage; ///< CPUʹ����, ��Χ(1-100)
};

/// @brief ����ȡ������������
#ifndef MAX_DISKPERFORMANCE_NUMBER
#define MAX_DISKPERFORMANCE_NUMBER 8
#endif

/// @brief Disk����
struct DiskPerformance
{
    unsigned int Count; ///< ��ȡ�����ܵĴ�������
    unsigned int UsageRate[MAX_DISKPERFORMANCE_NUMBER]; ///< ����ʹ����
    wstring DiskDriveID[MAX_DISKPERFORMANCE_NUMBER]; ///< ����������ID, �� L"\\\\.\\PhysicalDrive0"
};

class CPerformanceCounter;

/// @brief ���ܼ�������
class PerformanceCounter
{
public:
    /// @brief ���캯��
    PerformanceCounter();

    /// @brief ��������
    ~PerformanceCounter();

    /// @brief ��ȡ�ڴ�����
    /// @param[out] memoryPerformance �洢�ڴ�����
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GetMemoryPerformance(OUT MemoryPerformance& memoryPerformance);

    /// @brief ��ȡ����������
    /// @param[out] processorPerformance �洢����������
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GetProcessorPerformance(OUT ProcessorPerformance& processorPerformance);

    /// @brief ��ȡ��������
    /// �÷���ֻ�ܻ�ȡ�̶����̵�����
    /// @param[in] diskPerformance �洢��������
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GetDiskPerformance(OUT DiskPerformance& diskPerformance);

private:
    CPerformanceCounter* m_pCPerformanceCounter; ///< ���ܼ�����ʵ�ֶ���
};


#endif