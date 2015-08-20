#ifndef _LEC_H_
#define _LEC_H_

#include <Windows.h>

/// @brief ��������ڳ�ʼ��WinRing0
///
/// �ڱ��ļ��и���ʵ���ѱ�����
class LWinRing0
{
public:
    LWinRing0();
    ~LWinRing0();
public:
    /// @brief �ж�WinRing0�Ƿ��ʼ���ɹ�
    /// @return ��ʼ���ɹ�true, ��ʼ��ʧ�ܷ���false 
    static bool IsReady()
    {
        return sm_bInitSuccess;
    }
private:
    static bool sm_bInitSuccess; ///< ��ʼ��WinRing0�ɹ�
};

/// @brief EC ����
class LEc
{
public:
    LEc();
    virtual ~LEc();

    /// @brief ������˿ڽ���д
    /// @param[in] command ����
    void WriteCommandPortByte(BYTE command);

    /// @brief ������˿ڽ��ж�
    /// @brief ��ȡ������
    BYTE ReadCommandPortByte();

    /// @brief �����ݶ˿ڽ���д
    /// @param[in] data д�������
    void WriteDataPortByte(BYTE data);

    /// @brief �����ݶ˿ڽ��ж�
    /// @return ��ȡ������
    BYTE ReadDataPortByte();

    /// @brief �ж����뻺����״̬
    /// @return true ���뻺����׼������, false ���뻺����δ����
    bool IBFReady();

    /// @brief �ж����������״̬
    /// @return true ���������׼������, false ���������δ����
    bool OBFReady();

protected:
    WORD EC_SC; ///<  ���˶˿�, ��Ӧ״̬�Ĵ���; д����ʱ��Ӧ����Ĵ���
    WORD EC_DATA; ///< ��Ӧ���ݼĴ���
};

/// @brief Asus ENE EC
class LAsusEneEc : public LEc
{
public:
    LAsusEneEc();
    virtual ~LAsusEneEc();
};

/// @brief Asus ITE EC
class LAsusIteEc : public LEc
{
public:
    LAsusIteEc();
    virtual ~LAsusIteEc();
};





#endif