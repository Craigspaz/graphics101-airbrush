#include "mainwindow.h"
#include <QApplication>
#include <QStatusBar>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    // I don't like the status bar. Hide it.
    w.statusBar()->hide();

    return a.exec();
}
