#include "MainPage.h"

#include <QtGui/QApplication>


/// @brief 主函数
/// @param[in] argc
/// @param[in] argv
/// @return 发生错误返回0, 有新版本需要更新返回1, 没有新版本需要更新返回2, 更新完成返回3 
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
