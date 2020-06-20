#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
//    QApplication::setStyle(QStyleFactory::create("Fusion"));
//    qDebug()<<QStyleFactory::keys();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
