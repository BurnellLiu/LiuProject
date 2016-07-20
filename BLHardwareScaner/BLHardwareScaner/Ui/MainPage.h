#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QSplashScreen>
#include <QtGui/QMouseEvent>

#include "ui_Main.h"

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
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private slots:
    void HardwareInforClicked();
    void TemperatureClicked();
    void TestItemClicked();

    void MinButtonClicked();
    void CloseButtonClicked();


private:
    /// @brief ����UI��ʽ
    void LoadQSS();

private:
    Ui::MainForm ui;
    QSplashScreen m_splashScreen; ///< ��������

    QPoint m_mouseLastPos; ///< �������λ��

    HardwareInforPage* m_pHardwareInforPage;
    TempManagementPage* m_pTempManagementPage;
    TestItemPage* m_pTestItemPage;

    float m_uiRatio; ///< UI��Ҫ�����ı���
};

#endif // MAINWINDOW_H
