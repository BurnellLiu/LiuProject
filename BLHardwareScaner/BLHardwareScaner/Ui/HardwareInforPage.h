

#ifndef _HARDWAREINFORPAGE_H_
#define _HARDWAREINFORPAGE_H_


#include "ui_HardwareInfor.h"

class HardwareInforPage : public QWidget
{
    Q_OBJECT

public:
    HardwareInforPage(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~HardwareInforPage();

private:
    Ui::HardwareInforForm ui;
};


#endif