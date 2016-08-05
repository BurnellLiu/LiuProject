



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

/// @brief �����ٶȲ���ҳ��
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

private:
    void StartTest();

    /// @brief ���·��, ·���������򴴽�
    /// @param[in] qstrPath ·��
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool CheckPath(IN const QString& qstrPath);

private:
    Ui::RestartAgingForm ui;
};

#endif