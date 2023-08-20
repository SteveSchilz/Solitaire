#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qDebug() << "Hello";

    MainWindow w;
    w.show();

    return app.exec();
}
