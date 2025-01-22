#include "stdafx.h"
#include "CrafterOfLight.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CrafterOfLight w;
    w.show();
    return a.exec();
}
