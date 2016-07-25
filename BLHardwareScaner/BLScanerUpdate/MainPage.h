#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QtGui/QDialog>
#include <QtCore/QFile>
#include <QTimer>

#include "ui_Main.h"

#include "FtpDownload.h"

// 程序运行模式: 检查是否有新版本模式
#define AP_CHECK 1

// 程序运行模式: 更新新版本模式
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
    /// @brief 下载进度计时器超时回调函数
    void DownloadProgressTimerTimeout();

private:
    /// @brief 检查路径, 路径不存在则创建
    /// @param[in] qstrPath 路径
    /// @return 成功返回true, 失败返回false
    bool CheckPath(IN const QString& qstrPath);

private:
    int m_mode; ///< 标志程序运行模式
    Ui::MainForm ui;

    QTimer* m_pDownloadProgressTimer; ///< 下载进度定时器

    LFtpDownload* m_pFtpDownload; ///< FTP下载对象
};

#endif // MAINPAGE_H
