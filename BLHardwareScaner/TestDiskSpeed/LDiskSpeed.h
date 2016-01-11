
#ifndef _LDISKSPEED_H_
#define _LDISKSPEED_H_

#include <string>
using std::wstring;


#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

/// @brief 速度测试状态
struct LSpeedTestState 
{
    bool TestDone; ///< 标记测试是否已经结束
    bool Error; ///< 标记是否存在错误
    float ReadSpeed; ///< 读速度, 单位M/S
    float WriteSpeed; ///< 写速度, 单位M/S
};

class CDiskSequenceTest;

/// @brief 磁盘顺序测试
class LDiskSequenceTest
{
public:

    /// @brief 构造函数
    LDiskSequenceTest();

    /// @brief 析构函数
    ~LDiskSequenceTest();

    /// @brief 开始测试
    /// 该方法为异步方法, 开始测试后使用GetState可以获得测试结果
    /// @param[in] filePath 测试文件完整路径
    /// @return 成功返回true, 失败返回false, 测试正在进行或者打开文件失败则测试会失败
    bool Start(IN const wstring& filePath);

    /// @brief 获取测试状态
    /// @return 测试状态
    LSpeedTestState GetState();

private:
    CDiskSequenceTest* m_pDiskSequenceTest; ///< 顺序测试实现对象
};

#endif