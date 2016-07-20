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
    /// @brief 窗口显示事件
    void showEvent(QShowEvent* e);

    /*
    去掉系统边框和标题栏后导致程序窗口无法移动
    手动处理以下三个事件来实现窗口移动
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
    /// @brief 加载UI样式
    void LoadQSS();

private:
    Ui::MainForm ui;
    QSplashScreen m_splashScreen; ///< 启动画面

    QPoint m_mouseLastPos; ///< 最新鼠标位置

    HardwareInforPage* m_pHardwareInforPage;
    TempManagementPage* m_pTempManagementPage;
    TestItemPage* m_pTestItemPage;

    float m_uiRatio; ///< UI需要调整的比例
};

#endif // MAINWINDOW_H
