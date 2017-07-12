

#ifndef _GPUSTRESS_PAGE_H_
#define _GPUSTRESS_PAGE_H_

#include <QtGui/QDialog>
#include <QtCore/QVector>
#include <QtCore/QTimer>
#include <QtCore/QProcess>
#include <QtCore/QTimer>

#include "ui_GPUStress.h"


#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

/// @brief GPU压力测试页面
class GPUStressPage : public QDialog
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param[in] uiRatio UI放大系数
    GPUStressPage(IN float uiRatio, IN QWidget *parent = 0, IN Qt::WFlags flags = 0);

    /// @brief 析构函数
    ~GPUStressPage();

protected:
    /// @brief 窗口显示事件
    void showEvent(QShowEvent* e);

private slots:
    /// @brief 测试按钮单击槽函数
    void TestButtonClicked();

    /// @brief GPU压力测试定时器超时函数
    void GpuStressTimerTimeout();

private:
    Ui::GPUStressForm ui;
    QProcess m_gpuProcess; ///< GPU压力测试进程
    QTimer m_gpuTimer; ///< GPU压力测试进程计时器
};

#endif