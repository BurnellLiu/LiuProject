
#include "MainPage.h"

#include <QtGui/QPaintDevice>
#include <QtGui/QPainter>
#include <QtGui/QDesktopWidget>
#include <QtCore/QFile>

#include "HardwareInforPage.h"
#include "TempManagementPage.h"
#include "TestItemPage.h"

#include "..\\Src\\Log\\LLog.h"

#define MAIN_TITLE "BLHWScaner-V1.1.2"

MainPage::MainPage(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{   
     ui.setupUi(this);

     this->LoadQSS();

     QObject::connect(ui.toolButtonHardware, SIGNAL(clicked()), this, SLOT(HardwareInforClicked()));
     QObject::connect(ui.toolButtonTempManagement, SIGNAL(clicked()), this, SLOT(TemperatureClicked()));
     QObject::connect(ui.toolButtonTestItem, SIGNAL(clicked()), this, SLOT(TestItemClicked()));

     QObject::connect(ui.pushButtonMin, SIGNAL(clicked()), this, SLOT(MinButtonClicked()));
     QObject::connect(ui.pushButtonClose, SIGNAL(clicked()), this, SLOT(CloseButtonClicked()));

     // 隐藏默认窗口边框和标题栏
     this->setWindowFlags(Qt::Window|Qt::FramelessWindowHint|Qt::WindowSystemMenuHint
         |Qt::WindowMinimizeButtonHint|Qt::WindowMaximizeButtonHint);

     this->setWindowIcon(QIcon(":/ControlImage/Main.png"));

     this->ui.labelTitle->setText(MAIN_TITLE);
     this->ui.pushButtonUpdate->setVisible(false);

    // 获取当前系统DPI, 当前系统DPI除以设计时DPI值, 则得到UI放大系数
    const float DESIGN_DPI = 96.0f; // 设计时DPI
    QPainter painter(this);
    QPaintDevice* pDevice = painter.device();
    PrintLogW(L"System DPI X: %d, Y: %d", pDevice->logicalDpiX(), pDevice->logicalDpiY());
    float ratioX = pDevice->logicalDpiX()/DESIGN_DPI;
    float ratioY = pDevice->logicalDpiY()/DESIGN_DPI;
    m_uiRatio = ratioX > ratioY ? ratioX : ratioY;
    if (m_uiRatio < 1.0f)
        m_uiRatio = 1.0f;
    PrintLogW(L"UI Ratio: %f", m_uiRatio);

    // 根据比例重新调整主UI大小, 并居中显示
    int width = this->geometry().width() * m_uiRatio;
    int height = this->geometry().height() * m_uiRatio;
    this->setFixedSize(width, height);
    QDesktopWidget* pDesk = QApplication::desktop();
    this->move((pDesk->width() - width) / 2, (pDesk->height() - height) / 2);

    // 显示启动画面
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
    ui.stackedWidget->addWidget(m_pHardwareInforPage);
    
    m_splashScreen.showMessage(QObject::tr("Creating Temperature Page..."), Qt::AlignLeft | Qt::AlignTop, Qt::red);
    m_pTempManagementPage = new TempManagementPage();
    m_pTempManagementPage->setFixedSize(width, height);
    ui.stackedWidget->addWidget(m_pTempManagementPage);

    m_splashScreen.showMessage(QObject::tr("Creating Test Item Page..."), Qt::AlignLeft | Qt::AlignTop, Qt::red);
    m_pTestItemPage = new TestItemPage(m_uiRatio);
    m_pTestItemPage->setFixedSize(width, height);
    ui.stackedWidget->addWidget(m_pTestItemPage);

    ui.stackedWidget->setCurrentWidget(m_pHardwareInforPage);
    
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
    }

    // 结束启动画面
    m_splashScreen.finish(this);

    PrintLogW((L"End MainPage::showEvent()"));
}

void MainPage::mousePressEvent(QMouseEvent *e)
{
    m_mouseLastPos = e->globalPos();  
}

void MainPage::mouseMoveEvent(QMouseEvent *e)  
{  
    int dx = e->globalX() - m_mouseLastPos.x();  
    int dy = e->globalY() - m_mouseLastPos.y();  
    m_mouseLastPos = e->globalPos();  
    move(this->x() + dx, this->y() + dy);  
}  
void MainPage::mouseReleaseEvent(QMouseEvent *e)  
{  
    int dx = e->globalX() - m_mouseLastPos.x();  
    int dy = e->globalY() - m_mouseLastPos.y();  
    move(this->x() + dx, this->y() + dy);  
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

void MainPage::LoadQSS()
{
    QFile qssFile(".\\Qss\\Default\\MainPage.qss");  
    qssFile.open(QFile::ReadOnly);  

    if(qssFile.isOpen())  
    {  
        QString qss = qssFile.readAll();
        qssFile.close();  

        this->setStyleSheet(qss);
    }  
}
