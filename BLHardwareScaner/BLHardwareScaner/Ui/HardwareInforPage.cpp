
#include "HardwareInforPage.h"

#include <QtCore/QString>
#include "..\\Src\\HardwareInfor.h"

HardwareInforPage::HardwareInforPage(QWidget *parent, Qt::WFlags flags)
    : QWidget(parent, flags)
{
    ui.setupUi(this);

    const ComputerSystemInfor& computerSystemInfor = HardwareInfor::GetInstance().GetComputerSystemInfor();
    QString strPCSystemInfor = QString::fromStdWString(computerSystemInfor.Manufacturer);
    switch (computerSystemInfor.Type)
    {
    case DESKTOP:
        strPCSystemInfor += QString::fromStdWString(L"  Desktop");
        break;
    case NOTE_BOOK:
        strPCSystemInfor += QString::fromStdWString(L"  NoteBook");
        break;
    case TABLET:
        strPCSystemInfor += QString::fromStdWString(L"  Tablet");
        break;
    default:
        break;
    }
    ui.labelComputerSystemInfor->setText(strPCSystemInfor);
}

HardwareInforPage::~HardwareInforPage()
{

}