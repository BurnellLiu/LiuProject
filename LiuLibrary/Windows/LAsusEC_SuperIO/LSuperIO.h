

#ifndef _LSUPERIO_H_
#define _LSUPERIO_H_

#include <Windows.h>

class LSuperIO
{
public:
    LSuperIO(WORD indexPort, WORD dataPort);
    ~LSuperIO();

    /// @brief �������˿ڽ���д
    void WriteIndexPortByte(BYTE data);

    /// @brief �����ݶ˿ڽ��ж�
    /// @brief ��ȡ������
    BYTE ReadDataPortByte();

    /// @brief �����ݶ˿ڽ���д
    /// @param[in] data д�������
    void WriteDataPortByte(BYTE data);

private:
    WORD INDEX_PORT; ///<  �����˿�
    WORD DATA_PORT; ///< ���ݶ˿�
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