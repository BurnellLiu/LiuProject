
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

class CDiskSequenceTest;
class CDisk4KRandomTest;

/// @brief 磁盘测试状态
struct LDiskTestState 
{
    bool TestDone; ///< 标记测试是否已经结束
    bool Error; ///< 标记是否存在错误
    float ReadSpeed; ///< 读速度, 单位M/S
    float WriteSpeed; ///< 写速度, 单位M/S
};

/// @brief 磁盘测试接口
class IDiskTest
{
public:
    /// @brief 析构函数
    virtual ~IDiskTest() = 0 {};

    /// @brief 开始测试
    /// 该方法为异步方法, 开始测试后使用GetState可以获得测试结果
    /// 注意: 针对系统盘目录需要管理员权限
    /// @param[in] filePath 测试文件完整路径
    /// @return 成功返回true, 失败返回false, 测试正在进行或者打开文件失败则测试会失败
    virtual bool Start(IN const wstring& filePath) = 0;

    /// @brief 获取测试状态
    /// @return 测试状态
    virtual LDiskTestState GetState() = 0;
};



/// @brief 磁盘顺序测试
class LDiskSequenceTest : public IDiskTest
{
public:
    /// @brief 构造函数
    LDiskSequenceTest();

    /// @brief 析构函数
    ~LDiskSequenceTest();

    /// @brief 开始测试
    bool Start(IN const wstring& filePath);

    /// @brief 获取测试状态
    LDiskTestState GetState();

private:
    CDiskSequenceTest* m_pDiskSequenceTest; ///< 顺序测试实现对象
};



/// @brief 磁盘4KB随机测试
class LDisk4KRandomTest : public IDiskTest
{
public:
    /// @brief 构造函数
    LDisk4KRandomTest();

    /// @brief 析构函数
    ~LDisk4KRandomTest();

    /// @brief 开始测试
    bool Start(IN const wstring& filePath);

    /// @brief 获取测试状态
    LDiskTestState GetState();

private:
    CDisk4KRandomTest* m_pDisk4KRandomTest; ///< 4K随机测试实现对象
};

#endif