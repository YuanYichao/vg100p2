#include "setting.h"
#include <QApplication>
#include <iostream>
#include <glwidget.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GLWidget w(nullptr);
    w.show();
    Setting s(nullptr, &w);
    s.show();

    return a.exec();
}
