
#ifndef _LWMI_SYSTEM_CLASSES_H_
#define _LWMI_SYSTEM_CLASSES_H_


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

    /// @brief 计算机系统管理者类(非操作系统概念上应该包含软硬件)
    class LComputerSystemManager
    {
    public:
        LComputerSystemManager();
        ~LComputerSystemManager();

        /// @brief 获取计算机类型
        enum LPC_SYSTEM_TYPE
        {
            UNKNOWN = 0, ///< 未知
            DESKTOP = 1, ///< 台式机
            NOTE_BOOK = 2, ///< 笔记本
            TABLET = 3 ///< 平板电脑
        };

        /// @brief 获取计算机系统数量
        /// @return 计算机系统数量(应该为1)
        int GetComputerSystemCount();

        /// @brief 获取计算机系统型号
        /// @param[in] index 索引
        /// @param[out] model 存储型号
        /// @return 成功返回true, 失败返回false
        bool GetComputerSystemModel(IN int index, OUT wstring& model);

        /// @brief 获取计算机系统制造商
        /// @param[in] index 索引
        /// @param[out] manufacturer 存储制造商
        /// @return 成功返回true, 失败返回false
        bool GetComputerSystemManufacturer(IN int index, OUT wstring& manufacturer);

        /// @brief 获取计算机类型
        /// @param[in] index 索引
        /// @param[out] type 存储类型
        /// @return 成功返回true, 失败返回false
        bool GetComputerSystemPCType(IN int index, OUT LPC_SYSTEM_TYPE& type);

        /// @brief 获取计算机系统类型
        /// @param[in] index 索引
        /// @param[out] type 存储类型
        /// @return 成功返回true, 失败返回false
        bool GetComputerSystemType(IN int index, OUT wstring& type);

    private:
        LWMICoreManager* m_pWMICoreManager; ///< WMI核心管理者
    };

    /// @brief 操作系统管理者类
    class LOperatingSystemManager
    {
    public:
        LOperatingSystemManager();
        ~LOperatingSystemManager();

        /// @brief 获取操作系统数量
        /// @return 操作系统数量(应该为1)
        int GetOSCount();

        /// @brief 获取操作系统标题
        /// @param[in] index 索引
        /// @param[out] caption 标题
        /// @return 成功返回true, 失败返回false
        bool GetOSCaption(IN int index, OUT wstring& caption);

        /// @brief 获取操作系统架构
        /// @param[in] index 索引
        /// @param[out] architecture 操作系统架构
        /// @return 成功返回true, 失败返回false
        bool GetOSArchitecture(IN int index, OUT wstring& architecture);

        /// @brief 获取操作系统版本
        /// @param[in] index 索引
        /// @param[out] version 版本
        /// @return 成功返回true, 失败返回false
        bool GetOSVersion(IN int index, OUT wstring& version);

        /// @brief 获取操作系统系统盘符
        /// @param[in] index 索引
        /// @param[out] drive 系统盘符
        /// @return 成功返回true, 失败返回false
        bool GetOSSystemDrive(IN int index, OUT wstring& drive);

    private:
        LWMICoreManager* m_pWMICoreManager; ///< WMI核心管理者
    };
}

#endif