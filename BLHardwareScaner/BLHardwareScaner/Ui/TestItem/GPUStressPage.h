

#ifndef _GPUSTRESS_PAGE_H_
#define _GPUSTRESS_PAGE_H_

#include <QtGui/QDialog>
#include <QtCore/QVector>
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

private:

private:
    Ui::GPUStressForm ui;
};

#endif