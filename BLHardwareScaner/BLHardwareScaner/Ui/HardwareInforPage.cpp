
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
    case COMPUTER_TYPE_DESKTOP:
        strPCSystemInfor += QString::fromStdWString(L"Desktop");
        break;
    case COMPUTER_TYPE_NOTE_BOOK:
        strPCSystemInfor += QString::fromStdWString(L"NoteBook");
        break;
    case COMPUTER_TYPE_TABLET:
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

    // 填写显卡信息
    const DisplayCardInforArray& displayCardInfor = HardwareInfor::GetInstance().GetDisplayCardInfor();
    int mainDisplayCardIndex = 0;
    if (1 == displayCardInfor.Count)
    {
        mainDisplayCardIndex = 0;
    }
    else
    {
        for (int i = 0; i < displayCardInfor.Count; i++)
        {
            if (displayCardInfor.Type[i] == DISPLAY_CARD_EXTERNAL)
            {
                mainDisplayCardIndex = i;
                break;
            }
        }
    }

    QString displayCardName = QString::fromStdWString(displayCardInfor.Description[mainDisplayCardIndex]);
    displayCardName = displayCardName.trimmed();
    if (displayCardInfor.Type[mainDisplayCardIndex] == DISPLAY_CARD_INTERNAL)
        displayCardName += QString::fromStdWString(L"  (集成显卡)");
    else if(displayCardInfor.Type[mainDisplayCardIndex] == DISPLAY_CARD_EXTERNAL)
        displayCardName += QString::fromStdWString(L"  (独立显卡)");
    else
        displayCardName += QString::fromStdWString(L"  (未知显卡)");

    ui.labelDisplayCardInfor->setText(displayCardName);

    // 填写内存信息
    QString memoryInfor;
    const PhysicalMemoryInforArray physicalMemoryInfor = HardwareInfor::GetInstance().GetPhysicalMemoryInfor();
    for (unsigned long i = 0; i < physicalMemoryInfor.Count; i++)
    {
        memoryInfor += QString::fromStdWString(L"内存%1(").arg(i + 1);
        memoryInfor += QString::fromStdWString(L"大小: %1M").arg(physicalMemoryInfor.Capacity[i]);
        memoryInfor += QString::fromStdWString(L" 频率: %1MHz)  ").arg(physicalMemoryInfor.Speed[i]);
    }
    ui.labelMemoryInfor->setText(memoryInfor);

}

HardwareInforPage::~HardwareInforPage()
{

}