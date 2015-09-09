
#include "HardwareInforPage.h"

#include <QtCore/QString>
#include "..\\Src\\HardwareInfor.h"

/// @brief 转换制造商名称
///  
/// 如ASUSTeK COMPUTER INC变成ASUS
/// @param[in] manufacturer 原始制造商名称
/// @return 新的制造商名称
static QString ConvertManufacturer(IN const QString& manufacturer)
{
    QString oldStr = manufacturer;
    oldStr = oldStr.toUpper();
    QString newStr = oldStr;

    if (oldStr.indexOf("ASUSTEK") != -1)
    {
        newStr = "ASUS";
        goto EXIT;
    }

    if (oldStr.indexOf("INTEL") != -1)
    {
        newStr = "Intel";
        goto EXIT;
    }

EXIT:
    return newStr;
}

HardwareInforPage::HardwareInforPage(QWidget *parent, Qt::WFlags flags)
    : QWidget(parent, flags)
{
    ui.setupUi(this);

    ui.listWidgetHWItem->addItem(tr("Computer"));
    HWItemInfor* pComputerItem = new ComputerItemInfor();
    m_hwItemVec.push_back(pComputerItem);

    ui.listWidgetHWItem->addItem(tr("Processor"));
    HWItemInfor* pProcessorItem = new ProcessorItemInfor;
    m_hwItemVec.push_back(pProcessorItem);

    ui.listWidgetHWItem->addItem(tr("MotherBoard"));
    ui.listWidgetHWItem->addItem(tr("Memory"));
    ui.listWidgetHWItem->addItem(tr("Disk"));
    ui.listWidgetHWItem->addItem(tr("VideoCard"));
    ui.listWidgetHWItem->addItem(tr("Monitor"));

    for (int i = 0; i < m_hwItemVec.size(); i++)
    {
        m_hwItemVec[i]->LoadHWInfor();
    }


    QObject::connect(ui.listWidgetHWItem, SIGNAL(itemSelectionChanged()), this, SLOT(CurrentItemChanged()));
    ui.listWidgetHWItem->setCurrentRow(0);

}

HardwareInforPage::~HardwareInforPage()
{
    for (int i = 0; i < m_hwItemVec.size(); i++)
    {
        delete m_hwItemVec[i];
        m_hwItemVec[i] = 0;
    }
}

void HardwareInforPage::CurrentItemChanged()
{
    int currentRow = ui.listWidgetHWItem->currentRow();
    if (currentRow >= m_hwItemVec.size())
    {
        ui.labelHWTitle->setText("");
        ui.textEditHWIinfor->setText("");
        return;
    }

    ui.labelHWTitle->setText(m_hwItemVec[currentRow]->GetTitle());
    ui.textEditHWIinfor->setText(m_hwItemVec[currentRow]->GetContent());
}

HWItemInfor::HWItemInfor()
{

}

HWItemInfor::~HWItemInfor()
{

}

const QString& HWItemInfor::GetTitle() const
{
    return m_title;
}

const QString& HWItemInfor::GetContent() const
{
    return m_content;
}

void HWItemInfor::SetTitle(IN const QString& title)
{
    m_title = title;
}

void HWItemInfor::ContentAddItem(IN const QString& subTitle, IN const QString text)
{
    if (subTitle.isEmpty())
        return;

    QString formatSubTitle = subTitle;

    formatSubTitle += "\t";

    m_content += formatSubTitle;
    
    QString formatText = text.trimmed();
    m_content += formatText;
    m_content += "\n\n";
}

void HWItemInfor::ContendAddBlankLine()
{
    m_content += "\n\n";
}

void HWItemInfor::ClearInfor()
{
    m_title.clear();
    m_content.clear();
}

ComputerItemInfor::ComputerItemInfor()
{

}

ComputerItemInfor::~ComputerItemInfor()
{

}

void ComputerItemInfor::LoadHWInfor()
{
    this->ClearInfor();


     // 获取计算机系统信息
    
    const ComputerSystemInfor& computerSystemInfor = HardwareInfor::GetInstance().GetComputerSystemInfor();
    QString strManufacturer = QString::fromStdWString(computerSystemInfor.Manufacturer);
    QString strPCSystemInfor = ConvertManufacturer(strManufacturer);
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

    this->SetTitle(strPCSystemInfor);
    this->ContentAddItem(QObject::tr("Computer Model"), strPCSystemInfor);

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
    this->ContentAddItem(QObject::tr("Operating System"), strOperatingSystemInfor);
    this->ContendAddBlankLine();

    // 填写主板信息
    const BaseBoardInfor& baseBoardInfor = HardwareInfor::GetInstance().GetBaseBoardInfor();
    QString baseBoardDesc = QString::fromStdWString(baseBoardInfor.Description);
    baseBoardDesc = baseBoardDesc.trimmed();
    QString baseBoardManufacturer = QString::fromStdWString(baseBoardInfor.Manufacturer);
    QString strBaseBoardInfor = ConvertManufacturer(baseBoardManufacturer);
    strBaseBoardInfor += "  ";
    strBaseBoardInfor += baseBoardDesc;
    this->ContentAddItem(QObject::tr("Mother Board"), strBaseBoardInfor);

    // 填写处理器信息
    const ProcessorInfor& processorInfor = HardwareInfor::GetInstance().GetProcessorInfor();
    QString processorName = QString::fromStdWString(processorInfor.Name);
    processorName = processorName.trimmed();
    processorName += QString::fromStdWString(L" ( Cores: %1 Logical Processor: %2 )").arg(processorInfor.CoresNumber).arg(processorInfor.LogicalProcessorNumber);
    this->ContentAddItem(QObject::tr("Processor\t"), processorName);

    // 填写显卡信息
    const DisplayCardInforArray& displayCardInfor = HardwareInfor::GetInstance().GetDisplayCardInfor();
    for (int i = 0; i < displayCardInfor.Count; i++)
    {
        QString displayCardName = QString::fromStdWString(displayCardInfor.Description[i]);
        displayCardName = displayCardName.trimmed();
        if (displayCardInfor.Type[i] == DISPLAY_CARD_INTERNAL)
            displayCardName += QString::fromStdWString(L"  (Integrated)");
        else if(displayCardInfor.Type[i] == DISPLAY_CARD_EXTERNAL)
            displayCardName += QString::fromStdWString(L"  (Independent)");
        else
            displayCardName += QString::fromStdWString(L"  (Unknown)");

        this->ContentAddItem(QObject::tr("Video Card\t"), displayCardName);
    }

    // 填写内存信息
    const PhysicalMemoryInforArray& physicalMemoryInfor = HardwareInfor::GetInstance().GetPhysicalMemoryInfor();
    for (unsigned long i = 0; i < physicalMemoryInfor.Count; i++)
    {
        QString memoryInfor;
        memoryInfor += QString::fromStdWString(L"Size: %1M").arg(physicalMemoryInfor.Capacity[i]);
        memoryInfor += QString::fromStdWString(L" Speed: %1MHz").arg(physicalMemoryInfor.Speed[i]);
        this->ContentAddItem(QObject::tr("Memory\t"), memoryInfor);
    }
    
    // 填写磁盘信息
    const DiskInforArray& diskInforArray = HardwareInfor::GetInstance().GetDiskInfor();
    
    for (unsigned long i = 0; i < diskInforArray.Count; i++)
    {
        if (diskInforArray.DiskType[i] != FIXED_IDE_DISK)
            continue;

        QString diskInfor;
        QString diskName = QString::fromStdWString(diskInforArray.Model[i]);
        diskName = diskName.trimmed();
        diskInfor += diskName;
        diskInfor += QString::fromStdWString(L" ( %1 G )").arg(diskInforArray.TotalSize[i]);

        this->ContentAddItem(QObject::tr("Disk\t"), diskInfor);
    }

    // 填写显示器信息
    const MonitorInforArray& monitorInforArray = HardwareInfor::GetInstance().GetMonitorInfor();
    for (unsigned long i = 0; i < monitorInforArray.Count; i++)
    {
        QString monitorInfor;
        monitorInfor += QString::fromStdWString(L"%1  ").arg(monitorInforArray.Name[i].c_str());
        this->ContentAddItem(QObject::tr("Monitor\t"), monitorInfor);
    }
}

ProcessorItemInfor::ProcessorItemInfor()
{

}

ProcessorItemInfor::~ProcessorItemInfor()
{

}

void ProcessorItemInfor::LoadHWInfor()
{
    this->ClearInfor();

    const ProcessorInfor& processprInfor = HardwareInfor::GetInstance().GetProcessorInfor();

    QString name = QString::fromStdWString(processprInfor.Name);
    this->SetTitle(name);
    this->ContentAddItem(QObject::tr("Name\t"), name);

    QString description = QString::fromStdWString(processprInfor.Description);
    this->ContentAddItem(QObject::tr("Description\t"), description);

    QString manufacturer = QString::fromStdWString(processprInfor.Manufacturer);
    this->ContentAddItem(QObject::tr("Manufacturer"), ConvertManufacturer(manufacturer));

    QString coresNumber = QString::fromStdString("Cores: %1    Logical Processors: %2").arg(processprInfor.CoresNumber).arg(processprInfor.LogicalProcessorNumber);
    this->ContentAddItem(QObject::tr("Cores Number"), coresNumber);

    QString speed = QString::fromStdString("%1MHz").arg(processprInfor.MaxClockSpeed);
    this->ContentAddItem(QObject::tr("Speed\t"), speed);

}