

#ifndef _PERFORMANCECOUNTER_H_
#define _PERFORMANCECOUNTER_H_

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
    unsigned long UnusedSize; ///< δʹ���ڴ��С, ��λM
};

/// @brief ��������������
struct ProcessorPerformance
{
    unsigned long LoadPercentage; ///< CPUʹ����, ��Χ(1-100)
};

class CPerformanceCounter;

/// @brief ���ܼ�������
class PerformanceCounter
{
public:
    PerformanceCounter();
    ~PerformanceCounter();


    /// @brief ��ȡ�ڴ�����
    /// @param[out] memoryPerformance �洢�ڴ�����
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GetMemoryPerformance(OUT MemoryPerformance& memoryPerformance);

    /// @brief ��ȡ����������
    /// @param[out] processorPerformance �洢����������
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GetProcessorPerformance(OUT ProcessorPerformance& processorPerformance);

private:
    CPerformanceCounter* m_pCPerformanceCounter; ///< ���ܼ�����ʵ�ֶ���
};


#endif