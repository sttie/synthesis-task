#include "include/audiomenu.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    AudioMenu w;
    w.show();

    return a.exec();
}
