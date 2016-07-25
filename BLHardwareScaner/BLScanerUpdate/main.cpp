#include "MainPage.h"

#include <QtGui/QApplication>


/// @brief ������
/// @param[in] argc
/// @param[in] argv
/// @return �������󷵻�0, ���°汾��Ҫ���·���1, û���°汾��Ҫ���·���2, ������ɷ���3 
int main(int argc, char *argv[])
{
    if (argc < 2)
        return 0;

    int apMode = 0;

    QString cmd = argv[1];
    cmd = cmd.trimmed();
    if (0 == cmd.compare("-Check"))
    {
        if (argc < 3)
            return 0;

        QString currentVersion = argv[2];
        currentVersion = currentVersion.trimmed();
        apMode = AP_CHECK;
    }
    else if (0 == cmd.compare("-Update"))
    {
        apMode = AP_UPDATE;
    }


    QApplication a(argc, argv);

    MainPage w(apMode);
    w.show();

    return a.exec();
}
