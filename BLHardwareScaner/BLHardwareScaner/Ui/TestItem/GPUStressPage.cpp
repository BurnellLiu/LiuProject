
#include "GPUStressPage.h"

#include "..\\Src\\LHardwareInfor.h"

#define GPU_TEST_START QString::fromStdWString(L"Test Start")
#define GPU_TEST_STOP QString::fromStdWString(L"Test Stop")

#define TIME_5M   "5M"    // 5分钟
#define TIME_15M  "15M"   // 15分钟
#define TIME_30M  "30M"   // 30分钟
#define TIME_1H   "1H"    // 1小时
#define TIME_2H   "2H"    // 2小时
#define TIME_4H   "4H"    // 4小时
#define TIME_8H   "8H"    // 8小时
#define TIME_16H  "16H"   // 16小时


GPUStressPage::GPUStressPage(IN float uiRatio, IN QWidget *parent /* = 0 */, IN Qt::WFlags flags /* = 0 */)
    : QDialog(parent, flags)
{
    m_gpuProcess.setWorkingDirectory(".\\GPUStress\\");
    m_gpuTimer.setInterval(500);
    QObject::connect(&m_gpuTimer, SIGNAL(timeout()), this, SLOT(GpuStressTimerTimeout()));
    

    ui.setupUi(this);

    QObject::connect(ui.startTestButton, SIGNAL(clicked()), this, SLOT(TestButtonClicked()));

    const VideoCardInforArray& videoCards = LHardwareInfor::GetVideoCardInfor();
    for (unsigned int i = 0; i < videoCards.Count; i++)
    {
        ui.deviceComboBox->addItem(QString::fromStdWString(videoCards.Description[i]));
    }

    ui.timeComboBox->addItem(TIME_5M);
    ui.timeComboBox->addItem(TIME_15M);
    ui.timeComboBox->addItem(TIME_30M);
    ui.timeComboBox->addItem(TIME_1H);
    ui.timeComboBox->addItem(TIME_2H);
    ui.timeComboBox->addItem(TIME_4H);
    ui.timeComboBox->addItem(TIME_8H);
    ui.timeComboBox->addItem(TIME_16H);

}


GPUStressPage::~GPUStressPage()
{
}

void GPUStressPage::showEvent(QShowEvent* e)
{
    static bool s_intiDone = false;

    if (!s_intiDone)
    {
        ui.startTestButton->setText(GPU_TEST_START);
        s_intiDone = true;
    }
}

void GPUStressPage::TestButtonClicked()
{
    if (ui.startTestButton->text() == GPU_TEST_START)
    {
        ui.deviceComboBox->setEnabled(false);
        ui.timeComboBox->setEnabled(false);
        ui.startTestButton->setText(GPU_TEST_STOP);

        m_gpuProcess.start(".\\GPUStress\\GPUStress.exe");
        m_gpuTimer.start();
    }
    else if (ui.startTestButton->text() == GPU_TEST_STOP)
    {
        m_gpuProcess.terminate();
    }
}

void GPUStressPage::GpuStressTimerTimeout()
{
    if (m_gpuProcess.state() == QProcess::NotRunning)
    {
        ui.deviceComboBox->setEnabled(true);
        ui.timeComboBox->setEnabled(true);
        ui.startTestButton->setText(GPU_TEST_START);

        m_gpuTimer.stop();
    }
    
}