#include "generatedata.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GenerateData w;
    w.show();

    return a.exec();
}
