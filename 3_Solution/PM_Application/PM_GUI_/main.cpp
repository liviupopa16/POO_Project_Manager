#include "PM_GUI_.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PM_GUI_ w;
    w.show();
    return a.exec();
}
