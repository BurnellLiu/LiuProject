

#ifndef _LAPPINSTANCE_H_
#define _LAPPINSTANCE_H_

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


class CAppInstance;

/// @brief Ӧ�ó���ʵ����
/// �������ڼ��Ӧ�ó����Ƿ��Ѿ�����һ��ʵ��������, ͨ��ʵ��������ʶ��
/// �뱣֤��������������, ����������Ӧ�ó������������
class LAppInstance
{
public:
    /// @brief ���캯��
    /// @param[in] name ʵ������
    explicit LAppInstance(IN const wstring& name);

    /// @brief ��������
    ~LAppInstance();

    /// @brief �ж��Ƿ��Ѿ����ڸ�ʵ�� 
    /// @return true(�Ѿ�����), false(��ʵ��)
    bool IsAlreadyExist();

private:
    CAppInstance* m_pAppInstance; ///< Ӧ�ó���ʵ��ʵ�ֶ���
};


#endif