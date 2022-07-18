#include "threads.h"
#include <QDebug>
#include <generatedata.h>

Threads::Threads()

{
    port = new QSerialPort(this);
    port->setDataBits(QSerialPort::Data8);
    port->setFlowControl(QSerialPort::NoFlowControl);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);
    port->setBaudRate(QSerialPort::Baud115200);
    qDebug() << "AND ";
//    for (int i = 0; i <= 100; i++ ) {
//            qDebug() << " " << i;
//        }

}

void Threads::run()
{

//    connect(port, SIGNAL(readyRead()), this, SLOT());
//    connect(port, SIGNAL(readyRead()), this, SLOT(readPort()));
    for (int i = 0; i <= 100; i++ ) {
            qDebug() << " " << i;
        }
}

