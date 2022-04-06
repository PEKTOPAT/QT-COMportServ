#include "generatedata.h"
#include "ui_generatedata.h"

#include <QFileDialog>
#include <QDebug>
#include <QTime>

//******************************************************************************

GenerateData::GenerateData(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GenerateData)
{
    ui->setupUi(this);

    int num_port = QSerialPortInfo::availablePorts().length();
    for(int i = 0; i < num_port; i++)
    {
        ui->comboBox_port->addItem(QSerialPortInfo::availablePorts().at(i).portName());
    }

    port = new QSerialPort(this);
    ui->comboBox_portSpeed->addItem("9600");
    ui->comboBox_portSpeed->addItem("19200");
    ui->comboBox_portSpeed->addItem("38400");
    ui->comboBox_portSpeed->addItem("57600");
    ui->comboBox_portSpeed->addItem("115200");
    ui->comboBox_speed_1->addItem("2,4");
    ui->comboBox_speed_1->addItem("4,8");
    ui->comboBox_speed_1->addItem("9,6");
    ui->comboBox_speed_2->addItem("2,4");
    ui->comboBox_speed_2->addItem("4,8");
    ui->comboBox_speed_2->addItem("9,6");



    port->setDataBits(QSerialPort::Data8);
    port->setFlowControl(QSerialPort::NoFlowControl);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);
    //**********
    //connecting
    //**********
    connect(ui->push_connect,SIGNAL(clicked()),this, SLOT(openPort()));
    connect(ui->push_disconnect,SIGNAL(clicked()),this, SLOT(closePort()));
    connect(ui->comboBox_portSpeed, SIGNAL(currentIndexChanged(int)), this, SLOT(setRate_slot(int)));
}

GenerateData::~GenerateData()
{
    delete ui;
}
//******************************************************************************
//******************************************************************************
//******************************************************************************
void GenerateData::setRate_slot(int rate)
{
    if(rate == 0) port->setBaudRate(QSerialPort::Baud9600);
    else if (rate == 1) port->setBaudRate(QSerialPort::Baud19200);
    else if (rate == 2) port->setBaudRate(QSerialPort::Baud38400);
    else if (rate == 3) port->setBaudRate(QSerialPort::Baud57600);
    else if (rate == 4) port->setBaudRate(QSerialPort::Baud115200);
}
//******************************************************************************
void GenerateData::openPort()
{
    if(!port) return;
    if(port->isOpen()) port->close();

    port->setPortName(ui->comboBox_port->currentText());
    port->open(QIODevice::ReadWrite);
    if(port->isOpen())
    {
        debugTextEdit(true, "Connected");
        ui->label_status->setText("Connected");
        ui->label_status->setStyleSheet("QLabel {font-weight: bold; color : green; }");
        ui->push_connect->setEnabled(false);
        ui->push_disconnect->setEnabled(true);
        ui->label_info->setText(ui->comboBox_port->currentText() +" @ "+ ui->comboBox_portSpeed->currentText());
    }
    else debugTextEdit(false, "Port not open!");
}
//******************************************************************************
void GenerateData::closePort()
{
    if (! port) return;
    if(port->isOpen())
    {

        port->close();
        debugTextEdit(true, "Disconnected");
        ui->label_status->setText("Disconnected");
        ui->label_status->setStyleSheet("QLabel {font-weight: bold; color : red; }");
        ui->push_connect->setEnabled(true);
        ui->push_disconnect->setEnabled(false);
    }
    else return;
}
//******************************************************************************
void GenerateData::debugTextEdit(bool status, QString debMSG)
{
    if(status) ui->textEdit->append(QTime::currentTime().toString("HH:mm:ss") + " -> " + debMSG);
    else ui->textEdit->append("<font color = red><\\font>" + QTime::currentTime().toString("HH:mm:ss") + " -> " + debMSG);
}
