
#include <QtGui/QMessageBox>
#include <QtGui/QApplication>
#include <QtCore/QDir>
#include <QtCore/QFile>

#include "MainPage.h"
#include "..\\Src\\Log\\LLog.h"
#include "..\\Src\\LAppInstance.h"


#define LOG_PATH ".\\Temp\\Log"

// 应用程序实例名称
#define APP_INSTANCE_NAME L"Global\\BLHWScaner{EB83918F-DAFD-47E6-A363-9B874F2CF2AF}"

/// @brief 检查路径, 路径不存在则创建
/// @param[in] qstrPath 路径
/// @return 成功返回true, 失败返回false
static bool CheckPath(IN const QString& qstrPath)
{
    QDir logDir;
    if (!logDir.exists(qstrPath))
    {
        return logDir.mkpath(qstrPath);
    }

    return true;
}

/// @brief 检查是否有新的更新程序的可执行文件
/// 如果有则删除旧文件, 重命名新文件
/// unrar.dllN变更为unrar.dll
/// UpdateInstall.exeN变更为UpdateInstall.exe
static void CheckNewUpdateExeFile()
{
    bool bRet = QFile::exists(".\\Update\\unrar.dllN");
    if (bRet)
    {
        QFile::remove(".\\Update\\unrar.dll");
        QFile::rename(".\\Update\\unrar.dllN", ".\\Update\\unrar.dll");
    }

    bRet = QFile::exists(".\\Update\\UpdateInstall.exeN");
    if (bRet)
    {
        QFile::remove(".\\Update\\UpdateInstall.exe");
        QFile::rename(".\\Update\\UpdateInstall.exeN", ".\\Update\\UpdateInstall.exe");
    }
}

int main(int argc, char *argv[])
{
    // 检测程序是否已经在运行, 如果已经有一个实例在运行, 则退出当前实例
    LAppInstance appInstance(APP_INSTANCE_NAME);
    if (appInstance.IsAlreadyExist())
        return 0;


    // 打开LOG档
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


    CheckNewUpdateExeFile();
   
    QApplication app(argc, argv);
    MainPage mainPage;

    mainPage.show();

    app.exec();

    PrintLogW(L"Log End");

    LLog::Close();

    return 0;
}