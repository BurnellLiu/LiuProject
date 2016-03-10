
#include "HardwareInforPage.h"

#include <QtCore/QString>
#include <QtCore/QFile>

#include "..\\Src\\LSensorObject.h"
#include "..\\Src\\HardwareInfor.h"
#include "..\\Src\\Log\\LLog.h"

HardwareInforPage::HardwareInforPage(QWidget *parent, Qt::WFlags flags)
    : QWidget(parent, flags)
{
    ui.setupUi(this);

    this->LoadQSS();

    QObject::connect(ui.listWidgetHWItem, SIGNAL(itemSelectionChanged()), this, SLOT(CurrentItemChanged()));    
}

HardwareInforPage::~HardwareInforPage()
{
    for (int i = 0; i < m_hwItemVec.size(); i++)
    {
        delete m_hwItemVec[i];
        m_hwItemVec[i] = 0;
    }
}

void HardwareInforPage::showEvent(QShowEvent* e)
{
    static bool s_initDone = false;
    if (!s_initDone)
    {
        this->InitHardwareInfor();
        ui.listWidgetHWItem->setCurrentRow(0);
        s_initDone = true;
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

void HardwareInforPage::InitHardwareInfor()
{
    ui.listWidgetHWItem->clear();
    for (int i = 0; i < m_hwItemVec.size(); i++)
    {
        delete m_hwItemVec[i];
        m_hwItemVec[i] = 0;
    }
    m_hwItemVec.clear();


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
    const ComputerSystemInfor& computerSystemInfor = HardwareInfor::GetInstance().GetComputerSystemInfor();
    if (batteryStaticInfor.Exist || 
        computerSystemInfor.Type == COMPUTER_TYPE_NOTE_BOOK ||
        computerSystemInfor.Type == COMPUTER_TYPE_TABLET)
    {
        ui.listWidgetHWItem->addItem(tr("Battery"));
        HWItemInfor* pBatteryItem = new BatteryItemInfor();
        m_hwItemVec.push_back(pBatteryItem);
    }

    LSensorObject sensorObject;

    SAccelerometer3DInforArray accelerometerSensor;
    sensorObject.GetAccelerometer3DInfor(&accelerometerSensor);
    SGyrometer3DInforArray gyrometerSensor;
    sensorObject.GetGyrometer3DInfor(&gyrometerSensor);
    SCompass3DInforArray compassSensor;
    sensorObject.GetCompass3DInfor(&compassSensor);
    SAmbientLightInforArray lightSensor;
    sensorObject.GetAmbientLightInfor(&lightSensor);
    SGpsInforArray gpsSensor;
    sensorObject.GetGpsInfor(&gpsSensor);
    if (accelerometerSensor.Count > 0 || 
        gyrometerSensor.Count > 0 || 
        compassSensor.Count > 0 || 
        lightSensor.Count > 0 || 
        gpsSensor.Count > 0)
    {
        ui.listWidgetHWItem->addItem(tr("Sensors"));
        HWItemInfor* pSensorsItem = new SensorsItemInfor();
        m_hwItemVec.push_back(pSensorsItem);
    }



    for (int i = 0; i < m_hwItemVec.size(); i++)
    {
        m_hwItemVec[i]->LoadHWInfor();
    }
}

void HardwareInforPage::LoadQSS()
{
    QFile qssFile(".\\Qss\\Default\\HardwareInforPage.qss");  
    qssFile.open(QFile::ReadOnly);  

    if(qssFile.isOpen())  
    {  
        QString qss = qssFile.readAll();
        qssFile.close();  

        this->setStyleSheet(qss);
    }  
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
    QString strOperatingSystemVersion;
    strOperatingSystemVersion = QString::fromStdWString(operatingSystemInfor.Version);
    this->ContentAddItem(QObject::tr("OS Version"), strOperatingSystemVersion);
    this->ContentAddBlankLine();

    // 填写主板信息
    const MotherBoardInfor& motherBoardInfor = HardwareInfor::GetInstance().GetMotherBoardInfor();
    QString motherBoardProductName = QString::fromStdWString(motherBoardInfor.ProductName).trimmed();
    QString motherBoardManufacturer = QString::fromStdWString(motherBoardInfor.Manufacturer);
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
        if (videoCardInfor.Type[i] == VIDEO_CARD_INTERNAL)
            displayCardName += QString::fromStdWString(L"  (Integrated)");
        else if(videoCardInfor.Type[i] == VIDEO_CARD_EXTERNAL)
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
        if (diskInforArray.DiskType[i] != FIXED_DISK)
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

    // 填写电池信息
    const BatteryStaticInfor& batteryStaticInfor = HardwareInfor::GetInstance().GetBatteryStaticInfor();
    if (batteryStaticInfor.Exist)
    {
        QString batteryManufacturer = QString::fromStdWString(batteryStaticInfor.Manufacturer);

        QString batteryName = QString::fromStdWString(batteryStaticInfor.Name);

        QString batteryInfor = batteryManufacturer;
        batteryInfor += "  ";
        batteryInfor += batteryName;

        this->ContentAddItem(QObject::tr("Battery"), batteryInfor);
    }
    else
    {
        if (computerSystemInfor.Type == COMPUTER_TYPE_NOTE_BOOK || 
            computerSystemInfor.Type == COMPUTER_TYPE_TABLET)
        {
            QString errorInfor = "Disconnect";
            this->ContentAddItem(QObject::tr("Battery"), errorInfor);
        }
    }

    // 填写光驱信息
    const CDRomDriveInforArray& cdRomDriveInforArray = HardwareInfor::GetInstance().GetCDRomDriveInfor();
    for (unsigned int i = 0; i < cdRomDriveInforArray.Count; i++)
    {
        QString cdRomName = QString::fromStdWString(cdRomDriveInforArray.Name[i]);
        this->ContentAddItem(QObject::tr("CD ROM Drive"), cdRomName);
    }
}

void ProcessorItemInfor::LoadHWInfor()
{
    this->ClearInfor();

    this->SetTitle(QObject::tr("Processor"));
    PrintLogW(L"Processor Information:");

    const ProcessorInfor& processprInfor = HardwareInfor::GetInstance().GetProcessorInfor();

    QString name = QString::fromStdWString(processprInfor.Name);
    this->ContentAddItem(QObject::tr("Name"), name);
    PrintLogW(L"\tName: %s", processprInfor.Name.c_str());

    QString description = QString::fromStdWString(processprInfor.Description);
    this->ContentAddItem(QObject::tr("Description"), description);
    PrintLogW(L"\tDescription: %s", processprInfor.Description.c_str());

    QString manufacturer = QString::fromStdWString(processprInfor.Manufacturer);
    this->ContentAddItem(QObject::tr("Manufacturer"), manufacturer);
    PrintLogW(L"\tManufacturer: %s", processprInfor.Manufacturer.c_str());

    QString coresNumber = QString::fromStdString("Cores: %1    Logical Processors: %2").arg(processprInfor.CoresNumber).arg(processprInfor.LogicalProcessorNumber);
    this->ContentAddItem(QObject::tr("Cores Number"), coresNumber);
    PrintLogW(L"\tPhysical Cores: %u, Logical Cores: %u", processprInfor.CoresNumber, processprInfor.LogicalProcessorNumber);

    QString speed = QString::fromStdString("%1MHz").arg(processprInfor.MaxClockSpeed);
    this->ContentAddItem(QObject::tr("Max Speed"), speed);
    PrintLogW(L"\tMax Speed: %uMHz", processprInfor.MaxClockSpeed);

    PrintLogW(L"");

}

void MotherBoardItemInfor::LoadHWInfor()
{
    this->ClearInfor();

    this->SetTitle(QObject::tr("Mother Board"));
    PrintLogW(L"Mother Board Information:");

    const MotherBoardInfor& motherBoardInfor = HardwareInfor::GetInstance().GetMotherBoardInfor();
    QString boardProductName = QString::fromStdWString(motherBoardInfor.ProductName).trimmed();
    this->ContentAddItem(QObject::tr("Product Name"), boardProductName);
    PrintLogW(L"\tProduct Name: %s", motherBoardInfor.ProductName.c_str());

    QString boardManufacturer = QString::fromStdWString(motherBoardInfor.Manufacturer);
    this->ContentAddItem(QObject::tr("Manufacturer"), boardManufacturer);
    PrintLogW(L"\tManufacturer: %s", motherBoardInfor.Manufacturer.c_str());
    

    QString boardSN = QString::fromStdWString(motherBoardInfor.SerialNumber);
    this->ContentAddItem(QObject::tr("Serial Number"), boardSN);
    PrintLogW(L"\tSerial Number: %s", motherBoardInfor.SerialNumber.c_str());

    this->ContentAddBlankLine();

    QString biosSN = QString::fromStdWString(motherBoardInfor.BiosSerialNumber);
    this->ContentAddItem(QObject::tr("BIOS SerialNumber"), biosSN);
    PrintLogW(L"\tBIOS SerialNumber: %s", motherBoardInfor.BiosSerialNumber.c_str());

    QString biosVersion = QString::fromStdWString(motherBoardInfor.BiosVersion);
    this->ContentAddItem(QObject::tr("BIOS Version"), biosVersion);
    PrintLogW(L"\tBIOS Version: %s", motherBoardInfor.BiosVersion.c_str());

    QString biosReleaseDate = QString::fromStdWString(motherBoardInfor.BiosReleaseDate).trimmed();
    this->ContentAddItem(QObject::tr("BIOS Release Date"), biosReleaseDate);
    PrintLogW(L"\tBIOS Release Date: %s", motherBoardInfor.BiosReleaseDate.c_str());

    QString biosVendor = QString::fromStdWString(motherBoardInfor.BiosVendor).trimmed();
    this->ContentAddItem(QObject::tr("BIOS Vendor"), biosVendor);
    PrintLogW(L"\tBIOS Vendor: %s", motherBoardInfor.BiosVendor.c_str());

    PrintLogW(L"");
}

void MemoryItemInfor::LoadHWInfor()
{
    this->ClearInfor();

    this->SetTitle("Memory");
    PrintLogW(L"Memory Information");

    // 填写内存信息
    const PhysicalMemoryInforArray& physicalMemoryInforArray = HardwareInfor::GetInstance().GetPhysicalMemoryInfor();
    for (unsigned long i = 0; i < physicalMemoryInforArray.Count; i++)
    {
        PrintLogW(L"\tMemory : %u", i);

        QString memoryModel = QString::fromStdWString(physicalMemoryInforArray.PartNumbe[i]);
        this->ContentAddItem(QObject::tr("Model Name"), memoryModel);
        PrintLogW(L"\tModel Name", memoryModel.toStdWString().c_str());

        QString memoryManufacturer = QString::fromStdWString(physicalMemoryInforArray.Manufacturer[i]);
        this->ContentAddItem(QObject::tr("Manufacturer"), memoryManufacturer);
        PrintLogW(L"\tManufacturer: %s", memoryManufacturer.toStdWString().c_str());

        QString memorySerialNumber = QString::fromStdWString(physicalMemoryInforArray.SerialNumber[i]);
        this->ContentAddItem(QObject::tr("Serial Number"), memorySerialNumber);
        PrintLogW(L"\tSerial Number: %s", memorySerialNumber.toStdWString().c_str());

        QString memorySize = QString::fromStdWString(L"%1M").arg(physicalMemoryInforArray.Capacity[i]);
        this->ContentAddItem(QObject::tr("Size"), memorySize);
        PrintLogW(L"\tSize: %s", memorySize.toStdWString().c_str());

        QString memorySpeed = QString::fromStdWString(L"%1MHz").arg(physicalMemoryInforArray.Speed[i]);
        this->ContentAddItem(QObject::tr("Speed"), memorySpeed);
        PrintLogW(L"\tSpeed: %s", memorySpeed.toStdWString().c_str());

        this->ContentAddBlankLine();
    }

    PrintLogW(L"");

}

void DiskItemInfor::LoadHWInfor()
{
    this->ClearInfor();

    this->SetTitle("Disk");
    
    const DiskInforArray& diskInforArray = HardwareInfor::GetInstance().GetDiskInfor();

    PrintLogW(L"Disk Information:");
    for (unsigned long i = 0; i < diskInforArray.Count; i++)
    {
        if (diskInforArray.DiskType[i] != FIXED_DISK)
            continue;

        PrintLogW(L"\tFixed Disk Index: %u", i);

        QString model = QString::fromStdWString(diskInforArray.Model[i]);
        this->ContentAddItem(QObject::tr("Model"), model);
        PrintLogW(L"\tModel: %s", model.toStdWString().c_str());

        QString size = QString::fromStdString("%1G").arg(diskInforArray.TotalSize[i]);
        this->ContentAddItem(QObject::tr("Size"), size);
        PrintLogW(L"\tSize: %s", size.toStdWString().c_str());

        QString logicalName = QString::fromStdWString(diskInforArray.LogicalName[i]);
        logicalName.replace(QString::fromStdWString(L";"), QString::fromStdWString(L" "));
        this->ContentAddItem(QObject::tr("Logical Name"), logicalName);
        PrintLogW(L"\tLogical Name: %s", logicalName.toStdWString().c_str());

        QString serialNumber = QString::fromStdWString(diskInforArray.SerialNumber[i]);
        this->ContentAddItem(QObject::tr("Serial Number"), serialNumber);
        PrintLogW(L"\tSerial Number: %s", serialNumber.toStdWString().c_str());

        QString interfaceType = QString::fromStdWString(diskInforArray.InterfaceType[i]);
        this->ContentAddItem(QObject::tr("Interface Type"), interfaceType);
        PrintLogW(L"\tInterface Type: %s", interfaceType.toStdWString().c_str());

        QString fixedDiskType;
        switch (diskInforArray.FixedDiskType[i])
        {
        case FIXED_DISK_HDD:
            fixedDiskType = "HDD";
            break;
        case FIXED_DISK_SSD:
            fixedDiskType = "SSD";
            break;
        case FIXED_DISK_EMMC:
            fixedDiskType = "EMMC";
            break;
        case FIXED_DISK_RAID:
            fixedDiskType = "RAID";
            break;
        default:
            fixedDiskType = "UNKNOWN";
            break;
        }
        this->ContentAddItem(QObject::tr("Fixed Disk Type"), fixedDiskType);
        PrintLogW(L"\tFixed Disk Type: %d", diskInforArray.FixedDiskType[i]);

        PrintLogW(L"\tATA Disk: %s", diskInforArray.IsATA[i] ? L"Yes" : L"No");
        if (diskInforArray.IsATA[i])
        {
            QString rotationRate;
            if (diskInforArray.ATAInfor[i].RotationRate != 1)
                rotationRate = QString::fromStdWString(L"%1 RPM").arg(diskInforArray.ATAInfor[i].RotationRate);

            if (diskInforArray.ATAInfor[i].RotationRate == 1)
                rotationRate = "SSD";

            this->ContentAddItem(QObject::tr("RotationRate"), rotationRate);
            PrintLogW(L"\tRotationRate: %s", rotationRate.toStdWString().c_str());

            QString sataType = QString::fromStdWString(L"SATA-%1").arg(diskInforArray.ATAInfor[i].SATAType);
            this->ContentAddItem(QObject::tr("SATA Type"), sataType);
            PrintLogW(L"\tSATA Type: %s", sataType.toStdWString().c_str());

            QString powerOnHours = QString::fromStdWString(L"%1 Hours").arg(diskInforArray.ATAInfor[i].PowerOnHours);
            this->ContentAddItem(QObject::tr("Power On Hours"), powerOnHours);
            PrintLogW(L"\tPower On Hours: %s", powerOnHours.toStdWString().c_str());
        }

        this->ContentAddBlankLine();
    }

    PrintLogW(L"");
}

void VideoCardItemInfor::LoadHWInfor()
{
    this->ClearInfor();

    this->SetTitle("Video Card");
    PrintLogW(L"Video Card Information:");

    // 填写显卡信息
    const VideoCardInforArray& videoCardInforArray = HardwareInfor::GetInstance().GetVideoCardInfor();
    for (int i = 0; i < videoCardInforArray.Count; i++)
    {
        PrintLogW(L"\tVideo Card Index: %d", i);

        QString videoCardName = QString::fromStdWString(videoCardInforArray.Description[i]);
        this->ContentAddItem(QObject::tr("Model"), videoCardName);
        PrintLogW(L"\tModel: %s", videoCardName.toStdWString().c_str());

        QString videoCardType;
        if (videoCardInforArray.Type[i] == VIDEO_CARD_INTERNAL)
            videoCardType = QString::fromStdWString(L"Integrated");
        else if(videoCardInforArray.Type[i] == VIDEO_CARD_EXTERNAL)
            videoCardType = QString::fromStdWString(L"Independent");
        else
            videoCardType = QString::fromStdWString(L"Unknown");

        this->ContentAddItem(QObject::tr("Type"), videoCardType);
        PrintLogW(L"\tType: %s", videoCardType.toStdWString().c_str());

        QString videoRAMSize = QString::fromStdString("%1M").arg(videoCardInforArray.RAMSize[i]);
        if (videoCardInforArray.Type[i] == VIDEO_CARD_INTERNAL)
            videoRAMSize += "(Share)";

        this->ContentAddItem(QObject::tr("RAMSize"), videoRAMSize);
        PrintLogW(L"\tRAMSize: %s", videoRAMSize.toStdWString().c_str());

        this->ContentAddBlankLine();
    }

    PrintLogW(L"");
}

void MonitorItemInfor::LoadHWInfor()
{
    this->ClearInfor();

    this->SetTitle("Monitor");

    PrintLogW(L"Monitor Information:");

    const MonitorInforArray& monitorInforArray = HardwareInfor::GetInstance().GetMonitorInfor();
    for (unsigned long i = 0; i < monitorInforArray.Count; i++)
    {
        PrintLogW(L"\tMonitor Index: %u", i);
        QString monitorName = QString::fromStdWString(monitorInforArray.Name[i]);
        this->ContentAddItem(QObject::tr("Model"), monitorName);
        PrintLogW(L"\tModel: %s", monitorName.toStdWString().c_str());

        QString date = QString::fromStdWString(monitorInforArray.Date[i]);
        this->ContentAddItem(QObject::tr("Product Date"), date);
        PrintLogW(L"\tProductDate: %s", date.toStdWString().c_str());

        this->ContentAddBlankLine();
    }

    PrintLogW(L"");
}

void BatteryItemInfor::LoadHWInfor()
{
    this->ClearInfor();

    this->SetTitle("Battery");
    PrintLogW(L"Battery Information:");

    const BatteryStaticInfor& batteryStaticInfor = HardwareInfor::GetInstance().GetBatteryStaticInfor();

    if (batteryStaticInfor.Exist)
    {
        QString batteryName = QString::fromStdWString(batteryStaticInfor.Name).trimmed();
        this->ContentAddItem(QObject::tr("Name"), batteryName);
        PrintLogW(L"\tName: %s", batteryName.toStdWString().c_str());

        QString batteryManufacturer = QString::fromStdWString(batteryStaticInfor.Manufacturer).trimmed();
        this->ContentAddItem(QObject::tr("Manufacturer"), batteryManufacturer);
        PrintLogW(L"\tManufacturer: %s", batteryManufacturer.toStdWString().c_str());

        QString batterySerialNumber = QString::fromStdWString(batteryStaticInfor.SerialNumber).trimmed();
        this->ContentAddItem(QObject::tr("Serial Number"), batterySerialNumber);
        PrintLogW(L"\tSerial Number: %s", batterySerialNumber.toStdWString().c_str());

        QString designedCapacity = QString::fromStdString("%1 mWh").arg(batteryStaticInfor.DesignedCapacity);
        this->ContentAddItem(QObject::tr("Designed Capacity"), designedCapacity);
        PrintLogW(L"\tDesigned Capacity: %s", designedCapacity.toStdWString().c_str());

        QString fullCapacity = QString::fromStdString("%1 mWh").arg(batteryStaticInfor.FullChargedCapacity);
        this->ContentAddItem(QObject::tr("Full Capacity"), fullCapacity);
        PrintLogW(L"\tFullCapacity: %s", fullCapacity.toStdWString().c_str());

        QString designedVoltage = QString::fromStdString("%1 mV").arg(batteryStaticInfor.DesignedVoltage);
        this->ContentAddItem(QObject::tr("Designed Voltage"), designedVoltage);
        PrintLogW(L"\tDesigned Voltage: %s", designedVoltage.toStdWString().c_str());
    }
    else
    {
        QString errorInfor = "Disconnect";
        this->ContentAddItem(QObject::tr("State"), errorInfor);
        PrintLogW(L"\tState: %s", errorInfor.toStdWString().c_str());
    }

    

    PrintLogW(L"");
}

void NetworkCardItemInfor::LoadHWInfor()
{
    this->ClearInfor();

    this->SetTitle(QObject::tr("Network Card"));
    PrintLogW(L"Network Card Information:");

    const NetworkCardInforArray& networkCardInforArray = HardwareInfor::GetInstance().GetNetworkCardInfor();

    for (unsigned long i = 0; i < networkCardInforArray.Count; i++)
    {
        PrintLogW(L"\tNetwork Card Index: %u", i);
        QString name = QString::fromStdWString(networkCardInforArray.Name[i]);
        this->ContentAddItem(QObject::tr("Name"), name);
        PrintLogW(L"\tName: %s", name.toStdWString().c_str());

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
        PrintLogW(L"\tType: %s", type.toStdWString().c_str());

        QString manufacturer = QString::fromStdWString(networkCardInforArray.Manufacturer[i]);
        this->ContentAddItem(QObject::tr("Manufacturer"), manufacturer);
        PrintLogW(L"\tManufacturer: %s", manufacturer.toStdWString().c_str());

        QString macAddress = QString::fromStdWString(networkCardInforArray.MACAddress[i]);
        this->ContentAddItem(QObject::tr("MACAddress"), macAddress);
        PrintLogW(L"\tMACAddress: %s", macAddress.toStdWString().c_str());

        this->ContentAddBlankLine();
    }

    PrintLogW(L"");
}

void SensorsItemInfor::LoadHWInfor()
{
    this->ClearInfor();

    this->SetTitle(QObject::tr("Sensors"));
    PrintLogW(L"Sensors Information:");

    LSensorObject sensorObject;

    SAccelerometer3DInforArray accelerometerSensor;
    sensorObject.GetAccelerometer3DInfor(&accelerometerSensor);
    for (unsigned int i = 0; i < accelerometerSensor.Count; i++)
    {
        QString friendlyName = QString::fromStdWString(accelerometerSensor.FriendlyName[i]);
        this->ContentAddItem(QObject::tr("Accelerometer"), friendlyName);
        PrintLogW(L"\tAccelerometer: %s", accelerometerSensor.FriendlyName[i]);
    }
    this->ContentAddBlankLine();


    SGyrometer3DInforArray gyrometerSensor;
    sensorObject.GetGyrometer3DInfor(&gyrometerSensor);

    for (unsigned int i = 0; i < gyrometerSensor.Count; i++)
    {
        QString friendlyName = QString::fromStdWString(gyrometerSensor.FriendlyName[i]);
        this->ContentAddItem(QObject::tr("Gyrometer"), friendlyName);
        PrintLogW(L"\tGyrometer: %s", gyrometerSensor.FriendlyName[i]);
    }
    this->ContentAddBlankLine();


    SCompass3DInforArray compassSensor;
    sensorObject.GetCompass3DInfor(&compassSensor);
    for (unsigned int i = 0; i < compassSensor.Count; i++)
    {
        QString friendlyName = QString::fromStdWString(compassSensor.FriendlyName[i]);
        this->ContentAddItem(QObject::tr("Compass"), friendlyName);
        PrintLogW(L"\tCompass: %s", compassSensor.FriendlyName[i]);
    }
    this->ContentAddBlankLine();

    SAmbientLightInforArray lightSensor;
    sensorObject.GetAmbientLightInfor(&lightSensor);
    for (unsigned int i =0; i< lightSensor.Count; i++)
    {
        QString friendlyName = QString::fromStdWString(lightSensor.FriendlyName[i]);
        this->ContentAddItem(QObject::tr("Light"), friendlyName);
        PrintLogW(L"\tLight: %s", lightSensor.FriendlyName[i]);
    }
    this->ContentAddBlankLine();

    SGpsInforArray gpsSensor;
    sensorObject.GetGpsInfor(&gpsSensor);
    for (unsigned int i = 0; i < gpsSensor.Count; i++)
    {
        QString friendlyName = QString::fromStdWString(gpsSensor.FriendlyName[i]);
        this->ContentAddItem(QObject::tr("GPS"), friendlyName);
        PrintLogW(L"\tGPS: %s", gpsSensor.FriendlyName[i]);
    }
    this->ContentAddBlankLine();
}