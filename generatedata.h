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

private slots:
    void openPort();
    void closePort();
    void setRate_slot(int rate);
//    void reset_Arduino();
//    void writePort(QByteArray data);
//    QByteArray readPort();
    void debugTextEdit(bool status, QString debMSG);

};

#endif // GENERATEDATA_H
