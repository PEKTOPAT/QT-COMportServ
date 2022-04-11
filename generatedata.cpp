#include "generatedata.h"
#include "ui_generatedata.h"

#include <QFileDialog>
#include <QDebug>
#include <QTime>
#include <QMessageBox>

//******************************************************************************

GenerateData::GenerateData(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GenerateData)
{
    ui->setupUi(this);
    setGeometry(250,250,435,360);
    ui->push_start_send->setDisabled(true);
    ui->push_stop_send->setDisabled(true);
    countByte_ch1 = 0;
    countByte_ch2 = 0;
    flagRecieve_ch1 = true;
    flagRecieve_ch2 = true;
    flagMain = false;
    int num_port = QSerialPortInfo::availablePorts().length();
    for(int i = 0; i < num_port; i++)
    {
        ui->comboBox_port->addItem(QSerialPortInfo::availablePorts().at(i).portName());
    }

    port = new QSerialPort(this);
//    ui->comboBox_portSpeed->addItem("9600");
//    ui->comboBox_portSpeed->addItem("19200");
//    ui->comboBox_portSpeed->addItem("38400");
//    ui->comboBox_portSpeed->addItem("57600");
    ui->comboBox_portSpeed->addItem("115200");
    ui->comboBox_speed_1->addItem("2,4");
    ui->comboBox_speed_1->addItem("4,8");
    ui->comboBox_speed_1->addItem("9,6");
    ui->comboBox_speed_2->addItem("2,4");
    ui->comboBox_speed_2->addItem("4,8");
    ui->comboBox_speed_2->addItem("9,6");

    ui->label_statusPort_1->setText("Down");
    ui->label_statusPort_1->setStyleSheet("QLabel {font-weight: bold; color : red; }");
    ui->label_statusPort_3->setText("Down");
    ui->label_statusPort_3->setStyleSheet("QLabel {font-weight: bold; color : red; }");

    port->setDataBits(QSerialPort::Data8);
    port->setFlowControl(QSerialPort::NoFlowControl);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);
    port->setBaudRate(QSerialPort::Baud115200);
    //**********
    //connecting
    //**********
    connect(ui->push_connect,SIGNAL(clicked()),this, SLOT(openPort()));
    connect(ui->push_disconnect,SIGNAL(clicked()),this, SLOT(closePort()));
    connect(ui->comboBox_portSpeed, SIGNAL(currentIndexChanged(int)), this, SLOT(setRate_slot(int)));
    connect(ui->push_download, SIGNAL(clicked(bool)), this, SLOT(openPatternFile()));
    connect(ui->push_reset_arduin, SIGNAL(clicked(bool)), this, SLOT(reset_Arduino()));
    connect(ui->push_start_send, SIGNAL(clicked(bool)), this, SLOT(sendPackage()));
    connect(ui->push_stop_send, SIGNAL(clicked(bool)), this, SLOT(stopSendPackage()));
    connect(port, SIGNAL(readyRead()), this, SLOT(readPort()));

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
        ui->push_start_send->setEnabled(true);
        ui->push_stop_send->setEnabled(true);
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
        ui->checkBox_1->setEnabled(true);
        ui->checkBox_2->setEnabled(true);
        ui->push_start_send->setEnabled(true);
        ui->comboBox_speed_1->setEnabled(true);
        ui->comboBox_speed_2->setEnabled(true);
        ui->label_statusPort_1->setText("Down");
        ui->label_statusPort_1->setStyleSheet("QLabel {font-weight: bold; color : red; }");
        ui->label_statusPort_3->setText("Down");
        ui->label_statusPort_3->setStyleSheet("QLabel {font-weight: bold; color : red; }");
        countByte_ch1 = 0;
        countByte_ch2 = 0;
        flagRecieve_ch1 = true;
        flagRecieve_ch2 = true;
        Package_ch1.clear();
        Package_ch2.clear();
    }
    else return;
}
//******************************************************************************
void GenerateData::openPatternFile()
{

    VPattern.clear();
    QString fileName = QFileDialog::getOpenFileName(this);
    if(fileName.isEmpty())
    {
        debugTextEdit(false, "File isEmpty");
        return;
    }
    QFile file(fileName);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        debugTextEdit(false, "File not open");
        return;
    }else debugTextEdit(true, "Control file load");
    QTextStream in(&file);
    QString line = in.readLine();
    VPattern.append(line);
    while(!in.atEnd())
    {
        QString line = in.readLine();
        VPattern[0].append(line);
    }
    file.close();
    return;
}
//******************************************************************************
void GenerateData::generatePackage()
{
    if(VPattern.size() == 0) return;
    Package_ch1.clear();
    Package_ch2.clear();
    sizeInfo_ch1 = 10;
    sizeInfo_ch2 = 10;
    QByteArray convert;
    convert.append(VPattern.at(0));
    if(ui->checkBox_1->checkState())
    {
        if(ui->comboBox_speed_1->currentText() == "2,4")
        {
            for(int i = 0; i < (VPattern[0].length())/sizeInfo_ch1; i++)
            {
                Package_ch1.append(171);
                Package_ch1.append(33);
                Package_ch1.append(sizeInfo_ch1);
                for(int j = 0; j < sizeInfo_ch1; j++)
                {
                    Package_ch1.append(convert.at(sizeInfo_ch1*i+j));
                }
            }
        }
        else if (ui->comboBox_speed_1->currentText() == "4,8")
        {
            for(int i = 0; i < (VPattern[0].length())/sizeInfo_ch1; i++)
            {
                Package_ch1.append(171);
                Package_ch1.append(34);
                Package_ch1.append(sizeInfo_ch1);
                for(int j = 0; j < sizeInfo_ch1; j++)
                {
                    Package_ch1.append(convert.at(sizeInfo_ch1*i+j));
                }
            }
        }
        else if (ui->comboBox_speed_1->currentText() == "9,6")
        {
            for(int i = 0; i < (VPattern[0].length())/sizeInfo_ch1; i++)
            {
                Package_ch1.append(171);
                Package_ch1.append(35);
                Package_ch1.append(sizeInfo_ch1);
                for(int j = 0; j < sizeInfo_ch1; j++)
                {
                    Package_ch1.append(convert.at(sizeInfo_ch1*i+j));
                }
            }
        }
    }
    if(ui->checkBox_2->checkState())
    {
        if(ui->comboBox_speed_2->currentText() == "2,4")
        {
            for(int i = 0; i < (VPattern[0].length())/sizeInfo_ch2; i++)
            {
                Package_ch2.append(171);
                Package_ch2.append(68);
                Package_ch2.append(sizeInfo_ch2);
                for(int j = 0; j < sizeInfo_ch2; j++)
                {
                    Package_ch2.append(convert.at(sizeInfo_ch2*i+j));
                }
            }
        }
        else if (ui->comboBox_speed_2->currentText() == "4,8")
        {
            for(int i = 0; i < (VPattern[0].length())/sizeInfo_ch2; i++)
            {
                Package_ch2.append(171);
                Package_ch2.append(72);
                Package_ch2.append(sizeInfo_ch2);
                for(int j = 0; j < sizeInfo_ch2; j++)
                {
                    Package_ch2.append(convert.at(sizeInfo_ch2*i+j));
                }
            }
        }
        else if (ui->comboBox_speed_2->currentText() == "9,6")
        {
            for(int i = 0; i < (VPattern[0].length())/sizeInfo_ch2; i++)
            {
                Package_ch2.append(171);
                Package_ch2.append(76);
                Package_ch2.append(sizeInfo_ch2);
                for(int j = 0; j < sizeInfo_ch2; j++)
                {
                    Package_ch2.append(convert.at(sizeInfo_ch2*i+j));
                }
            }
        }
    }
}
//******************************************************************************
void GenerateData::sendPackage()
{
    if(VPattern.size() == 0)
    {
        QMessageBox::critical(this, "Error", "File not loaded!");
        return;
    }
    generatePackage();
    ui->checkBox_1->setEnabled(false);
    ui->checkBox_2->setEnabled(false);
    ui->push_start_send->setEnabled(false);
    ui->comboBox_speed_1->setEnabled(false);
    ui->comboBox_speed_2->setEnabled(false);
    ui->push_download->setEnabled(false);
    partPackage_ch1.clear();
    partPackage_ch2.clear();
    qDebug() << Package_ch1.size() << flagRecieve_ch1;
    if(Package_ch1.size() != 0 && flagRecieve_ch1)
    {
        ui->label_statusPort_1->setText("Up");
        ui->label_statusPort_1->setStyleSheet("QLabel {font-weight: bold; color : green; }");
        for (int i = 0; i < 13; i++)
        {
            if(Package_ch1.size() == 13*countByte_ch1 + i)
            {
                countByte_ch1 = 0;
                partPackage_ch1.append(Package_ch1.at(13*countByte_ch1 + i));
            }
            else partPackage_ch1.append(Package_ch1.at(13*countByte_ch1 + i));
        }
        debugTextEdit(true, "Send on Ch1!");
        writePort(partPackage_ch1);
        countByte_ch1++;
    }
    if(Package_ch2.size() != 0 && flagRecieve_ch2)
    {
        ui->label_statusPort_3->setText("Up");
        ui->label_statusPort_3->setStyleSheet("QLabel {font-weight: bold; color : green; }");
        for (int i = 0; i < 13; i++)
        {
            if(Package_ch2.size() == 13*countByte_ch2 + i)
            {
                countByte_ch2 = 0;
                partPackage_ch2.append(Package_ch2.at(13*countByte_ch2 + i));
            }
            else partPackage_ch2.append(Package_ch2.at(13*countByte_ch2 + i));
        }
        debugTextEdit(true, "Send on Ch2!");
        writePort(partPackage_ch2);
        countByte_ch2++;
    }
    flagRecieve_ch1 = false;
    flagRecieve_ch2 = false;
}
//******************************************************************************
void GenerateData::stopSendPackage()
{
    ui->checkBox_1->setEnabled(true);
    ui->checkBox_2->setEnabled(true);
    ui->push_start_send->setEnabled(true);
    ui->comboBox_speed_1->setEnabled(true);
    ui->comboBox_speed_2->setEnabled(true);
    ui->label_statusPort_1->setText("Down");
    ui->label_statusPort_1->setStyleSheet("QLabel {font-weight: bold; color : red; }");
    ui->label_statusPort_3->setText("Down");
    ui->label_statusPort_3->setStyleSheet("QLabel {font-weight: bold; color : red; }");
    countByte_ch1 = 0;
    countByte_ch2 = 0;
    flagRecieve_ch1 = true;
    flagRecieve_ch2 = true;
    Package_ch1.clear();
    Package_ch2.clear();
}
//******************************************************************************
void GenerateData::debugTextEdit(bool status, QString debMSG)
{
    if(status) ui->textEdit->append(QTime::currentTime().toString("HH:mm:ss") + " -> " + debMSG);
    else ui->textEdit->append("<font color = red><\\font>" + QTime::currentTime().toString("HH:mm:ss") + " -> " + debMSG);
}
//******************************************************************************
void GenerateData::writePort(QByteArray data)
{

    port->write(data);
}
//******************************************************************************
void GenerateData::reset_Arduino()
{
    QByteArray msg;
    msg.append(170);
    if(port->isOpen())
    {
        writePort(msg);
        debugTextEdit(true, "Reset arduino");
        ui->checkBox_1->setEnabled(true);
        ui->checkBox_2->setEnabled(true);
        ui->push_start_send->setEnabled(true);
        ui->comboBox_speed_1->setEnabled(true);
        ui->comboBox_speed_2->setEnabled(true);
        ui->label_statusPort_1->setText("Down");
        ui->label_statusPort_1->setStyleSheet("QLabel {font-weight: bold; color : red; }");
        ui->label_statusPort_3->setText("Down");
        ui->label_statusPort_3->setStyleSheet("QLabel {font-weight: bold; color : red; }");
        countByte_ch1 = 0;
        countByte_ch2 = 0;
        flagRecieve_ch1 = true;
        flagRecieve_ch2 = true;
        Package_ch1.clear();
        Package_ch2.clear();
    }
    else
    {
        debugTextEdit(false, "Reset err. No connect");
        return;
    }
}
//******************************************************************************
void GenerateData::readPort()
{
    QByteArray data;
    QByteArray transit;
    if (port->bytesAvailable() == 0) return;
    data = port->readAll();
    for(int i = 0; i < data.size(); i++)
    {
        transit.clear();
        transit.append(data[i]);
        const QString tab = " ";
        QString strData;
        int intData = static_cast<quint8>(transit.at(0));
        for (int i = 0;i < transit.size();i++)
        {
            strData = strData+QString("%1").arg(intData)+tab;
        }
        strData.resize(strData.length() - 1);

        qDebug() << strData;
        debugTextEdit(true, strData);

        if(!flagMain && strData == "170")flagMain = true;
        else if (flagMain)
        {
            if(strData == "71")
            {
                flagMain = false;
                flagRecieve_ch1 = true;
            }
            else if(strData == "70")
            {
                flagMain = false;
                flagRecieve_ch1 = true;
            }
            else if(strData == "69")
            {
                flagMain = false;
                flagRecieve_ch1 = true;
            }
            else if(strData == "68")
            {
                flagRecieve_ch1 = true;
                sendPackage();
                flagMain = false;
            }
            else if(strData == "184")
            {
                flagMain = false;
                flagRecieve_ch2 = true;
            }
            else if(strData == "176")
            {
                flagMain = false;
                flagRecieve_ch2 = true;
            }
            else if(strData == "168")
            {
                flagMain = false;
                flagRecieve_ch2 = true;
            }
            else if(strData == "160")
            {
                flagRecieve_ch2 = true;
                sendPackage();
                flagMain = false;
            }
        }
        else
        {
            debugTextEdit(false, "Recieve err");
            return;
        }
    }
    return;
}
//******************************************************************************
