#include "MainWindow.h"

#include "HardwareInforPage.h"

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
    ui.setupUi(this);

    HardwareInforPage* pHardwareInforPage = new HardwareInforPage();
    int iRet = ui.stackedWidget->addWidget(pHardwareInforPage);
    ui.stackedWidget->setCurrentIndex(2);
}

MainWindow::~MainWindow()
{

}
