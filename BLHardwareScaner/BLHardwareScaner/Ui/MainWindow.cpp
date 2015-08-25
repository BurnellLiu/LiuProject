#include "MainWindow.h"

#include "HardwareInformation.h"

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
    ui.setupUi(this);

    HardwareInformation* pHardwareInformation = new HardwareInformation();
    int iRet = ui.stackedWidget->addWidget(pHardwareInformation);
    ui.stackedWidget->setCurrentIndex(2);
}

MainWindow::~MainWindow()
{

}
