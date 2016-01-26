
#include "DiskSpeedPage.h"

DiskSpeedPage::DiskSpeedPage(QWidget *parent, Qt::WFlags flags)
    : QDialog(parent, flags)
{
    ui.setupUi(this); 

    m_bInitDone = false;
    m_uiRatio = 1.0f;
}

DiskSpeedPage::~DiskSpeedPage()
{

}

void DiskSpeedPage::SetUIRatio(IN float ratio)
{
    m_uiRatio = ratio;
}

void DiskSpeedPage::showEvent(QShowEvent* e)
{
    if (!m_bInitDone)
    {
        this->Init();
        m_bInitDone = true;
    }
}

void DiskSpeedPage::Init()
{

}