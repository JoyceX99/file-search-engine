#include "finder.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Finder w;
    w.show();

    return a.exec();
}
