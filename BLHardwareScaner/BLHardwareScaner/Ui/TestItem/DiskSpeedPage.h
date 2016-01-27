

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

/// @brief 磁盘速度测试页面
class DiskSpeedPage : public QDialog
{
    Q_OBJECT

public:
    /// @brief 构造函数
    DiskSpeedPage(IN QWidget *parent = 0, IN Qt::WFlags flags = 0);


    /// @brief 析构函数
    ~DiskSpeedPage();

    /// @brief 设置UI调整比例
    void SetUIRatio(IN float ratio);

protected:
    /// @brief 窗口显示事件
    void showEvent(QShowEvent* e);

private:

    /// @brief 初始化页面
    void Init();

    /// @brief 清理资源
    void Clear();

private slots:
    /// @brief 测试按钮单击槽函数
    void TestButtonClicked();

    /// @brief 测试监测计时器超时回调函数
    void TestMonitorTimerTimeout();

private:
    Ui::DiskSpeedForm ui;
    float m_uiRatio; ///< UI需要调整的比例

    QTimer m_testMonitorTimer; ///< 测试监测定时器

    unsigned int m_currentRuningIndex; ///< 当前测试项索引
    QVector<IDiskSpeedTest*> m_testItemList; ///< 测试项列表

};

#endif