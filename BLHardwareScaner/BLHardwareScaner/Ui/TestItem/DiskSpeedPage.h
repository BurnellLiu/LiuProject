

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

/// @brief 磁盘速度测试页面
class DiskSpeedPage : public QDialog
{
    Q_OBJECT

public:
    DiskSpeedPage(IN QWidget *parent = 0, IN Qt::WFlags flags = 0);
    ~DiskSpeedPage();

    /// @brief 设置UI调整比例
    void SetUIRatio(IN float ratio);

protected:
    /// @brief 窗口显示事件
    void showEvent(QShowEvent* e);

private:

    /// @brief 初始化测试项页面
    void Init();

private:
    Ui::DiskSpeedForm ui;

    float m_uiRatio; ///< UI需要调整的比例
    bool m_bInitDone; ///< 标记是否初始化完成
};

#endif