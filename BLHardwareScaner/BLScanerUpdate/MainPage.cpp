#include "MainPage.h"

#include <QtCore/QUrl>

#include <QtCore/QFileInfo>

MainPage::MainPage(QWidget *parent, Qt::WFlags flags)
    : QDialog(parent, flags)
{
    ui.setupUi(this);

    connect(&m_ftp, SIGNAL(done(bool)), this, SLOT(FtpDone(bool)));

    QUrl url("ftp://bxu2359840588@bxu2359840588.my3w.com/myfolder/SoftRelease/BLHWScaner/Version");
    QString fileName = QFileInfo(url.path()).fileName();
    m_downloadFile.setFileName(fileName);
    m_downloadFile.open(QIODevice::WriteOnly);

    m_ftp.connectToHost(url.host(), url.port(21));

    m_ftp.login("bxu2359840588", "");
    m_ftp.get(url.path(), &m_downloadFile);

    m_ftp.close();

}

MainPage::~MainPage()
{

}

void MainPage::FtpDone(bool bError)
{
    m_downloadFile.close();
}
