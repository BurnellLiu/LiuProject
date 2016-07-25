#include "MainPage.h"

#include <QtCore/QDir>


#define FTP_USER "bxu2359840588"
#define FTP_PWD ""

#define CHECK_VERSION_URL "ftp://bxu2359840588@bxu2359840588.my3w.com/myfolder/SoftRelease/BLHWScaner/Version"
#define UPDATE_FILE_URL   "ftp://bxu2359840588@bxu2359840588.my3w.com/myfolder/SoftRelease/BLHWScaner/BLHWScaner.rar"

#define UPDATE_PATH ".\\Temp\\Update\\"

#define WITH_ERROR 0
#define WITH_NEW_VERSION 1
#define WITHOUT_NEW_VERSION 2


MainPage::MainPage(int mode, QWidget *parent, Qt::WFlags flags)
    : QDialog(parent, flags)
{
    ui.setupUi(this);

    ui.progressBar->setRange(0, 100);

    m_mode = mode;

    m_pDownloadProgressTimer = new QTimer();
    m_pDownloadProgressTimer->setInterval(1000);
    QObject::connect(m_pDownloadProgressTimer, SIGNAL(timeout()), this, SLOT(DownloadProgressTimerTimeout()));

    if (AP_CHECK == m_mode)
        m_pFtpDownload = new LFtpDownload(CHECK_VERSION_URL);
    else if (AP_UPDATE == m_mode)
        m_pFtpDownload = new LFtpDownload(UPDATE_FILE_URL);

    m_pFtpDownload->SetLogin(FTP_USER, FTP_PWD);
    m_pFtpDownload->SetFilePath(UPDATE_PATH);

    this->CheckPath(UPDATE_PATH);
}

MainPage::~MainPage()
{
    if (NULL != m_pFtpDownload)
    {
        delete m_pFtpDownload;
        m_pFtpDownload = NULL;
    }

    if (m_pDownloadProgressTimer != NULL)
    {
        delete m_pDownloadProgressTimer;
        m_pDownloadProgressTimer = NULL;
    }
}

void MainPage::showEvent(QShowEvent* e)
{
    m_pFtpDownload->StartDownloadAsync();

    m_pDownloadProgressTimer->start();
}

void MainPage::DownloadProgressTimerTimeout()
{
    const DownloadState& state = m_pFtpDownload->GetDownloadState();

    if (state.TotalSize != 0)
    {
        qint64 percent = state.DoneSize * 100/state.TotalSize;

        ui.progressBar->setValue(percent);
    }

    if (state.IsDone)
    {
        m_pDownloadProgressTimer->stop();   
    }

    if (state.IsDone && state.IsError)
    {
        this->done(WITH_ERROR);
    }
}

bool MainPage::CheckPath(IN const QString& qstrPath)
{
    QDir logDir;
    if (!logDir.exists(qstrPath))
    {
        return logDir.mkpath(qstrPath);
    }

    return true;
}
