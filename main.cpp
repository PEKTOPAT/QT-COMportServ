#include "generatedata.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GenerateData w;

    //w.setAttribute(Qt::WA_ShowWithoutActivating);
    //w.setParent(0);
    //w.setWindowFlags(Qt::Tool | Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    w.show();


    return a.exec();
}
