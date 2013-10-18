#include <QApplication>
#include <QSplashScreen>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    application.setApplicationName("Gaiascape");

    QSplashScreen splashScreen(QPixmap("media/splashscreen.png"));
    splashScreen.show();

    MainWindow mainWindow;
    mainWindow.show();

    splashScreen.finish(&mainWindow);
    
    return application.exec();
}
