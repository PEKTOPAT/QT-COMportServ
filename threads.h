#ifndef THREADS_H
#define THREADS_H

#include <QThread>
#include <QSerialPort>
//#include "generatedata.h"

class Threads : public QThread
{
public:
    explicit Threads();
    QSerialPort *port;

    void run();
private:
    //GenerateData *pwt;

private slots:
//    void openPort();
//    void closePort();
//    void setRate_slot(int rate);
//    void reset_Arduino();
//    void writePort(QByteArray data);
//   void readPort();


};

#endif // THREADS_H
