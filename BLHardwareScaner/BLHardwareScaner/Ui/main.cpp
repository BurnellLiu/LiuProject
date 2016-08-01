
#include <QtGui/QMessageBox>
#include <QtGui/QApplication>
#include <QtCore/QDir>
#include <QtCore/QFile>

#include "MainPage.h"
#include "..\\Src\\Log\\LLog.h"
#include "..\\Src\\LAppInstance.h"


#define LOG_PATH ".\\Temp\\Log"

// Ӧ�ó���ʵ������
#define APP_INSTANCE_NAME L"Global\\BLHWScaner{EB83918F-DAFD-47E6-A363-9B874F2CF2AF}"

/// @brief ���·��, ·���������򴴽�
/// @param[in] qstrPath ·��
/// @return �ɹ�����true, ʧ�ܷ���false
static bool CheckPath(IN const QString& qstrPath)
{
    QDir logDir;
    if (!logDir.exists(qstrPath))
    {
        return logDir.mkpath(qstrPath);
    }

    return true;
}

/// @brief ����Ƿ����µĸ��³���Ŀ�ִ���ļ�
/// �������ɾ�����ļ�, ���������ļ�
/// unrar.dllN���Ϊunrar.dll
/// UpdateInstall.exeN���ΪUpdateInstall.exe
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
    // �������Ƿ��Ѿ�������, ����Ѿ���һ��ʵ��������, ���˳���ǰʵ��
    LAppInstance appInstance(APP_INSTANCE_NAME);
    if (appInstance.IsAlreadyExist())
        return 0;


    // ��LOG��
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