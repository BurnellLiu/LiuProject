
#include "MainPage.h"

#include <QtGui/QPaintDevice>
#include <QtGui/QPainter>
#include <QtGui/QDesktopWidget>
#include <QtCore/QFile>
#include <QtCore/QProcess>

#include "App\\AppParam.h"

#include "HardwareInforPage.h"
#include "TempManagementPage.h"
#include "TestItemPage.h"

#include "..\\Src\\Log\\LLog.h"

#define MAIN_TITLE "BLHWScaner"

MainPage::MainPage(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{   
     ui.setupUi(this);

     this->LoadQSS();

     m_mousePressed = false;

     QObject::connect(ui.toolButtonHardware, SIGNAL(clicked()), this, SLOT(HardwareInforClicked()));
     QObject::connect(ui.toolButtonTempManagement, SIGNAL(clicked()), this, SLOT(TemperatureClicked()));
     QObject::connect(ui.toolButtonTestItem, SIGNAL(clicked()), this, SLOT(TestItemClicked()));

     QObject::connect(ui.pushButtonMin, SIGNAL(clicked()), this, SLOT(MinButtonClicked()));
     QObject::connect(ui.pushButtonClose, SIGNAL(clicked()), this, SLOT(CloseButtonClicked()));
     QObject::connect(ui.pushButtonUpdate, SIGNAL(clicked()), this, SLOT(UpdateButtonClicked()));

     QObject::connect(&m_checkNewTimer, SIGNAL(timeout()), this, SLOT(CheckNewTimerTimeout()));
     QObject::connect(&m_downloadNewTimer, SIGNAL(timeout()), this, SLOT(DownloadNewTimerTimeout()));

     // ����Ĭ�ϴ��ڱ߿�ͱ�����
     this->setWindowFlags(Qt::Window|Qt::FramelessWindowHint|Qt::WindowSystemMenuHint
         |Qt::WindowMinimizeButtonHint|Qt::WindowMaximizeButtonHint);

     this->setWindowIcon(QIcon(":/ControlImage/Main.png"));

     QString title = MAIN_TITLE;
     title += LAppParam::GetAppVersion();
     this->ui.labelTitle->setText(title);

     this->ui.pushButtonUpdate->setVisible(false);

    // ��ȡ��ǰϵͳDPI, ��ǰϵͳDPI�������ʱDPIֵ, ��õ�UI�Ŵ�ϵ��
    const float DESIGN_DPI = 96.0f; // ���ʱDPI
    QPainter painter(this);
    QPaintDevice* pDevice = painter.device();
    PrintLogW(L"System DPI X: %d, Y: %d", pDevice->logicalDpiX(), pDevice->logicalDpiY());
    float ratioX = pDevice->logicalDpiX()/DESIGN_DPI;
    float ratioY = pDevice->logicalDpiY()/DESIGN_DPI;
    m_uiRatio = ratioX > ratioY ? ratioX : ratioY;
    if (m_uiRatio < 1.0f)
        m_uiRatio = 1.0f;
    PrintLogW(L"UI Ratio: %f", m_uiRatio);

    // ���ݱ������µ�����UI��С, ��������ʾ
    int width = this->geometry().width() * m_uiRatio;
    int height = this->geometry().height() * m_uiRatio;
    this->setFixedSize(width, height);
    QDesktopWidget* pDesk = QApplication::desktop();
    this->move((pDesk->width() - width) / 2, (pDesk->height() - height) / 2);

    // ��ʾ��������
    QPixmap originalImage(".\\Image\\Background\\splash.png");
    QSize imageSize(originalImage.width() * m_uiRatio, originalImage.height() * m_uiRatio);
    QPixmap scaledImage = originalImage.scaled(imageSize, Qt::KeepAspectRatio);
    QFont splashFont("Microsoft YaHei UI", 10);
    m_splashScreen.setFont(splashFont);
    m_splashScreen.setPixmap(scaledImage);
    m_splashScreen.show();
    
    width = ui.stackedWidget->width() * m_uiRatio;
    height = ui.stackedWidget->height() * m_uiRatio;

    m_splashScreen.showMessage(QObject::tr("Creating Hardware Page..."), Qt::AlignLeft | Qt::AlignTop, Qt::red);
    m_pHardwareInforPage = new HardwareInforPage(m_uiRatio);
    m_pHardwareInforPage->SetSplashScreen(&m_splashScreen);
    m_pHardwareInforPage->setFixedSize(width, height);
    m_pHardwareInforPage->InitHardwareInfor();
    ui.stackedWidget->addWidget(m_pHardwareInforPage);
    
    m_splashScreen.showMessage(QObject::tr("Creating Temperature Page..."), Qt::AlignLeft | Qt::AlignTop, Qt::red);
    m_pTempManagementPage = new TempManagementPage();
    m_pTempManagementPage->setFixedSize(width, height);
    ui.stackedWidget->addWidget(m_pTempManagementPage);

    m_splashScreen.showMessage(QObject::tr("Creating Test Item Page..."), Qt::AlignLeft | Qt::AlignTop, Qt::red);
    m_pTestItemPage = new TestItemPage(m_uiRatio);
    m_pTestItemPage->setFixedSize(width, height);
    ui.stackedWidget->addWidget(m_pTestItemPage);


    if (APP_NORMAL == LAppParam::GetStartMode())
        ui.stackedWidget->setCurrentWidget(m_pHardwareInforPage);
    else if (APP_RESTARTAGING == LAppParam::GetStartMode())
        ui.stackedWidget->setCurrentWidget(m_pTestItemPage);
    else
        ui.stackedWidget->setCurrentWidget(m_pTestItemPage);
    
}

MainPage::~MainPage()
{
    if (m_pHardwareInforPage != NULL)
    {
        delete m_pHardwareInforPage;
        m_pHardwareInforPage = NULL;
    }

    if (m_pTempManagementPage != NULL)
    {
        delete m_pTempManagementPage;
        m_pTempManagementPage = NULL;
    }

    if (m_pTestItemPage != NULL)
    {
        delete m_pTestItemPage;
        m_pTestItemPage = NULL;
    }
}

void MainPage::showEvent(QShowEvent* e)
{
    PrintLogW((L"Start MainPage::showEvent()"));

    static bool sInitDone = false;
    if (!sInitDone)
    {
        sInitDone = true;

        QSize iconSize(64 * m_uiRatio, 64 * m_uiRatio);
        ui.toolButtonTempManagement->setIconSize(iconSize);
        ui.toolButtonTempManagement->setIcon(QIcon(".\\Image\\Temperature.png"));

        ui.toolButtonHardware->setIconSize(iconSize);
        ui.toolButtonHardware->setIcon(QIcon(".\\Image\\Computer.png"));

        ui.toolButtonTestItem->setIconSize(iconSize);
        ui.toolButtonTestItem->setIcon(QIcon(".\\Image\\TestItem"));

        // �״���ʾ, ��ȡ����
        this->setFocus(Qt::ActiveWindowFocusReason);

        // ����°汾
        m_checkNew.SetCurrentVersion(LAppParam::GetAppVersion());
        m_checkNew.StartCheckAsync();
        m_checkNewTimer.setInterval(1000);
        m_checkNewTimer.start();
    }

    // ������������
    m_splashScreen.finish(this);

    PrintLogW((L"End MainPage::showEvent()"));
}

void MainPage::mousePressEvent(QMouseEvent *e)
{
    m_mousePressed = true;
    m_mouseLastPos = e->globalPos();  
}

void MainPage::mouseMoveEvent(QMouseEvent *e)  
{  
    if (!m_mousePressed)
        return;


    int dx = e->globalX() - m_mouseLastPos.x();  
    int dy = e->globalY() - m_mouseLastPos.y();  
    m_mouseLastPos = e->globalPos();  
    move(this->x() + dx, this->y() + dy);  
}  
void MainPage::mouseReleaseEvent(QMouseEvent *e)  
{  
    m_mousePressed = false; 
}  

void MainPage::HardwareInforClicked()
{
    ui.stackedWidget->setCurrentWidget(m_pHardwareInforPage);
}

void MainPage::TemperatureClicked()
{
    ui.stackedWidget->setCurrentWidget(m_pTempManagementPage);
}

void MainPage::TestItemClicked()
{
    ui.stackedWidget->setCurrentWidget(m_pTestItemPage);
}

void MainPage::MinButtonClicked()
{
    this->showMinimized();
}

void MainPage::CloseButtonClicked()
{
    this->close();
}

void MainPage::UpdateButtonClicked()
{
    if (ui.pushButtonUpdate->text() == "Update")
    {
        // ��ʼ�����°汾
        m_downloadNew.StartDownloadAsync();
        m_downloadNewTimer.setInterval(1000);
        m_downloadNewTimer.start();

        ui.pushButtonUpdate->setEnabled(false);
        ui.pushButtonUpdate->setText("");

        return;
    }

    if (ui.pushButtonUpdate->text() == "Install")
    {
        ui.pushButtonUpdate->setEnabled(false);
        ui.pushButtonUpdate->setText("");
        ui.labelUpdate->setText("Installing... ");

        QProcess::startDetached(".\\Update\\UpdateInstall.exe", QStringList(), ".\\Update\\");

        return;
    }
    
}

void MainPage::CheckNewTimerTimeout()
{
    CHECKNEW_RESULT result = m_checkNew.GetResult();

    if (CHECKNEW_RUNNING == result)
    {
        return;
    }

    m_checkNewTimer.stop();

    
    // ���°汾��Ҫ����
    if (CHECKNEW_WITH_NEW == result)
    {
        ui.pushButtonUpdate->setText("Update");
        ui.pushButtonUpdate->setVisible(true);
        ui.labelUpdate->setText("New Version!! ");
    }

}

void MainPage::DownloadNewTimerTimeout()
{
    int iRet = m_downloadNew.GetResult();

    // ���������°汾
    if (iRet >= 0 && iRet <= 100)
    {
        ui.labelUpdate->setText(QObject::tr("Downloading: %1% ").arg(iRet));
        return;
    }

    // �����°汾����
    if (DOWNLOADNEW_ERROR == iRet)
    {
        ui.labelUpdate->setText("Download Error! ");
        ui.pushButtonUpdate->setText("Update");
        ui.pushButtonUpdate->setEnabled(true);
        m_downloadNewTimer.stop();
        return;
    }

    // �°汾�������
    if (DOWNLOADNEW_DONE == iRet)
    {
        ui.labelUpdate->setText("Download Completed! ");
        ui.pushButtonUpdate->setText("Install");
        ui.pushButtonUpdate->setEnabled(true);
        m_downloadNewTimer.stop();
        return;
    }

    
}

void MainPage::LoadQSS()
{
    QString filePath = LAppParam::GetDefaultQssPath();
    filePath += "MainPage.qss";
    QFile qssFile(filePath);   
    qssFile.open(QFile::ReadOnly);  

    if(qssFile.isOpen())  
    {  
        QString qss = qssFile.readAll();
        qssFile.close();  

        this->setStyleSheet(qss);
    }  
}
