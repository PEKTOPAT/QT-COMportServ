#include "threads.h"
#include <QDebug>
#include <QFileDialog>
#include <QTime>
#include <QMessageBox>

Threads::Threads()

{
    flagRecieve_ch1 = true;
    flagRecieve_ch2 = true;
    flagMain = false;
    flagStopReceive = false;
    sizeInfo_ch1 = 50;
    sizeInfo_ch2 = 50;
    sizePackage = 53;
    countByte_CH1 = 0;
    countByte_CH2 = 0;
    shiftFreq = 0;
    correction_Freq = 0;
    timer_RefrashPort = new QTimer();
    timer_RefrashPort->start(3000);
    port = new QSerialPort(this);
    port->setDataBits(QSerialPort::Data8);
    port->setFlowControl(QSerialPort::NoFlowControl);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);
    port->setBaudRate(QSerialPort::Baud115200);

    //connect(ui->push_clear_log, SIGNAL(clicked(bool)), this, SLOT(test()));
}

void Threads::run()
{
    //    connect(port, SIGNAL(readyRead()), this, SLOT());
    //    connect(port, SIGNAL(readyRead()), this, SLOT(readPort()));
    //    for (int i = 0; i <= 100; i++ ) {
    //            qDebug() << " " << i;
    //        }
}

void Threads::test()
{
    for(int i = 0; i <= 50; i++)
    {
        qDebug() << "Thread_1" << i;
    }
}
//******************************************************************************
void Threads::setRate_slot(int rate)
{

    if(rate == 0)  port->setBaudRate(QSerialPort::Baud9600);
    else if (rate == 1) port->setBaudRate(QSerialPort::Baud19200);
    else if (rate == 2) port->setBaudRate(QSerialPort::Baud38400);
    else if (rate == 3) port->setBaudRate(QSerialPort::Baud57600);
    else if (rate == 4) port->setBaudRate(QSerialPort::Baud115200);
}
//******************************************************************************
void Threads::openPort()
{

    if(!port) return;
    if(port->isOpen()) port->close();

    //port->setPortName(ui->comboBox_port->currentText());
    port->open(QIODevice::ReadWrite);
    if(port->isOpen())
    {
        //debugTextEdit(true, "Connected");
//        ui->label_status->setText("Connected");
//        ui->label_status->setStyleSheet("QLabel {font-weight: bold; color : green; }");
//        ui->push_connect->setEnabled(false);
//        ui->push_disconnect->setEnabled(true);
//        ui->label_info->setText(ui->comboBox_port->currentText() +" @ "+ ui->comboBox_portSpeed->currentText());
//        ui->push_start_send->setEnabled(true);
//        ui->push_stop_send->setEnabled(true);
    }
    //else debugTextEdit(false, "Port not open!");
}
//******************************************************************************
//void GenerateData::closePort()
//{
//    if (! myThread->port) return;
//    if(myThread->port->isOpen())
//    {

//        myThread->port->close();
//        debugTextEdit(true, "Disconnected");
//        ui->label_status->setText("Disconnected");
//        ui->label_status->setStyleSheet("QLabel {font-weight: bold; color : red; }");
//        ui->push_connect->setEnabled(true);
//        ui->push_disconnect->setEnabled(false);
//        ui->checkBox_1->setEnabled(true);
//        ui->checkBox_2->setEnabled(true);
//        ui->push_start_send->setEnabled(true);
//        ui->comboBox_speed_1->setEnabled(true);
//        ui->comboBox_speed_2->setEnabled(true);
//        ui->label_statusPort_1->setText("Down");
//        ui->label_statusPort_1->setStyleSheet("QLabel {font-weight: bold; color : red; }");
//        ui->label_statusPort_3->setText("Down");
//        ui->label_statusPort_3->setStyleSheet("QLabel {font-weight: bold; color : red; }");
//        flagRecieve_ch1 = true;
//        flagRecieve_ch2 = true;
//        Package_ch1.clear();
//        Package_ch2.clear();
//        sizeInfo_ch1 = 50;
//        sizeInfo_ch2 = 50;
//        sizePackage = 53;
//    }
//    else return;
//}
