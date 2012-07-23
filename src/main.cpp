#include <QtGui/QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    application.setApplicationName("Gaiascape");
    MainWindow mainWindow;
    mainWindow.show();
    
    return application.exec();
}
