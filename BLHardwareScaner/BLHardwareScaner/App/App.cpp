
#include "App.h"

#include <QtCore/QString>

#define STRING_PARAM_RESTARTAGING "-RestartAging"

APP_START_MODE LAppStartMode::Get()
{
    return sAppRunMode;
}

void LAppStartMode::Init(int argc, char *argv[])
{
    if (1 == argc)
    {
        sAppRunMode = APP_NORMAL;
        return;
    }

    if ( 2 == argc)
    {
        QString param = argv[1];
        if (param == STRING_PARAM_RESTARTAGING)
        {
            sAppRunMode = APP_RESTARTAGING;
        }else
        {
            sAppRunMode = APP_NORMAL;
        }

        return;
    }

    return;
}

APP_START_MODE LAppStartMode::sAppRunMode = APP_NORMAL;
