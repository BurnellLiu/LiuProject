
#include "HardwareInforPage.h"

#include <QtCore/QString>
#include "..\\Src\\HardwareInfor.h"

HardwareInforPage::HardwareInforPage(QWidget *parent, Qt::WFlags flags)
    : QWidget(parent, flags)
{
    ui.setupUi(this);

    // 填充计算机系统信息
    const ComputerSystemInfor& computerSystemInfor = HardwareInfor::GetInstance().GetComputerSystemInfor();

    QString strManufacturer = QString::fromStdWString(computerSystemInfor.Manufacturer);
    strManufacturer = strManufacturer.trimmed();
    strManufacturer = strManufacturer.toUpper();
    if (strManufacturer.indexOf("ASUSTEK") != -1)
    {
        strManufacturer = "ASUS";
    }
    QString strPCSystemInfor = strManufacturer;
    strPCSystemInfor += QString::fromStdWString(L"  ");

    QString modelName = QString::fromStdWString(computerSystemInfor.ModelName);
    modelName = modelName.trimmed();
    
    
    strPCSystemInfor += modelName;
    strPCSystemInfor += QString::fromStdWString(L"  ");
    switch (computerSystemInfor.Type)
    {
    case DESKTOP:
        strPCSystemInfor += QString::fromStdWString(L"Desktop");
        break;
    case NOTE_BOOK:
        strPCSystemInfor += QString::fromStdWString(L"NoteBook");
        break;
    case TABLET:
        strPCSystemInfor += QString::fromStdWString(L"Tablet");
        break;
    default:
        break;
    }
    ui.labelComputerSystemInfor->setText(strPCSystemInfor);

    // 填充操作系统信息
    const OperatingSystemInfor& operatingSystemInfor = HardwareInfor::GetInstance().GetOperatingSystemInfor();
    
    QString systemCaption = QString::fromStdWString(operatingSystemInfor.Caption);
    systemCaption = systemCaption.trimmed();
    QString systemArchitecture = QString::fromStdWString(operatingSystemInfor.Architecture);
    systemArchitecture = systemArchitecture.trimmed();

    QString strOperatingSystemInfor;
    strOperatingSystemInfor += systemCaption;
    strOperatingSystemInfor += "  ";
    strOperatingSystemInfor += systemArchitecture;

    ui.labelOperatingSystemInfor->setText(strOperatingSystemInfor);

    // 填写主板信息
    const BaseBoardInfor& baseBoardInfor = HardwareInfor::GetInstance().GetBaseBoardInfor();
    QString baseBoardDesc = QString::fromStdWString(baseBoardInfor.Description);
    baseBoardDesc = baseBoardDesc.trimmed();
    QString baseBoardManufacturer = QString::fromStdWString(baseBoardInfor.Manufacturer);
    baseBoardManufacturer = baseBoardManufacturer.trimmed();
    QString strBaseBoardInfor = baseBoardManufacturer;
    strBaseBoardInfor += "  ";
    strBaseBoardInfor += baseBoardDesc;

    ui.labelBaseBoardInfor->setText(strBaseBoardInfor);

    // 填写处理器信息
    const ProcessorInfor& processorInfor = HardwareInfor::GetInstance().GetProcessorInfor();
    QString processorName = QString::fromStdWString(processorInfor.Name);
    processorName = processorName.trimmed();
    ui.labelProcessorInfor->setText(processorName);
}

HardwareInforPage::~HardwareInforPage()
{

}