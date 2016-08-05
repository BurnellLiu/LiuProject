
#include "RestartAgingPage.h"

#include <QtGui/QValidator>
#include <QtCore/QDir>
#include <QtCore/QSettings>

// Ĭ�ϼ��ʱ��60��
#define DEFAULT_INTERVALTIME 60

// Ĭ��Aging����60��
#define DEFAULT_AGINGTIMES 60

#define STRING_START_TEST QObject::tr("StartTest")
#define STRING_STOP_TEST QObject::tr("StopTest")

#define RESTARTAGING_TTEST_CONFIG_PATH ".\\Temp\\TestItem\\"
#define RESTARTAGING_TTEST_CONFIG_FILE ".\\Temp\\TestItem\\RestartAging.ini"

#define RESTARTAGING_INTERVALTIMES "Test/IntervalTimes"
#define RESTARTAGING_AGINGTIMES "Test/AgingTimes"
#define RESTARTAGING_COMPLETETIMES "Test/CompleteTimes"
#define RESTARTAGING_BSODTIMES "Test/BSODTimes"


RestartAgingPage::RestartAgingPage(IN QWidget *parent /* = 0 */, IN Qt::WFlags flags /* = 0 */)
    : QDialog(parent, flags)
{
    ui.setupUi(this);

    QObject::connect(ui.pushButtonTest, SIGNAL(clicked()), this, SLOT(TestButtonClicked()));

    // ���������ֻ����[0, 999]֮�������
    // ��Ϊ����pValidator��parentΪthis, ���Կ��Բ�delete, ��Ϊthis��deleteʱ, pValidatorҲ�ᱻdelete
    QValidator* pValidator = new QIntValidator(0, 999, this);
    ui.lineEditIntervalTime->setValidator(pValidator);
    ui.lineEditAgingTimes->setValidator(pValidator);

    ui.lineEditIntervalTime->setText(QObject::tr("%1").arg(DEFAULT_INTERVALTIME));
    ui.lineEditAgingTimes->setText(QObject::tr("%1").arg(DEFAULT_AGINGTIMES));

    ui.pushButtonTest->setText(STRING_START_TEST);

    this->CheckPath(RESTARTAGING_TTEST_CONFIG_PATH);
}

RestartAgingPage::~RestartAgingPage()
{

}

void RestartAgingPage::showEvent(QShowEvent* e)
{

}

void RestartAgingPage::TestButtonClicked()
{
    if (ui.pushButtonTest->text() == STRING_START_TEST)
    {
        ui.pushButtonTest->setText(STRING_STOP_TEST);
        ui.lineEditIntervalTime->setEnabled(false);
        ui.lineEditAgingTimes->setEnabled(false);
        ui.pushButtonReset->setEnabled(false);

        this->StartTest();

        return;
    }

    if (ui.pushButtonTest->text() == STRING_STOP_TEST)
    {
        ui.pushButtonTest->setText(STRING_START_TEST);
        ui.lineEditIntervalTime->setEnabled(true);
        ui.lineEditAgingTimes->setEnabled(true);
        ui.pushButtonReset->setEnabled(true);

        return;
    }
}

void RestartAgingPage::StartTest()
{
    // ����ǰ��Ϣ�����������ļ���
    QSettings* pConfigIniWrite = new QSettings(RESTARTAGING_TTEST_CONFIG_FILE, QSettings::IniFormat);   
    pConfigIniWrite->setValue(RESTARTAGING_INTERVALTIMES, ui.lineEditIntervalTime->text());    
    pConfigIniWrite->setValue(RESTARTAGING_AGINGTIMES, ui.lineEditAgingTimes->text());  
    pConfigIniWrite->setValue(RESTARTAGING_COMPLETETIMES, ui.labelCompleteTimes->text());  
    pConfigIniWrite->setValue(RESTARTAGING_BSODTIMES, ui.labelBSODTimes->text());
    if (NULL != pConfigIniWrite)
    {
        delete pConfigIniWrite;
        pConfigIniWrite = NULL;
    }
    
}


bool RestartAgingPage::CheckPath(IN const QString& qstrPath)
{
    QDir logDir;
    if (!logDir.exists(qstrPath))
    {
        return logDir.mkpath(qstrPath);
    }

    return true;
}