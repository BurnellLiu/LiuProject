
#include <QtGui/QMessageBox>
#include <QtGui/QApplication>
#include <QtGui/QSplashScreen>
#include <QtCore/QDir>

#include "MainPage.h"
#include "..\\Src\\Log\\LLog.h"

#define MAIN_TITLE "BLHardwareScaner(V1.1.0)"
#define LOG_PATH ".\\Log"

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

    // ��������ͼƬ
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