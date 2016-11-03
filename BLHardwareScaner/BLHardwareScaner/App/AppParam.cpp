
#include "AppParam.h"

#include <QtCore/QFile>


#define VERSION_FILE_PATH ".\\Config\\Version"
#define STRING_PARAM_RESTARTAGING "-RestartAging"


static APP_START_MODE s_appStartMode = APP_NORMAL; ///< ��������ģʽ
static QString s_appVersion = ""; ///< ����汾

/// @brief ������������ģʽ
/// @param[in] argc �����в�����Ŀ
/// @param[in] argv[] �����в���
/// @return ��������ģʽ
static APP_START_MODE ParseStartMode(int argc, char *argv[])
{
    APP_START_MODE startMode = APP_NORMAL;
    if (1 == argc)
    {
        startMode = APP_NORMAL;
    }

    if (2 == argc)
    {
        QString param = argv[1];
        if (param == STRING_PARAM_RESTARTAGING)
        {
            startMode = APP_RESTARTAGING;
        }
        else
        {
            startMode = APP_NORMAL;
        }
    }

    return startMode;
}

/// @brief ��ȡ����汾��Ϣ
/// @return ����汾��Ϣ
static QString ReadAppVersion()
{
    QFile versionFile(VERSION_FILE_PATH);
    if (!versionFile.open(QIODevice::ReadOnly))
    {
        return "";
    }

    QString version = versionFile.readAll();
    versionFile.close();

    return version.trimmed();
}

APP_START_MODE LAppParam::GetStartMode()
{
    return s_appStartMode;
}

QString LAppParam::GetAppVersion()
{
    return s_appVersion;
}

QString LAppParam::GetDefaultQssPath()
{
    return ".\\Config\\QSS\\Default\\";
}

void LAppParam::InitAppParam(int argc, char *argv[])
{
    s_appStartMode = ParseStartMode(argc, argv);
    s_appVersion = ReadAppVersion();
}


