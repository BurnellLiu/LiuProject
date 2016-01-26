

#ifndef _DISKSPEED_PAGE_H_
#define _DISKSPEED_PAGE_H_

#include <QtGui/QDialog>

#include "ui_DiskSpeed.h"

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

/// @brief �����ٶȲ���ҳ��
class DiskSpeedPage : public QDialog
{
    Q_OBJECT

public:
    DiskSpeedPage(IN QWidget *parent = 0, IN Qt::WFlags flags = 0);
    ~DiskSpeedPage();

    /// @brief ����UI��������
    void SetUIRatio(IN float ratio);

protected:
    /// @brief ������ʾ�¼�
    void showEvent(QShowEvent* e);

private:

    /// @brief ��ʼ��������ҳ��
    void Init();

private:
    Ui::DiskSpeedForm ui;

    float m_uiRatio; ///< UI��Ҫ�����ı���
    bool m_bInitDone; ///< ����Ƿ��ʼ�����
};

#endif