#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
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

    /// @brief ��ȡUI��������
    /// @return ����
    float GetUiResizeRatio();

protected:
    /// @brief ������ʾ�¼�
    void showEvent(QShowEvent* e);

private slots:
    void HardwareInforClicked();
    void TemperatureClicked();
    void TestItemClicked();


private:
    /// @brief ����UI��ʽ
    void LoadQSS();

private:
    Ui::MainForm ui;

    HardwareInforPage* m_pHardwareInforPage;
    TempManagementPage* m_pTempManagementPage;
    TestItemPage* m_pTestItemPage;

    float m_uiRatio; ///< UI��Ҫ�����ı���
};

#endif // MAINWINDOW_H
