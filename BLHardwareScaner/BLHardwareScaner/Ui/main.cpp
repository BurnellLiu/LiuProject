
#include <QtGui/QMessageBox>
#include <QtGui/QApplication>
#include <QtCore/QDir>


#include "MainWindow.h"
#include "..\\Src\\Log\\LLog.h"

#define LOG_PATH ".\\Log"

int main(int argc, char *argv[])
{
    // 检查或创建LOG文件夹
    QDir logDir;
    if (!logDir.exists(LOG_PATH))
    {
        if (!logDir.mkpath(LOG_PATH))
        {
            QMessageBox::warning(NULL, "Warning", "Can't Create Log Folder");
        }
    }

    OpenLog(".\\Log\\TraceLog.txt");
    PrintLogW(L"Log Start");
   
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    a.exec();

    PrintLogW(L"Log End");
    CloseLog();

    return 0;
}
