

#ifndef _LDISKCONTROLLER_H_
#define _LDISKCONTROLLER_H_


#include <string>
using std::wstring;


#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

class CIDEDiskController;

/// @brief IDE(ATA)磁盘控制器
///
/// 使用该类需要管理员权限
class LIDEDiskController
{
public:
    /// @brief 构造函数
    /// @param[in] devicePath 设备路径
    /// 设备路径格式为(C语言)"////.//DeviceName"
    /// 设备名称如: PhysicalDrive0, PhysicalDrive1, ...
    LIDEDiskController(IN const wstring& devicePath);
    ~LIDEDiskController();

    /// @brief 判断设备是否存在
    /// @return 存在返回true, 不存在返回false
    bool DeviceExist();

    /// @brief 获取标定转速
    /// @return 标定转速(单位RPM), 发生错误返回0, 返回值为1表示硬盘为SSD
    unsigned long GetRotationRate();

private:
    CIDEDiskController* m_pIDEDiskController; ///< IDE磁盘控制器实现对象
};

#endif