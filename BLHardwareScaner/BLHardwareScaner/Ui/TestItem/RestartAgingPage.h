



#ifndef _RESTARTAGING_PAGE_H_
#define _RESTARTAGING_PAGE_H_

#include <QtGui/QDialog>
#include <QtCore/QTimer>

#include "ui_RestartAging.h"

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
class RestartAgingPage : public QDialog
{
    Q_OBJECT

public:
    /// @brief 构造函数
    RestartAgingPage(IN QWidget *parent = 0, IN Qt::WFlags flags = 0);

    /// @brief 析构函数
    ~RestartAgingPage();

protected:
    /// @brief 窗口显示事件
    void showEvent(QShowEvent* e);

private slots:
    void TestButtonClicked();

private:
    void StartTest();

    /// @brief 检查路径, 路径不存在则创建
    /// @param[in] qstrPath 路径
    /// @return 成功返回true, 失败返回false
    bool CheckPath(IN const QString& qstrPath);

private:
    Ui::RestartAgingForm ui;
};

#endif