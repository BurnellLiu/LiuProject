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

    /// @brief 获取UI调整比例
    /// @return 比例
    float GetUiResizeRatio();

protected:
    /// @brief 窗口显示事件
    void showEvent(QShowEvent* e);

private slots:
    void HardwareInforClicked();
    void TemperatureClicked();
    void TestItemClicked();


private:
    /// @brief 加载UI样式
    void LoadQSS();

private:
    Ui::MainForm ui;

    HardwareInforPage* m_pHardwareInforPage;
    TempManagementPage* m_pTempManagementPage;
    TestItemPage* m_pTestItemPage;

    float m_uiRatio; ///< UI需要调整的比例
};

#endif // MAINWINDOW_H
