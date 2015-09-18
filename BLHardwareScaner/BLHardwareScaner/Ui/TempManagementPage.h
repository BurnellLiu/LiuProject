
#ifndef _TEMPMANAGEMENTPAGE_H_
#define _TEMPMANAGEMENTPAGE_H_

#include "ui_TempManagement.h"

#include <QVector>
#include <QTimer>
#include <QThread>

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

/// @brief 扫描温度线程类
class ScanTempThread : public QThread
{
    Q_OBJECT
public:
    ScanTempThread();
    ~ScanTempThread();

    /// @brief 停止线程
    void Stop();

protected:
    virtual void run();

private:
    bool m_bStopThread; ///< 标记是否停止线程

};


class TempManagementPage : public QWidget
{
    Q_OBJECT

public:
    TempManagementPage(IN QWidget *parent = 0, IN Qt::WFlags flags = 0);
    ~TempManagementPage();

protected:
    void showEvent(QShowEvent* e);
    void hideEvent(QHideEvent* e);

private slots:
    void TempRefreshTimerTimeout();

private:
    void RefreshTemperature();

private:
    Ui::TempManagementForm ui;
    QTimer* m_pTempRefreshTimer;

    QVector<QLabel*> m_tempLabelList;
    ScanTempThread m_scanTempThread;
};





#endif