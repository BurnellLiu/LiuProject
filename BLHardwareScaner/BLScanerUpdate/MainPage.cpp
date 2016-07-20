#include "MainPage.h"

MainPage::MainPage(QWidget *parent, Qt::WFlags flags)
    : QDialog(parent, flags)
{
    ui.setupUi(this);

    connect(&m_ftp, SIGNAL(done(bool)), this, SLOT(FtpDone(bool)));

    QUrl url()
}

MainPage::~MainPage()
{

}
