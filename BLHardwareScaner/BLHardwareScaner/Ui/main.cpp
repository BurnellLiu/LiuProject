
#include "MainWindow.h"
#include <QtGui/QApplication>

#include "..\\Src\\Log\\LLog.h"

int main(int argc, char *argv[])
{
    LLog::Open(".\\Log\\TraceLog.txt");
    LLog::WriteLineW(L"Log Start");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    a.exec();

    LLog::WriteLineW(L"Log End");
    LLog::Close();

    return 0;
}
