/// @file LDiskSpeed.h
/// @brief ��ͷ�ļ��������˴����ٶȲ����� 
/// 
/// Detail:
/// @author Burnell Liu Email:burnell_liu@outlook.com
/// @version   
/// @date 2016:1:13

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

/// @brief �����ٶȲ��Դ�����
enum LDiskSpeedTestError
{
    DST_NO_ERROR = 0, ///< û�д���
    DST_OPEN_FILE_ERROR, ///< ���ļ�����
    DST_WRITE_FILE_ERROR, ///< д�ļ�����
    DST_READ_FILE_ERROR, ///< ���ļ�����
    DST_ALLOC_MEMORY_ERROR, ///< �����ڴ����
    DST_TEST_ABORT_ERROR ///< �����жϴ���
};

/// @brief �����ٶȲ���״̬
struct LDiskSpeedTestState 
{
    bool TestDone; ///< ��ǲ����Ƿ��Ѿ�����
    float ReadSpeed; ///< ���ٶ�, ��λM/S
    float WriteSpeed; ///< д�ٶ�, ��λM/S
    LDiskSpeedTestError Error; ///< ������
    wstring ErrorMsg; ///< ���Դ�����Ϣ
    wstring ErrorMsgWindows; ///< Windows������Ϣ
};

/// @brief �����ٶȲ��Խӿ�
class IDiskSpeedTest
{
public:
    /// @brief ��������
    virtual ~IDiskSpeedTest() = 0 {};

    /// @brief ��ʼ����
    /// �÷���Ϊ�첽����, ��ʼ���Ժ�ʹ��GetState���Ի�ò��Խ��
    /// ע��: ���ϵͳ��Ŀ¼��Ҫ����ԱȨ��
    /// @param[in] filePath �����ļ�����·��
    /// @return �ɹ�����true, ʧ�ܷ���false, �������ڽ������ʧ��
    virtual bool Start(IN const wstring& filePath) = 0;

    /// @brief ��ȡ����״̬
    /// @return ����״̬
    virtual LDiskSpeedTestState GetState() = 0;

    /// @brief ֹͣ����
    virtual void Stop() = 0;
};



/// @brief ����˳�����
/// ����˳�����Ҫ�����������2G��ʣ��ռ�
class LDiskSequenceTest : public IDiskSpeedTest
{
public:
    /// @brief ���캯��
    LDiskSequenceTest();

    /// @brief ��������
    ~LDiskSequenceTest();

    /// @brief ��ʼ����
    bool Start(IN const wstring& filePath);

    /// @brief ��ȡ����״̬
    LDiskSpeedTestState GetState();

    /// @brief ֹͣ����
    void Stop();

private:
    CDiskSequenceTest* m_pDiskSequenceTest; ///< ˳�����ʵ�ֶ���
};



/// @brief ����4KB�������
/// ����4K�������Ҫ�����������1G��ʣ��ռ�
class LDisk4KRandomTest : public IDiskSpeedTest
{
public:
    /// @brief ���캯��
    LDisk4KRandomTest();

    /// @brief ��������
    ~LDisk4KRandomTest();

    /// @brief ��ʼ����
    bool Start(IN const wstring& filePath);

    /// @brief ��ȡ����״̬
    LDiskSpeedTestState GetState();

    /// @brief ֹͣ����
    void Stop();

private:
    CDisk4KRandomTest* m_pDisk4KRandomTest; ///< 4K�������ʵ�ֶ���
};

#endif