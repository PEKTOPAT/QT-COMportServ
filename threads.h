#ifndef THREADS_H
#define THREADS_H

#include <QThread>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QFile>
#include <QTime>
#include <QTimer>
#include "generatedata.h"


class Threads : public QThread
{
public:
    explicit Threads();
private:
    void run();
    QSerialPort *port;
    QString Pattern;
    QByteArray Package_ch1;
    QByteArray Package_ch2;
    int sizeInfo_ch1;
    int sizeInfo_ch2;
    int countByte_CH1;
    int countByte_CH2;
    bool flagMain;
    bool flagRecieve_ch1;
    bool flagRecieve_ch2;
    bool flagStopReceive;
    int sizePackage;
    short shiftFreq;
    double correction_Freq;
    QTimer *timer_RefrashPort;

private slots:
    void openPort();
    void closePort();
    void setRate_slot(int rate);
    //    void reset_Arduino();
    //    void writePort(QByteArray data);
    //   void readPort();
        void test();


};

#endif // THREADS_H
