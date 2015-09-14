
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
    QString oldStr = manufacturer.toUpper();
    QString newStr = manufacturer;

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

    if (oldStr.indexOf("QUALCOMM") != -1)
    {
        newStr = "Qualcomn";
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

    ui.listWidgetHWItem->addItem(tr("MotherBoard"));
    HWItemInfor* pMotherBoardItem = new MotherBoardItemInfor();
    m_hwItemVec.push_back(pMotherBoardItem);

    ui.listWidgetHWItem->addItem(tr("Processor"));
    HWItemInfor* pProcessorItem = new ProcessorItemInfor();
    m_hwItemVec.push_back(pProcessorItem);

    ui.listWidgetHWItem->addItem(tr("VideoCard"));
    HWItemInfor* pVideoCardItem = new VideoCardItemInfor();
    m_hwItemVec.push_back(pVideoCardItem);

    ui.listWidgetHWItem->addItem(tr("Memory"));
    HWItemInfor* pMemoryItem = new MemoryItemInfor();
    m_hwItemVec.push_back(pMemoryItem);


    ui.listWidgetHWItem->addItem(tr("Disk"));
    HWItemInfor* pDiskItem = new DiskItemInfor();
    m_hwItemVec.push_back(pDiskItem);

    const NetworkCardInforArray& networkCardInforArray = HardwareInfor::GetInstance().GetNetworkCardInfor();
    if (networkCardInforArray.Count >= 1)
    {
        ui.listWidgetHWItem->addItem(tr("Network Card"));
        HWItemInfor* pNetworkCardItem = new NetworkCardItemInfor();
        m_hwItemVec.push_back(pNetworkCardItem);
    }

    ui.listWidgetHWItem->addItem(tr("Monitor"));
    HWItemInfor* pMonitorItem = new MonitorItemInfor();
    m_hwItemVec.push_back(pMonitorItem);

    const BatteryStaticInfor& batteryStaticInfor = HardwareInfor::GetInstance().GetBatteryStaticInfor();
    if (batteryStaticInfor.Exist)
    {
        ui.listWidgetHWItem->addItem(tr("Battery"));
        HWItemInfor* pBatteryItem = new BatteryItemInfor();
        m_hwItemVec.push_back(pBatteryItem);
    }

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
        ui.textEditHWInfor->setText("");
        return;
    }

    ui.labelHWTitle->setText(m_hwItemVec[currentRow]->GetTitle());
    ui.textEditHWInfor->setText(m_hwItemVec[currentRow]->GetContent());
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

    QString formatSubTitle = subTitle.trimmed();

    while (formatSubTitle.length() < 20)
    {
        formatSubTitle += " ";
    }

    m_content += formatSubTitle;
    
    QString formatText = text.trimmed();
    m_content += formatText;
    m_content += "\n\n";
}

void HWItemInfor::ContentAddBlankLine()
{
    m_content += "\n\n";
}

void HWItemInfor::ClearInfor()
{
    m_title.clear();
    m_content.clear();
}

void ComputerItemInfor::LoadHWInfor()
{
    this->ClearInfor();

    this->SetTitle(QObject::tr("Computer"));


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
    this->ContentAddBlankLine();

    // 填写主板信息
    const MotherBoardInfor& motherBoardInfor = HardwareInfor::GetInstance().GetMotherBoardInfor();
    QString motherBoardProductName = QString::fromStdWString(motherBoardInfor.ProductName).trimmed();
    QString motherBoardManufacturer = QString::fromStdWString(motherBoardInfor.Manufacturer);
    motherBoardManufacturer = ConvertManufacturer(motherBoardManufacturer);
    QString motherBoardDesc = motherBoardManufacturer;
    motherBoardDesc += "  ";
    motherBoardDesc += motherBoardProductName;
    this->ContentAddItem(QObject::tr("Mother Board"), motherBoardDesc);

    // 填写处理器信息
    const ProcessorInfor& processorInfor = HardwareInfor::GetInstance().GetProcessorInfor();
    QString processorName = QString::fromStdWString(processorInfor.Name);
    processorName = processorName.trimmed();
    processorName += QString::fromStdWString(L"(Cores: %1 Threads: %2)").arg(processorInfor.CoresNumber).arg(processorInfor.LogicalProcessorNumber);
    this->ContentAddItem(QObject::tr("Processor"), processorName);

    // 填写显卡信息
    const VideoCardInforArray& videoCardInfor = HardwareInfor::GetInstance().GetVideoCardInfor();
    for (int i = 0; i < videoCardInfor.Count; i++)
    {
        QString displayCardName = QString::fromStdWString(videoCardInfor.Description[i]);
        displayCardName = displayCardName.trimmed();
        if (videoCardInfor.Type[i] == DISPLAY_CARD_INTERNAL)
            displayCardName += QString::fromStdWString(L"  (Integrated)");
        else if(videoCardInfor.Type[i] == DISPLAY_CARD_EXTERNAL)
            displayCardName += QString::fromStdWString(L"  (Independent)");
        else
            displayCardName += QString::fromStdWString(L"  (Unknown)");

        this->ContentAddItem(QObject::tr("Video Card"), displayCardName);
    }

    // 填写内存信息
    const PhysicalMemoryInforArray& physicalMemoryInfor = HardwareInfor::GetInstance().GetPhysicalMemoryInfor();
    for (unsigned long i = 0; i < physicalMemoryInfor.Count; i++)
    {
        QString memoryInfor;
        memoryInfor += QString::fromStdWString(L"Size: %1M").arg(physicalMemoryInfor.Capacity[i]);
        memoryInfor += QString::fromStdWString(L" Speed: %1MHz").arg(physicalMemoryInfor.Speed[i]);
        this->ContentAddItem(QObject::tr("Memory"), memoryInfor);
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

        this->ContentAddItem(QObject::tr("Disk"), diskInfor);
    }

    // 获取网卡信息
    const NetworkCardInforArray& networkCardInforArray = HardwareInfor::GetInstance().GetNetworkCardInfor();
    for (unsigned long i = 0; i < networkCardInforArray.Count; i++)
    {
        QString name = QString::fromStdWString(networkCardInforArray.Name[i]);
        this->ContentAddItem(QObject::tr("Network Card"), name);
    }

    // 填写显示器信息
    const MonitorInforArray& monitorInforArray = HardwareInfor::GetInstance().GetMonitorInfor();
    for (unsigned long i = 0; i < monitorInforArray.Count; i++)
    {
        QString monitorInfor;
        monitorInfor += QString::fromStdWString(monitorInforArray.Name[i]);
        this->ContentAddItem(QObject::tr("Monitor"), monitorInfor);
    }

    const BatteryStaticInfor& batteryStaticInfor = HardwareInfor::GetInstance().GetBatteryStaticInfor();
    if (batteryStaticInfor.Exist)
    {
        QString batteryManufacturer = QString::fromStdWString(batteryStaticInfor.Manufacturer);
        batteryManufacturer = ConvertManufacturer(batteryManufacturer);

        QString batteryName = QString::fromStdWString(batteryStaticInfor.Name);

        QString batteryInfor = batteryManufacturer;
        batteryInfor += "  ";
        batteryInfor += batteryName;

        this->ContentAddItem(QObject::tr("Battery"), batteryInfor);
    }
}

void ProcessorItemInfor::LoadHWInfor()
{
    this->ClearInfor();

    this->SetTitle(QObject::tr("Processor"));

    const ProcessorInfor& processprInfor = HardwareInfor::GetInstance().GetProcessorInfor();

    QString name = QString::fromStdWString(processprInfor.Name);
    this->ContentAddItem(QObject::tr("Name"), name);

    QString description = QString::fromStdWString(processprInfor.Description);
    this->ContentAddItem(QObject::tr("Description"), description);

    QString manufacturer = QString::fromStdWString(processprInfor.Manufacturer);
    this->ContentAddItem(QObject::tr("Manufacturer"), ConvertManufacturer(manufacturer));

    QString coresNumber = QString::fromStdString("Cores: %1    Logical Processors: %2").arg(processprInfor.CoresNumber).arg(processprInfor.LogicalProcessorNumber);
    this->ContentAddItem(QObject::tr("Cores Number"), coresNumber);

    QString speed = QString::fromStdString("%1MHz").arg(processprInfor.MaxClockSpeed);
    this->ContentAddItem(QObject::tr("Speed"), speed);

}

void MotherBoardItemInfor::LoadHWInfor()
{
    this->ClearInfor();

    this->SetTitle(QObject::tr("Mother Board"));

    const MotherBoardInfor& motherBoardInfor = HardwareInfor::GetInstance().GetMotherBoardInfor();
    QString boardProductName = QString::fromStdWString(motherBoardInfor.ProductName).trimmed();
    this->ContentAddItem(QObject::tr("Product Name"), boardProductName);

    QString boardManufacturer = QString::fromStdWString(motherBoardInfor.Manufacturer);
    boardManufacturer = ConvertManufacturer(boardManufacturer);
    this->ContentAddItem(QObject::tr("Manufacturer"), boardManufacturer);
    

    QString boardSN = QString::fromStdWString(motherBoardInfor.SerialNumber);
    this->ContentAddItem(QObject::tr("Serial Number"), boardSN);

    this->ContentAddBlankLine();

    QString biosSN = QString::fromStdWString(motherBoardInfor.BiosSerialNumber);
    this->ContentAddItem(QObject::tr("BIOS SerialNumber"), biosSN);

    QString biosVersion = QString::fromStdWString(motherBoardInfor.BiosVersion);
    this->ContentAddItem(QObject::tr("BIOS Version"), biosVersion);

    QString biosReleaseDate = QString::fromStdWString(motherBoardInfor.BiosReleaseDate).trimmed();
    this->ContentAddItem(QObject::tr("BIOS Release Date"), biosReleaseDate);

    QString biosVendor = QString::fromStdWString(motherBoardInfor.BiosVendor).trimmed();
    this->ContentAddItem(QObject::tr("BIOS Vendor"), biosVendor);
}

void MemoryItemInfor::LoadHWInfor()
{
    this->ClearInfor();

    this->SetTitle("Memory");

    // 填写内存信息
    const PhysicalMemoryInforArray& physicalMemoryInforArray = HardwareInfor::GetInstance().GetPhysicalMemoryInfor();
    for (unsigned long i = 0; i < physicalMemoryInforArray.Count; i++)
    {
        QString memoryModel = QString::fromStdWString(physicalMemoryInforArray.PartNumbe[i]);
        this->ContentAddItem(QObject::tr("Model Name"), memoryModel);

        QString memoryManufacturer = QString::fromStdWString(physicalMemoryInforArray.Manufacturer[i]);
        this->ContentAddItem(QObject::tr("Manufacturer"), memoryManufacturer);

        QString memorySerialNumber = QString::fromStdWString(physicalMemoryInforArray.SerialNumber[i]);
        this->ContentAddItem(QObject::tr("Serial Number"), memorySerialNumber);

        QString memorySize = QString::fromStdWString(L"%1M").arg(physicalMemoryInforArray.Capacity[i]);
        this->ContentAddItem(QObject::tr("Size"), memorySize);

        QString memorySpeed = QString::fromStdWString(L"%1MHz").arg(physicalMemoryInforArray.Speed[i]);
        this->ContentAddItem(QObject::tr("Speed"), memorySpeed);

        this->ContentAddBlankLine();
    }

}

void DiskItemInfor::LoadHWInfor()
{
    this->ClearInfor();

    this->SetTitle("Disk");

    const DiskInforArray& diskInforArray = HardwareInfor::GetInstance().GetDiskInfor();
    for (unsigned long i = 0; i < diskInforArray.Count; i++)
    {
        if (diskInforArray.DiskType[i] != FIXED_IDE_DISK)
            continue;

        QString model = QString::fromStdWString(diskInforArray.Model[i]);
        this->ContentAddItem(QObject::tr("Model"), model);

        QString size = QString::fromStdString("%1G").arg(diskInforArray.TotalSize[i]);
        this->ContentAddItem(QObject::tr("Size"), size);

        QString serialNumber = QString::fromStdWString(diskInforArray.SerialNumber[i]);
        this->ContentAddItem(QObject::tr("Serial Number"), serialNumber);

        this->ContentAddBlankLine();
    }


}

void VideoCardItemInfor::LoadHWInfor()
{
    this->ClearInfor();

    this->SetTitle("Video Card");

    // 填写显卡信息
    const VideoCardInforArray& videoCardInforArray = HardwareInfor::GetInstance().GetVideoCardInfor();
    for (int i = 0; i < videoCardInforArray.Count; i++)
    {
        QString videoCardName = QString::fromStdWString(videoCardInforArray.Description[i]);
        this->ContentAddItem(QObject::tr("Model"), videoCardName);

        QString videoCardType;
        if (videoCardInforArray.Type[i] == DISPLAY_CARD_INTERNAL)
            videoCardType = QString::fromStdWString(L"Integrated");
        else if(videoCardInforArray.Type[i] == DISPLAY_CARD_EXTERNAL)
            videoCardType = QString::fromStdWString(L"Independent");
        else
            videoCardType = QString::fromStdWString(L"Unknown");

        this->ContentAddItem(QObject::tr("Type"), videoCardType);

        QString videoSize = QString::fromStdString("%1M").arg(videoCardInforArray.RAMSize[i]);

        this->ContentAddItem(QObject::tr("Size"), videoSize);

        this->ContentAddBlankLine();
    }
}

void MonitorItemInfor::LoadHWInfor()
{
    this->ClearInfor();

    this->SetTitle("Monitor");

    const MonitorInforArray& monitorInforArray = HardwareInfor::GetInstance().GetMonitorInfor();
    for (unsigned long i = 0; i < monitorInforArray.Count; i++)
    {
        
        QString monitorName = QString::fromStdWString(monitorInforArray.Name[i]);
        this->ContentAddItem(QObject::tr("Model"), monitorName);

        QString date = QString::fromStdWString(monitorInforArray.Date[i]);
        this->ContentAddItem(QObject::tr("Product Date"), date);

        this->ContentAddBlankLine();
    }
}

void BatteryItemInfor::LoadHWInfor()
{
    this->ClearInfor();

    this->SetTitle("Battery");

    const BatteryStaticInfor& batteryStaticInfor = HardwareInfor::GetInstance().GetBatteryStaticInfor();

    QString batteryName = QString::fromStdWString(batteryStaticInfor.Name).trimmed();
    this->ContentAddItem(QObject::tr("Name"), batteryName);

    QString batteryManufacturer = QString::fromStdWString(batteryStaticInfor.Manufacturer).trimmed();
    this->ContentAddItem(QObject::tr("Manufacturer"), ConvertManufacturer(batteryManufacturer));

    QString batterySerialNumber = QString::fromStdWString(batteryStaticInfor.SerialNumber).trimmed();
    this->ContentAddItem(QObject::tr("Serial Number"), batterySerialNumber);

    QString designedCapacity = QString::fromStdString("%1 mWh").arg(batteryStaticInfor.DesignedCapacity);
    this->ContentAddItem(QObject::tr("Designed Capacity"), designedCapacity);

    QString fullCapacity = QString::fromStdString("%1 mWh").arg(batteryStaticInfor.FullChargedCapacity);
    this->ContentAddItem(QObject::tr("Full Capacity"), fullCapacity);

    QString designedVoltage = QString::fromStdString("%1 mV").arg(batteryStaticInfor.DesignedVoltage);
    this->ContentAddItem(QObject::tr("Designed Voltage"), designedVoltage);

}

void NetworkCardItemInfor::LoadHWInfor()
{
    this->ClearInfor();

    this->SetTitle(QObject::tr("Network Card"));

    const NetworkCardInforArray& networkCardInforArray = HardwareInfor::GetInstance().GetNetworkCardInfor();

    for (unsigned long i = 0; i < networkCardInforArray.Count; i++)
    {
        QString name = QString::fromStdWString(networkCardInforArray.Name[i]);
        this->ContentAddItem(QObject::tr("Name"), name);

        QString type;
        if (networkCardInforArray.Type[i] == ETHERNET_NETCARD)
            type = "Ethernet";
        else if (networkCardInforArray.Type[i] == WIFI_NETCARD)
            type = "Wi-Fi";
        else if (networkCardInforArray.Type[i] == BLUETOOTH_NETCARD)
            type = "Bluetooth";
        else
            type = "Unknown";

        this->ContentAddItem(QObject::tr("Type"), type);

        QString manufacturer = ConvertManufacturer(QString::fromStdWString(networkCardInforArray.Manufacturer[i]));
        this->ContentAddItem(QObject::tr("Manufacturer"), manufacturer);

        QString macAddress = QString::fromStdWString(networkCardInforArray.MACAddress[i]);
        this->ContentAddItem(QObject::tr("MACAddress"), macAddress);

        this->ContentAddBlankLine();
    }
}