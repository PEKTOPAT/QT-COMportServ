#include "generatedata.h"
#include <QApplication>
#include "threads.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GenerateData w;
    w.show();
    //w.setAttribute(Qt::WA_ShowWithoutActivating);
    //w.setParent(0);
    //w.setWindowFlags(Qt::Tool | Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);



    return a.exec();
}
