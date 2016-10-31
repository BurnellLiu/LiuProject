#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QSplashScreen>
#include <QtGui/QMouseEvent>
#include <QtCore/QTimer>

#include "ui_Main.h"

#include "Update/NewVersionUpdate.h"

class HardwareInforPage;
class TempManagementPage;
class TestItemPage;

class MainPage : public QMainWindow
{
    Q_OBJECT

public:
    MainPage(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~MainPage();

protected:
    /// @brief ������ʾ�¼�
    void showEvent(QShowEvent* e);

    /*
    ȥ��ϵͳ�߿�ͱ��������³��򴰿��޷��ƶ�
    �ֶ��������������¼���ʵ�ִ����ƶ�
    */
    /// @brief ��갴���¼�
    void mousePressEvent(QMouseEvent *e);

    /// @brief ����ƶ��¼�, ֻ����괦�ڰ���״̬ʱ�Żᴥ�����¼�
    void mouseMoveEvent(QMouseEvent *e);

    /// @brief ����ͷ��¼�
    void mouseReleaseEvent(QMouseEvent *e);

private slots:
    void HardwareInforClicked();
    void TemperatureClicked();
    void TestItemClicked();

    void MinButtonClicked();
    void CloseButtonClicked();
    void UpdateButtonClicked();

    /// @brief ����°汾��ʱ����ʱ�ص�����
    void CheckNewTimerTimeout();

    /// @brief �����°汾��ʱ����ʱ�ص�����
    void DownloadNewTimerTimeout();

private:
    /// @brief ����UI��ʽ
    void LoadQSS();

private:
    Ui::MainForm ui;
    QSplashScreen m_splashScreen; ///< ��������

    bool m_mousePressed; ///< ��־����Ƿ񱻰���
    QPoint m_mouseLastPos; ///< �������λ��

    HardwareInforPage* m_pHardwareInforPage;
    TempManagementPage* m_pTempManagementPage;
    TestItemPage* m_pTestItemPage;

    float m_uiRatio; ///< UI��Ҫ�����ı���

    CheckNew m_checkNew; ///< ����°汾
    QTimer m_checkNewTimer; ///< ����°汾��ʱ��

    DownloadNew m_downloadNew; ///< �����°汾
    QTimer m_downloadNewTimer; ///< �����°汾��ʱ��
};

#endif // MAINWINDOW_H
