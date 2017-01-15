#include "lbtclock.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    lbtClock w;
    w.show();

    return a.exec();
}
