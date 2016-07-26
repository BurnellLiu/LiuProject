
#include <QtGui/QMessageBox>
#include <QtGui/QApplication>
#include <QtCore/QDir>

#include "MainPage.h"
#include "..\\Src\\Log\\LLog.h"


#define LOG_PATH ".\\Temp\\Log"

/// @brief ���·��, ·���������򴴽�
/// @param[in] qstrPath ·��
/// @return �ɹ�����true, ʧ�ܷ���false
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
    LLog::Open(L".\\Temp\\Log\\TraceLog.txt");

    PrintLogW(L"Log Start");
    PrintLogA("Program Path: %s", argv[0]);
    for (int i = 1; i < argc; i++)
    {
        PrintLogA("Command Argument%d: %s", i, argv[i]);
    }
   
    QApplication app(argc, argv);
    MainPage mainPage;

    mainPage.show();

    app.exec();

    PrintLogW(L"Log End");

    LLog::Close();

    return 0;
}