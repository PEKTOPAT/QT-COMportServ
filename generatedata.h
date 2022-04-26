#ifndef GENERATEDATA_H
#define GENERATEDATA_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QFile>

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
    QByteArray partPackage_ch1;
    QByteArray partPackage_ch2;
    int countByte_ch1;
    int countByte_ch2;
    bool flagMain;
    bool flagRecieve_ch1;
    bool flagRecieve_ch2;
    bool flagStopReceive;
    int sizePackage;

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
};

#endif // GENERATEDATA_H
