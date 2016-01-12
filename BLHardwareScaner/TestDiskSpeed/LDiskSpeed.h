
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

/// @brief ���̲���״̬
struct LDiskTestState 
{
    bool TestDone; ///< ��ǲ����Ƿ��Ѿ�����
    bool Error; ///< ����Ƿ���ڴ���
    float ReadSpeed; ///< ���ٶ�, ��λM/S
    float WriteSpeed; ///< д�ٶ�, ��λM/S
};

/// @brief ���̲��Խӿ�
class IDiskTest
{
public:
    /// @brief ��������
    virtual ~IDiskTest() = 0 {};

    /// @brief ��ʼ����
    /// �÷���Ϊ�첽����, ��ʼ���Ժ�ʹ��GetState���Ի�ò��Խ��
    /// ע��: ���ϵͳ��Ŀ¼��Ҫ����ԱȨ��
    /// @param[in] filePath �����ļ�����·��
    /// @return �ɹ�����true, ʧ�ܷ���false, �������ڽ��л��ߴ��ļ�ʧ������Ի�ʧ��
    virtual bool Start(IN const wstring& filePath) = 0;

    /// @brief ��ȡ����״̬
    /// @return ����״̬
    virtual LDiskTestState GetState() = 0;
};



/// @brief ����˳�����
class LDiskSequenceTest : public IDiskTest
{
public:
    /// @brief ���캯��
    LDiskSequenceTest();

    /// @brief ��������
    ~LDiskSequenceTest();

    /// @brief ��ʼ����
    bool Start(IN const wstring& filePath);

    /// @brief ��ȡ����״̬
    LDiskTestState GetState();

private:
    CDiskSequenceTest* m_pDiskSequenceTest; ///< ˳�����ʵ�ֶ���
};



/// @brief ����4KB�������
class LDisk4KRandomTest : public IDiskTest
{
public:
    /// @brief ���캯��
    LDisk4KRandomTest();

    /// @brief ��������
    ~LDisk4KRandomTest();

    /// @brief ��ʼ����
    bool Start(IN const wstring& filePath);

    /// @brief ��ȡ����״̬
    LDiskTestState GetState();

private:
    CDisk4KRandomTest* m_pDisk4KRandomTest; ///< 4K�������ʵ�ֶ���
};

#endif