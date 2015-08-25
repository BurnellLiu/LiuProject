

#ifndef _HARDWAREINFORMATION_H_
#define _HARDWAREINFORMATION_H_


#include "ui_HardwareInformation.h"

class HardwareInformation : public QWidget
{
    Q_OBJECT

public:
    HardwareInformation(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~HardwareInformation();

private:
    Ui::HardwareInformationForm ui;
};


#endif