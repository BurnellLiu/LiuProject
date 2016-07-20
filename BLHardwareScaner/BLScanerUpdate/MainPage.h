#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QtGui/QDialog>
#include <QtNetwork/QFtp>

#include "ui_Main.h"

class MainPage : public QDialog
{
    Q_OBJECT

public:
    MainPage(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~MainPage();

private slots:
    void FtpDone(bool bError);

private:
    Ui::MainForm ui;

    QFtp m_ftp;
};

#endif // MAINPAGE_H
