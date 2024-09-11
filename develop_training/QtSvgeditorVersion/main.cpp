#include "kmainwindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    KMainWindow w;
    w.show();
    return a.exec();
}
