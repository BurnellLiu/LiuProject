



#ifndef _RESTARTAGING_PAGE_H_
#define _RESTARTAGING_PAGE_H_

#include <QtGui/QDialog>
#include <QtCore/QTimer>
#include <QTimer>

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

/// @brief �ظ��������Բ���
struct RestartAgingParam
{
    int IntervalTime; ///< ���ʱ��
    int AgingTimes; ///< ���Դ���
    int CompletedTimes; ///< ����ɲ��Դ���
    int BSODTimes; ///< ���������Ĵ���
    QString LastRestartTime; ///< �����ؿ���ʱ��, ��ʽ"yyyy-MM-dd hh:mm:ss"
};

/// @brief �ظ���������ҳ��
class RestartAgingPage : public QDialog
{
    Q_OBJECT

public:
    /// @brief ���캯��
    RestartAgingPage(IN QWidget *parent = 0, IN Qt::WFlags flags = 0);

    /// @brief ��������
    ~RestartAgingPage();

protected:
    /// @brief ������ʾ�¼�
    void showEvent(QShowEvent* e);

private slots:
    void TestButtonClicked();
    void ResetButtonClicked();

    /// @brief ����ʱ��ʱ����ʱ�ص�����
    void CountDownTimerTimeout();

private:
    /// @brief ע������ƻ�
    void RegisterTaskScheduler();

    /// @brief ɾ������ƻ�
    void DeleteTaskScheduler();

    /// @brief ������Բ������ļ���
    void SaveTestParamToFile(IN const RestartAgingParam& param);

    /// @brief ���ļ��л�ȡ���Բ���
    void GetTestParamFromFile(OUT RestartAgingParam& param);

    /// @brief �ж��Ƿ�������
    /// @param[in] lastRestartTime �ϴιػ�ʱ��
    /// @return ������������true, δ������������false
    bool IsBSODHappened(IN const QString& lastRestartTime);

    /// @brief ���·��, ·���������򴴽�
    /// @param[in] qstrPath ·��
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool CheckPath(IN const QString& qstrPath);

    /// @brief ����UI��ʽ
    void LoadQSS();

private:
    Ui::RestartAgingForm ui;
    int m_currentCountDown; ///< ��ǰ����ʱ
    QTimer m_countDownTimer; ///< ����ʱ��ʱ��
};

#endif