

#ifndef _LPDH_H_
#define _LPDH_H_

#include <string>
using std::wstring;


#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

class CPdh;

/// @brief PDH�ӿ���
class LPdh
{
public:
    /// @brief ���캯��
    /// @param[in] counterPath �ռ���·��
    explicit LPdh(IN const wstring& counterPath);

    /// @brief ��������
    ~LPdh();

    /// @brief �ռ���������
    /// �ú����ķ�ָ�����ռ�ʱ��
    /// @param[in] time �ռ�ʱ��, ��λ����
    /// @param[in] value �洢�ռ��������
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool CollectDataLong(IN unsigned long time, OUT long& value);

private:
    CPdh* m_pPdh; ///< PDHʵ�ֶ���
};

#endif