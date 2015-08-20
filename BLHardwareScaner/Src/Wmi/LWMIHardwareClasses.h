

#ifndef _LWMIHARDWARECLASSES_H_
#define _LWMIHARDWARECLASSES_H_

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

namespace LWMI
{
    class LWMICoreManager;

     /// @brief 主板管理者类
    class LBaseBoardManager
    {
    public:
        LBaseBoardManager();
        ~LBaseBoardManager();

        /// @brief 获取主板数量
        /// @return 主板数量(应该为1)
        int GetBaseBoardCount();

        /// @brief 获取主板标题
        /// @param[in] index 主板索引
        /// @param[out] caption 存储主板标题
        /// @return 成功返回true, 失败返回false
        bool GetBaseBoardCaption(IN int index, OUT wstring& caption);

        /// @brief 获取主板描述
        /// @param[in] index 主板索引
        /// @param[out] caption 存储主板描述
        /// @return 成功返回true, 失败返回false
        bool GetBaseBoardDescription(IN int index, OUT wstring& description);

        /// @brief 获取主板制造商
        /// @param[in] index 主板索引
        /// @param[out] manufacturer 存储主板制造商
        /// @return 成功返回true, 失败返回false
        bool GetBaseBoardManufacturer(IN int index, OUT wstring& manufacturer);

        /// @brief 获取主板序列号
        /// @param[in] index 主板索引
        /// @param[存储序列号] serialNumber
        /// @return 成功返回true, 失败返回false
        bool GetBaseBoardSerialNumber(IN int index, OUT wstring& serialNumber);

    private:
        LWMICoreManager* m_pWMICoreManager; ///< WMI核心管理者
    };

    /// @brief BIOS管理者类
    class LBIOSManager
    {
    public:
        LBIOSManager();
        ~LBIOSManager();

        /// @brief 获取BIOS数量
        /// @return BIOS数量(应该为1)
        int GetBIOSCount();

        /// @brief 获取BIOS描述
        /// @param[in] index BIOS索引
        /// @param[out] description 存储BIOS描述
        /// @return 成功返回true, 失败返回false
        bool GetBIOSDescription(IN int index, OUT wstring& description);

        /// @brief 获取BIOS制造商
        /// @param[in] index BIOS索引
        /// @param[out] manufacturer 存储BIOS制造商
        /// @return 成功返回true, 失败返回false
        bool GetBIOSManufacturer(IN int index, OUT wstring& manufacturer);

        /// @brief 获取BIOS序列号
        /// @param[in] index BIOS索引
        /// @param[out] serialNumber 存储BIOS序列号
        /// @return 成功返回true, 失败返回false
        bool GetBIOSSerialNumber(IN int index, OUT wstring& serialNumber);

        /// @brief 获取BIOS版本
        /// @param[in] index BIOS索引
        /// @param[in] version 存储BIOS版本
        /// @return 成功返回true, 失败返回false
        bool GetBIOSVersion(IN int index, OUT wstring& version);

        /// @brief 获取SMBIOS BIOS版本
        /// @param[in] index BIOS索引
        /// @param[in] version 存储SMBIOS BIOS版本
        /// @return 成功返回true, 失败返回false
        bool GetSMBIOSBIOSVersion(IN int index, OUT wstring& version);

    private:
        LWMICoreManager* m_pWMICoreManager; ///< WMI核心管理者
    };

    /// @brief 处理器管理者类
    class LProcessorManager
    {
    public:
        LProcessorManager();
        ~LProcessorManager();

        /// @brief 处理器架构
        enum LPROCESSOR_ARCHITECTURE
        {
            X86 = 0,
            MIPS = 1,
            ALPHA = 2,
            POWER_PC = 3,
            ARM = 5,
            IA64 = 6,
            X64 = 9
        };

        /// @brief 获取处理器数量
        /// @return 处理器数量(一般为1)
        int GetProcessorCount();

        /// @brief 获取处理器名称
        /// @param[in] index 处理器索引
        /// @param[out] name 存储名称
        /// @return 成功返回true, 失败返回false
        bool GetProcessorName(IN int index, OUT wstring& name);
        
        /// @brief 获取处理器描述
        /// @param[in] index 处理器索引
        /// @param[out] description 存储描述信息
        /// @return 成功返回true, 失败返回false
        bool GetProcessorDesription(IN int index, OUT wstring& description);

        /// @brief 获取处理器架构信息
        /// @param[in] index 处理器索引
        /// @param[out] architecture 存储架构信息
        /// @return 成功返回true, 失败返回false
        bool GetProcessorArchitecture(IN int index, OUT LPROCESSOR_ARCHITECTURE& architecture);
        
        /// @brief 获取处理器制造商
        /// @param[in] index 处理器索引
        /// @param[out] manufacturer 存储制造商
        /// @return 成功返回true, 失败返回false
        bool GetProcessorManufacturer(IN int index, OUT wstring& manufacturer);
    
        /// @brief 获取处理器最大频率(单位MHz)
        /// @param[in] index 处理器索引
        /// @param[in] speed 存储频率
        /// @return 成功返回true, 失败返回false
        bool GetProcessorMaxClockSpeed(IN int index, OUT unsigned long& speed);

        /// @brief 获取处理器核心数
        /// @param[in] index 处理器索引
        /// @param[in] coresNumber 存储核心数
        /// @return 成功返回true, 失败返回false
        bool GetProcessorCoresNumber(IN int index, OUT unsigned long& coresNumber);

        /// @brief 获取处理器逻辑处理器数量(线程数)
        /// @param[in] index 处理器索引
        /// @param[in] logicalProcessorsNumber 逻辑处理器数量
        /// @return 成功返回true, 失败返回false
        bool GetProcessorLogicalProcessorNumber(IN int index, OUT unsigned long& logicalProcessorsNumber);

    private:
        LWMICoreManager* m_pWMICoreManager; ///< WMI核心管理者
    };

    /// @brief 物理内存管理者类
    class LPhysicalMemoryManager
    {
    public:
        LPhysicalMemoryManager();
        ~LPhysicalMemoryManager();

        /// @brief 获取物理内存数量
        /// @return 物理内存数量
        int GetPhysicalMemoryCount();

        /// @brief 获取物理内存描述
        /// @param[in] index 内存索引
        /// @param[out] description 存储描述
        /// @return 成功返回true, 失败返回false
        bool GetPhysicalMemoryDescription(IN int index, OUT wstring& description);

        /// @brief 获取物理内存制造商
        /// @param[in] index 内存索引
        /// @param[out] manufacturer 存储制造商
        /// @return 成功返回true, 失败返回false
        bool GetPhysicalMemoryManufacturer(IN int index, OUT wstring& manufacturer);

        /// @brief 获取物理内存序列号
        /// @param[in] index 物理内存索引
        /// @param[out] serialNumber 存储序列号
        /// @return 成功返回true, 失败返回false
        bool GetPhysicalMemorySerialNumber(IN int index, OUT wstring& serialNumber);

        /// @brief 获取物理内存容量(单位M)
        /// @param[in] index 物理内存索引
        /// @param[out] capacity 存储容量
        /// @return 成功返回true, 失败返回false
        bool GetPhysicalMemoryCapacity(IN int index, OUT unsigned long& capacity);

    private:
        LWMICoreManager* m_pWMICoreManager; ///< WMI核心管理者
    };

    /// @brief 视屏控制器(显卡)管理者类
    class LVideoControllerManager
    {
    public:
        LVideoControllerManager();
        ~LVideoControllerManager();

        /// @brief 获取视屏控制器数量
        /// @return 视屏控制器数量
        int GetVideoControllerCount();

        /// @brief 获取视屏控制器描述
        /// @param[in] index 视屏控制器索引
        /// @param[in] description 存储视屏控制器描述
        /// @return 成功返回true, 失败返回false
        bool GetVideoControllerDescription(IN int index, OUT wstring& description);

        /// @brief 获取视屏控制器适配器记忆体容量(显存容量)(单位M)
        /// @param[in] index 视屏控制器索引
        /// @param[in] ramCapacity 记忆体容量
        /// @return 成功返回true, 失败返回false
        bool GetVideoControllerAdapterRAMCapacity(IN int index, OUT unsigned long& ramCapacity);
    private:
        LWMICoreManager* m_pWMICoreManager; ///< WMI核心管理者
    };

}

#endif