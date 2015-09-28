#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_MainWindow.h"

class HardwareInforPage;
class TempManagementPage;

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


private:
    Ui::MainWindowForm ui;

    HardwareInforPage* m_pHardwareInforPage;
    TempManagementPage* m_pTempManagementPage;

    float m_uiRatio; ///< UI��Ҫ�����ı���
};

#endif // MAINWINDOW_H
