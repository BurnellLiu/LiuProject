
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

/// @brief �ٶȲ���״̬
struct LSpeedTestState 
{
    bool TestDone; ///< ��ǲ����Ƿ��Ѿ�����
    bool Error; ///< ����Ƿ���ڴ���
    float ReadSpeed; ///< ���ٶ�, ��λM/S
    float WriteSpeed; ///< д�ٶ�, ��λM/S
};

class CDiskSequenceTest;

/// @brief ����˳�����
class LDiskSequenceTest
{
public:

    /// @brief ���캯��
    LDiskSequenceTest();

    /// @brief ��������
    ~LDiskSequenceTest();

    /// @brief ��ʼ����
    /// �÷���Ϊ�첽����, ��ʼ���Ժ�ʹ��GetState���Ի�ò��Խ��
    /// @param[in] filePath �����ļ�����·��
    /// @return �ɹ�����true, ʧ�ܷ���false, �������ڽ��л��ߴ��ļ�ʧ������Ի�ʧ��
    bool Start(IN const wstring& filePath);

    /// @brief ��ȡ����״̬
    /// @return ����״̬
    LSpeedTestState GetState();

private:
    CDiskSequenceTest* m_pDiskSequenceTest; ///< ˳�����ʵ�ֶ���
};

#endif