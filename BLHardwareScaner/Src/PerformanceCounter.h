

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


/// @brief 内存性能
struct MemoryPerformance
{
    unsigned long TotalSize; ///< 总内存大小, 单位M
    unsigned long AvailableSize; ///< 可用内存大小, 单位M
    unsigned long UnusedSize; ///< 未使用内存大小, 单位M
};

/// @brief 处理器性能性能
struct ProcessorPerformance
{
    unsigned long LoadPercentage; ///< CPU使用率, 范围(1-100)
};

class CPerformanceCounter;

/// @brief 性能计数器类
class PerformanceCounter
{
public:
    PerformanceCounter();
    ~PerformanceCounter();


    /// @brief 获取内存性能
    /// @param[out] memoryPerformance 存储内存性能
    /// @return 成功返回true, 失败返回false
    bool GetMemoryPerformance(OUT MemoryPerformance& memoryPerformance);

    /// @brief 获取处理器性能
    /// @param[out] processorPerformance 存储处理器性能
    /// @return 成功返回true, 失败返回false
    bool GetProcessorPerformance(OUT ProcessorPerformance& processorPerformance);

private:
    CPerformanceCounter* m_pCPerformanceCounter; ///< 性能计数器实现对象
};


#endif