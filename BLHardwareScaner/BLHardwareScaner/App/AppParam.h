

#ifndef _APP_H_
#define _APP_H_

#include <QtCore/QString>

/// @brief ��������ģʽ
enum APP_START_MODE
{
    APP_NORMAL = 0, ///< ��׼����ģʽ
    APP_RESTARTAGING ///< �ؿ���Aging����ģʽ
};

/// @brief ���������
class LAppParam
{
public:
    /// @brief ��ȡ��������ģʽ
    /// @return ��������ģʽ
    static APP_START_MODE GetStartMode();

    /// @brief ��ȡ����ǰ�汾
    /// @return �汾��Ϣ, ��V1.3.6
    static QString GetAppVersion();

private:
    /// @brief ��ʼ���������
    /// @param[in] argc �����в�����Ŀ
    /// @param[in] argv[] �����в���
    static void InitAppParam(int argc, char *argv[]);
    friend int main(int argc, char *argv[]); ///< ����main����Ϊ��Ԫ
};

#endif