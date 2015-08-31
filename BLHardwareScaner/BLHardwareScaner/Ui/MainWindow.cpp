
#include "MainWindow.h"
#include "UiDefines.h"

#include "HardwareInforPage.h"

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
    
    ui.setupUi(this);
    RESIZE_UI;
    
    HardwareInforPage* pHardwareInforPage = new HardwareInforPage();
    int width = ui.stackedWidget->width() * GetUIResizeRatio();
    int height = ui.stackedWidget->height() * GetUIResizeRatio();
    pHardwareInforPage->setFixedSize(width, height);
    int iRet = ui.stackedWidget->addWidget(pHardwareInforPage);
    ui.stackedWidget->setCurrentIndex(2);
}

MainWindow::~MainWindow()
{

}
