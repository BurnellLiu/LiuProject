
#include "MainWindow.h"
#include "UiDefines.h"

#include "HardwareInforPage.h"
#include "TempManagementPage.h"

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
    
    ui.setupUi(this);
    RESIZE_UI;

    QObject::connect(ui.toolButtonHardware, SIGNAL(clicked()), this, SLOT(HardwareInforClicked()));
    QObject::connect(ui.toolButtonTempManagement, SIGNAL(clicked()), this, SLOT(TemperatureClicked()));
    
    int width = ui.stackedWidget->width() * GetUIResizeRatio();
    int height = ui.stackedWidget->height() * GetUIResizeRatio();

    m_pHardwareInforPage = new HardwareInforPage();
    m_pHardwareInforPage->setFixedSize(width, height);
    ui.stackedWidget->addWidget(m_pHardwareInforPage);
    

    m_pTempManagementPage = new TempManagementPage();
    m_pTempManagementPage->setFixedSize(width, height);
    ui.stackedWidget->addWidget(m_pTempManagementPage);

}

MainWindow::~MainWindow()
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
}

void MainWindow::showEvent(QShowEvent* e)
{
    ui.stackedWidget->setCurrentWidget(m_pHardwareInforPage);
}

void MainWindow::HardwareInforClicked()
{
    ui.stackedWidget->setCurrentWidget(m_pHardwareInforPage);
}

void MainWindow::TemperatureClicked()
{
    ui.stackedWidget->setCurrentWidget(m_pTempManagementPage);
}
