

#ifndef _APP_H_
#define _APP_H_

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

private:
    /// @brief ��ʼ����������ģʽ
    /// @param[in] argc �����в�����Ŀ
    /// @param[in] argv[] �����в���
    static void Init(int argc, char *argv[]);

    static APP_START_MODE sAppRunMode; ///< ��������ģʽ

    friend int main(int argc, char *argv[]); ///< ����main����Ϊ��Ԫ
};

#endif