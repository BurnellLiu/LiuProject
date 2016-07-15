
#include <QtGui/QMessageBox>
#include <QtGui/QApplication>
#include <QtGui/QSplashScreen>
#include <QtCore/QDir>

#include "MainPage.h"
#include "..\\Src\\Log\\LLog.h"

#define MAIN_TITLE "BLHardwareScaner(V1.1.0)"
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
    MainPage mainPage;

	mainPage.setWindowTitle(MAIN_TITLE);

    // 缩放启动图片
    float uiRatio = mainPage.GetUiResizeRatio();
    QPixmap originalImage(".\\Image\\Background\\splash.png");
    QSize imageSize(originalImage.width() * uiRatio, originalImage.height() * uiRatio);
    QPixmap scaledImage = originalImage.scaled(imageSize, Qt::KeepAspectRatio);


    QSplashScreen splashScreen(scaledImage);

    splashScreen.show();
    splashScreen.showMessage(QObject::tr("Getting Hardware Information..."), Qt::AlignLeft | Qt::AlignTop, Qt::red);
    mainPage.show();
    splashScreen.finish(&mainPage);

    app.exec();

    PrintLogW(L"Log End");

    LLog::Close();

    return 0;
}