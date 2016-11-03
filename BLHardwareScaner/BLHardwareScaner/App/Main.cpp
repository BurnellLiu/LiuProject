
#include <QtGui/QMessageBox>
#include <QtGui/QApplication>
#include <QtCore/QDir>
#include <QtCore/QFile>

#include "Ui\\MainPage.h"
#include "App\\AppParam.h"
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

/// @brief �����°�װ����Ŀ�ִ���ļ�
/// ������µ���ɾ�����ļ�, ���������ļ�
/// unrar.dllN���Ϊunrar.dll
/// UpdateInstall.exeN���ΪUpdateInstall.exe
static void CheckUpdateInstallFile()
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

    QApplication app(argc, argv);

    // ���ܳ�����ʲô���ķ�ʽ����, �����ó���Ĺ���·��Ϊ���������ڵ�·��
    QString appPath = app.applicationDirPath();
    QDir::setCurrent(appPath);

    // ��ʼ��APP����ģʽ
    LAppParam::Init(argc, argv);
    APP_START_MODE startMode = LAppParam::GetStartMode();

    CheckPath(LOG_PATH);
    // ������ڽ����ؿ���AGING����, ����׷�ӷ�ʽ��LOG
    if (APP_RESTARTAGING == startMode)
    {
        LLog::SetAppendOpen(true);
    }
    LLog::ShowThreadId(true);
    LLog::ShowTime(true);
    LLog::Open(L".\\Temp\\Log\\TraceLog.txt");

    PrintLogW(L"Log Start");
    for (int i = 0; i < argc; i++)
    {
        PrintLogA("Command Argument%d: %s", i, argv[i]);
    }
    if (APP_NORMAL == startMode)
    {
        PrintLogW(L"AppStartMode: APP_NORMAL");
    }
    if (APP_RESTARTAGING == startMode)
    {
        PrintLogW(L"AppStartMode: APP_RESTARTAGING");
    }

    // �����°�װ��ִ���ļ�
    CheckUpdateInstallFile();
   

    // ������ҳ��
    MainPage mainPage;

    // ��ʾ��ҳ��
    mainPage.show();

    // ������Ϣѭ��
    app.exec();

    // �ر�LOG
    PrintLogW(L"Log End");
    LLog::Close();

    // �������
    return 0;
}