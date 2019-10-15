#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setGeometry(100,100,600,500);
    w.show();
    return a.exec();
}
