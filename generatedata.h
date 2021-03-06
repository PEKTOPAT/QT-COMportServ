#ifndef GENERATEDATA_H
#define GENERATEDATA_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QFile>
#include <QTime>
#include <QTimer>

namespace Ui {
class GenerateData;
}

class GenerateData : public QMainWindow
{
    Q_OBJECT

public:
    explicit GenerateData(QWidget *parent = 0);
    ~GenerateData();

private:
    Ui::GenerateData *ui;
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
    int num_port;

private slots:
    void openPort();
    void closePort();
    void setRate_slot(int rate);
    void reset_Arduino();
    void writePort(QByteArray data);
    void readPort();
    void debugTextEdit(bool status, QString debMSG);
    void openPatternFile();
    void generatePackage();
    void sendPackage();
    void stopSendPackage();
    void clear_Log();
    void setShiftFreq(int value);
    void correctionFreq();
    void refrashPort();
};

#endif // GENERATEDATA_H
