

#ifndef _DISKSPEED_PAGE_H_
#define _DISKSPEED_PAGE_H_

#include <QtGui/QDialog>
#include <QtCore/QVector>
#include <QtCore/QTimer>

#include "ui_DiskSpeed.h"
#include "..\\Src\\LDiskSpeed.h"

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
    /// @brief ���캯��
    DiskSpeedPage(IN QWidget *parent = 0, IN Qt::WFlags flags = 0);


    /// @brief ��������
    ~DiskSpeedPage();

    /// @brief ����UI��������
    void SetUIRatio(IN float ratio);

protected:
    /// @brief ������ʾ�¼�
    void showEvent(QShowEvent* e);

private:

    /// @brief ��ʼ��ҳ��
    void Init();

    /// @brief ������Դ
    void Clear();

private slots:
    /// @brief ���԰�ť�����ۺ���
    void TestButtonClicked();

    /// @brief ���Լ���ʱ����ʱ�ص�����
    void TestMonitorTimerTimeout();

private:
    Ui::DiskSpeedForm ui;
    float m_uiRatio; ///< UI��Ҫ�����ı���

    QTimer m_testMonitorTimer; ///< ���Լ�ⶨʱ��

    unsigned int m_currentRuningIndex; ///< ��ǰ����������
    QVector<IDiskSpeedTest*> m_testItemList; ///< �������б�

};

#endif