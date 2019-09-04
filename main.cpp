#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("HTY");
    a.setApplicationName("HTYFTP");
    qSetMessagePattern("[ %{file}: %{line} ] %{message}");
    MainWindow w;
    w.show();
    return a.exec();
}