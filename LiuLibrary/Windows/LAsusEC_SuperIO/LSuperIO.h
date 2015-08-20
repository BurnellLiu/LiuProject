

#ifndef _LSUPERIO_H_
#define _LSUPERIO_H_

#include <Windows.h>

class LSuperIO
{
public:
    LSuperIO(WORD indexPort, WORD dataPort);
    ~LSuperIO();

    /// @brief 对索引端口进行写
    void WriteIndexPortByte(BYTE data);

    /// @brief 对数据端口进行读
    /// @brief 读取的数据
    BYTE ReadDataPortByte();

    /// @brief 对数据端口进行写
    /// @param[in] data 写入的数据
    void WriteDataPortByte(BYTE data);

private:
    WORD INDEX_PORT; ///<  索引端口
    WORD DATA_PORT; ///< 数据端口
};


class LWinbondIO : public LSuperIO
{
public:
    LWinbondIO();
    ~LWinbondIO();
};

class LLogicalDevice
{
public:
    enum MyEnum
    {

    };
    LLogicalDevice();
    ~LLogicalDevice();

    WORD GetDeviceIndex();
private:
    LSuperIO* m_pSuperIO;
};



#endif