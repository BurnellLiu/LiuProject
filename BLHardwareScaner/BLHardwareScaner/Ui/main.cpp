
#include <QtGui/QMessageBox>
#include <QtGui/QApplication>
#include <QtCore/QDir>


#include "MainWindow.h"
#include "..\\Src\\Log\\LLog.h"

#define LOG_PATH ".\\Log"
#define CMD_ARG_DEBUG "-DEBUG"

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
    if (argc >= 2)
    {
        QString cmdArg = argv[1];
        cmdArg = cmdArg.toUpper().trimmed();
        if (cmdArg.compare(CMD_ARG_DEBUG) == 0)
            LLog::SetDelayedWrite(false);
    }
    LLog::Open(".\\Log\\TraceLog.txt");

    PrintLogW(L"Log Start");
    PrintLogA("Program Path: %s", argv[0]);
    for (int i = 1; i < argc; i++)
    {
        PrintLogA("Command Argument%d: %s", i, argv[i]);
    }
   
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    a.exec();

    PrintLogW(L"Log End");

    LLog::Close();

    return 0;
}