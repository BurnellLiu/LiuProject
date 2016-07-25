#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QtGui/QDialog>
#include <QtCore/QFile>
#include <QTimer>

#include "ui_Main.h"

#include "FtpDownload.h"

// ��������ģʽ: ����Ƿ����°汾ģʽ
#define AP_CHECK 1

// ��������ģʽ: �����°汾ģʽ
#define AP_UPDATE 2

class MainPage : public QDialog
{
    Q_OBJECT

public:
    MainPage(int mode, QWidget *parent = 0, Qt::WFlags flags = 0);
    ~MainPage();

protected:
    void showEvent(QShowEvent* e);

private slots:
    /// @brief ���ؽ��ȼ�ʱ����ʱ�ص�����
    void DownloadProgressTimerTimeout();

private:
    /// @brief ���·��, ·���������򴴽�
    /// @param[in] qstrPath ·��
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool CheckPath(IN const QString& qstrPath);

private:
    int m_mode; ///< ��־��������ģʽ
    Ui::MainForm ui;

    QTimer* m_pDownloadProgressTimer; ///< ���ؽ��ȶ�ʱ��

    LFtpDownload* m_pFtpDownload; ///< FTP���ض���
};

#endif // MAINPAGE_H
