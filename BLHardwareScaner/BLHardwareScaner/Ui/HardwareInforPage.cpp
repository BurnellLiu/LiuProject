
#include "HardwareInforPage.h"

#include <QtCore/QString>
#include "..\\Src\\HardwareInfor.h"

HardwareInforPage::HardwareInforPage(QWidget *parent, Qt::WFlags flags)
    : QWidget(parent, flags)
{
    ui.setupUi(this);

    // �������ϵͳ��Ϣ
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

    // ������ϵͳ��Ϣ
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

    // ��д������Ϣ
    const BaseBoardInfor& baseBoardInfor = HardwareInfor::GetInstance().GetBaseBoardInfor();
    QString baseBoardDesc = QString::fromStdWString(baseBoardInfor.Description);
    baseBoardDesc = baseBoardDesc.trimmed();
    QString baseBoardManufacturer = QString::fromStdWString(baseBoardInfor.Manufacturer);
    baseBoardManufacturer = baseBoardManufacturer.trimmed();
    QString strBaseBoardInfor = baseBoardManufacturer;
    strBaseBoardInfor += "  ";
    strBaseBoardInfor += baseBoardDesc;

    ui.labelBaseBoardInfor->setText(strBaseBoardInfor);

    // ��д��������Ϣ
    const ProcessorInfor& processorInfor = HardwareInfor::GetInstance().GetProcessorInfor();
    QString processorName = QString::fromStdWString(processorInfor.Name);
    processorName = processorName.trimmed();
    ui.labelProcessorInfor->setText(processorName);

    // ��д�Կ���Ϣ
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
        displayCardName += QString::fromStdWString(L"  (�����Կ�)");
    else if(displayCardInfor.Type[mainDisplayCardIndex] == DISPLAY_CARD_EXTERNAL)
        displayCardName += QString::fromStdWString(L"  (�����Կ�)");
    else
        displayCardName += QString::fromStdWString(L"  (δ֪�Կ�)");

    ui.labelDisplayCardInfor->setText(displayCardName);

    // ��д�ڴ���Ϣ
    QString memoryInfor;
    const PhysicalMemoryInforArray physicalMemoryInfor = HardwareInfor::GetInstance().GetPhysicalMemoryInfor();
    for (unsigned long i = 0; i < physicalMemoryInfor.Count; i++)
    {
        memoryInfor += QString::fromStdWString(L"�ڴ�%1(").arg(i + 1);
        memoryInfor += QString::fromStdWString(L"��С: %1M").arg(physicalMemoryInfor.Capacity[i]);
        memoryInfor += QString::fromStdWString(L" Ƶ��: %1MHz)  ").arg(physicalMemoryInfor.Speed[i]);
    }
    ui.labelMemoryInfor->setText(memoryInfor);

}

HardwareInforPage::~HardwareInforPage()
{

}