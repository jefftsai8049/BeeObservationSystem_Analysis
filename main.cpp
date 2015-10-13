#include "mainwindow.h"
#include <QApplication>
#include <QReadWriteLock>

QReadWriteLock lock(QReadWriteLock::NonRecursive);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    a.setWindowIcon(QIcon("honeybee.jpg"));
    MainWindow w;
    w.show();

    return a.exec();
}
