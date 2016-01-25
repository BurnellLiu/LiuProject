#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_MainWindow.h"

class HardwareInforPage;
class TempManagementPage;
class TestItemPage;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~MainWindow();

protected:
    void showEvent(QShowEvent* e);

private slots:
    void HardwareInforClicked();
    void TemperatureClicked();
    void TestItemClicked();


private:
    Ui::MainWindowForm ui;

    HardwareInforPage* m_pHardwareInforPage;
    TempManagementPage* m_pTempManagementPage;
    TestItemPage* m_pTestItemPage;

    float m_uiRatio; ///< UI需要调整的比例
};

#endif // MAINWINDOW_H
