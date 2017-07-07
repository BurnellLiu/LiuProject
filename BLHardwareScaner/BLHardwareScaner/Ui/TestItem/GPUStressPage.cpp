
#include "GPUStressPage.h"

GPUStressPage::GPUStressPage(IN float uiRatio, IN QWidget *parent /* = 0 */, IN Qt::WFlags flags /* = 0 */)
    : QDialog(parent, flags)
{
    ui.setupUi(this);
}


GPUStressPage::~GPUStressPage()
{

}

void GPUStressPage::showEvent(QShowEvent* e)
{

}