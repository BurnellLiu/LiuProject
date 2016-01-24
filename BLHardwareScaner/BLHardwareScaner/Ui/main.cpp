
#include <QtGui/QMessageBox>
#include <QtGui/QApplication>
#include <QtGui/QSplashScreen>
#include <QtCore/QDir>

#include "MainWindow.h"
#include "..\\Src\\Log\\LLog.h"

#define LOG_PATH ".\\Log"

/// @brief 检查路径, 路径不存在则创建
/// @param[in] qstrPath 路径
/// @return 成功返回true, 失败返回false
bool CheckPath(IN const QString& qstrPath)
{
    QDir logDir;
    if (!logDir.exists(qstrPath))
    {
        return logDir.mkpath(qstrPath);
    }

    return true;
}

int main(int argc, char *argv[])
{
   
    CheckPath(LOG_PATH);

    LLog::ShowThreadId(true);
    LLog::ShowTime(true);
    LLog::Open(L".\\Log\\TraceLog.txt");

    PrintLogW(L"Log Start");
    PrintLogA("Program Path: %s", argv[0]);
    for (int i = 1; i < argc; i++)
    {
        PrintLogA("Command Argument%d: %s", i, argv[i]);
    }
   
    QApplication app(argc, argv);
    MainWindow mainWindow;

    QPixmap splashMap(":/MyResource/Images/splash.png");
    QSplashScreen splashScreen(splashMap);
    splashScreen.show();
    splashScreen.showMessage(QObject::tr("Getting Hardware Information..."), Qt::AlignLeft | Qt::AlignTop, Qt::red);
    mainWindow.show();
    splashScreen.finish(&mainWindow);

    app.exec();

    PrintLogW(L"Log End");

    LLog::Close();

    return 0;
}