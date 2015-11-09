
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

/// @brief ɨ���¶��߳���
class ScanTempThread : public QThread
{
    Q_OBJECT
public:
    ScanTempThread();
    ~ScanTempThread();

    /// @brief ֹͣ�߳�
    void Stop();

protected:
    virtual void run();

private:
    bool m_bStopThread; ///< ����Ƿ�ֹͣ�߳�

};

/// @brief ɨ�������߳���
class ScanPerformanceThread : public QThread
{
    Q_OBJECT
public:
    ScanPerformanceThread();
    ~ScanPerformanceThread();

    /// @brief ֹͣ�߳�
    void Stop();

protected:
    virtual void run();

private:
    bool m_bStopThread; ///< ����Ƿ�ֹͣ�߳�

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
    /// @brief UIˢ�¼�������ʱ�ص�����
    void UiRefreshTimerTimeout();

private:
    /// @brief ˢ��UI
    void RefreshUi();

private:
    Ui::TempManagementForm ui;
    QTimer* m_pUiRefreshTimer; ///< ˢ��UI��ʱ��

    ScanTempThread m_scanTempThread; ///< ɨ���¶��߳�
    ScanPerformanceThread m_scanPerformanceThred; ///< ɨ���������߳�
    
    bool m_bExternalVideoCardExist; ///< ��ʶ�Ƿ���ڶ����Կ�
};





#endif