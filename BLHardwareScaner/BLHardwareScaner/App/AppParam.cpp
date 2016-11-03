
#include "AppParam.h"

#include <QtCore/QString>

#define STRING_PARAM_RESTARTAGING "-RestartAging"

APP_START_MODE LAppParam::GetStartMode()
{
    return sAppRunMode;
}

void LAppParam::Init(int argc, char *argv[])
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

APP_START_MODE LAppParam::sAppRunMode = APP_NORMAL;
