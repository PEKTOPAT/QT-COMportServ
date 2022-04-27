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
    flagRecieve_ch1 = true;
    flagRecieve_ch2 = true;
    flagMain = false;
    flagStopReceive = false;
    sizeInfo_ch1 = 20;
    sizeInfo_ch2 = 20;
    sizePackage = 23;
    countByte_CH1 = 0;
    countByte_CH2 = 0;
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
    ui->comboBox_speed_1->addItem("1,2");
    ui->comboBox_speed_1->addItem("2,4");
    ui->comboBox_speed_1->addItem("4,8");
    ui->comboBox_speed_2->addItem("1,2");
    ui->comboBox_speed_2->addItem("2,4");
    ui->comboBox_speed_2->addItem("4,8");

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
    connect(ui->push_clear_log, SIGNAL(clicked(bool)), this, SLOT(clear_Log()));
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
        flagRecieve_ch1 = true;
        flagRecieve_ch2 = true;
        Package_ch1.clear();
        Package_ch2.clear();
        sizeInfo_ch1 = 20;
        sizeInfo_ch2 = 20;
        sizePackage = 23;
    }
    else return;
}
//******************************************************************************
void GenerateData::openPatternFile()
{
    Pattern.clear();
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
    while(!in.atEnd())
    {
        Pattern.append(in.readLine());
    }
    return;
}
//******************************************************************************
void GenerateData::generatePackage()
{
    if(Pattern.size() == 0) return;
    Package_ch1.clear();
    Package_ch2.clear();
    QByteArray convert;
    convert = Pattern.toLocal8Bit();
    if(ui->checkBox_1->checkState())
    {
        if(ui->comboBox_speed_1->currentText() == "1,2")
        {
            Package_ch1.append(171);
            Package_ch1.append(33);
            Package_ch1.append(sizeInfo_ch1);
            for(int j = 0; j < sizeInfo_ch1; j++)
            {
                Package_ch1.append(convert.at(countByte_CH1));
                countByte_CH1++;
                if(countByte_CH1 > Pattern.length()) countByte_CH1 = 0;
            }
        }
        else if (ui->comboBox_speed_1->currentText() == "2,4")
        {
            Package_ch1.append(171);
            Package_ch1.append(34);
            Package_ch1.append(sizeInfo_ch1);
            for(int j = 0; j < sizeInfo_ch1; j++)
            {
                Package_ch1.append(convert.at(countByte_CH1));
                countByte_CH1++;
                if(countByte_CH1 > Pattern.length()) countByte_CH1 = 0;
            }
        }
        else if (ui->comboBox_speed_1->currentText() == "4,8")
        {
            Package_ch1.append(171);
            Package_ch1.append(35);
            Package_ch1.append(sizeInfo_ch1);
            for(int j = 0; j < sizeInfo_ch1; j++)
            {
                Package_ch1.append(convert.at(countByte_CH1));
                countByte_CH1++;
                if(countByte_CH1 > Pattern.length()) countByte_CH1 = 0;
            }
        }
    }
    if(ui->checkBox_2->checkState())
    {
        if(ui->comboBox_speed_2->currentText() == "1,2")
        {
            Package_ch2.append(171);
            Package_ch2.append(68);
            Package_ch2.append(sizeInfo_ch2);
            for(int j = 0; j < sizeInfo_ch2; j++)
            {
                Package_ch2.append(convert.at(countByte_CH2));
                countByte_CH2++;
                if(countByte_CH2 == Pattern.length()) countByte_CH2 = 0;
            }
        }
        else if (ui->comboBox_speed_2->currentText() == "2,4")
        {
            Package_ch2.append(171);
            Package_ch2.append(72);
            Package_ch2.append(sizeInfo_ch2);
            for(int j = 0; j < sizeInfo_ch2; j++)
            {
                Package_ch2.append(convert.at(countByte_CH2));
                countByte_CH2++;
                if(countByte_CH2 == Pattern.length()) countByte_CH2 = 0;
            }
        }
        else if (ui->comboBox_speed_2->currentText() == "4,8")
        {
            Package_ch2.append(171);
            Package_ch2.append(76);
            Package_ch2.append(sizeInfo_ch2);
            for(int j = 0; j < sizeInfo_ch2; j++)
            {
                Package_ch2.append(convert.at(countByte_CH2));
                countByte_CH2++;
                if(countByte_CH2 == Pattern.length()) countByte_CH2 = 0;
            }
        }
    }
}
//******************************************************************************
void GenerateData::sendPackage()
{
    flagStopReceive = false;
    if(Pattern.size() == 0)
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
    if(Package_ch1.size() != 0 && flagRecieve_ch1)
    {
        ui->label_statusPort_1->setText("Up");
        ui->label_statusPort_1->setStyleSheet("QLabel {font-weight: bold; color : green; }");
        writePort(Package_ch1);
    }
    if(Package_ch2.size() != 0 && flagRecieve_ch2)
    {
        ui->label_statusPort_3->setText("Up");
        ui->label_statusPort_3->setStyleSheet("QLabel {font-weight: bold; color : green; }");
        writePort(Package_ch2);
    }
    flagRecieve_ch1 = false;
    flagRecieve_ch2 = false;
    sizeInfo_ch1 = 15;
    sizeInfo_ch2 = 15;
    sizePackage = 18;
}
//******************************************************************************
void GenerateData::stopSendPackage()
{
    sizeInfo_ch1 = 20;
    sizeInfo_ch2 = 20;
    sizePackage = 23;
    flagStopReceive = true;
    ui->checkBox_1->setEnabled(true);
    ui->checkBox_2->setEnabled(true);
    ui->push_start_send->setEnabled(true);
    ui->comboBox_speed_1->setEnabled(true);
    ui->comboBox_speed_2->setEnabled(true);
    ui->label_statusPort_1->setText("Down");
    ui->label_statusPort_1->setStyleSheet("QLabel {font-weight: bold; color : red; }");
    ui->label_statusPort_3->setText("Down");
    ui->label_statusPort_3->setStyleSheet("QLabel {font-weight: bold; color : red; }");
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
void GenerateData::readPort()
{
    if(flagStopReceive) return;
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
        if(!flagMain && strData == "170")flagMain = true;
        else if (flagMain)
        {
            if(strData == "64")
            {
                flagMain = false;
            }
            else if(strData == "65")
            {
                flagRecieve_ch1 = true;
                sendPackage();
                flagMain = false;
            }
            else if(strData == "67")
            {
                flagMain = false;
            }
            else if(strData == "71")
            {
                flagMain = false;
            }
            else if(strData == "128")
            {
                flagMain = false;
            }
            else if(strData == "136")
            {
                flagRecieve_ch2 = true;
                sendPackage();
                flagMain = false;
            }
            else if(strData == "152")
            {
                flagMain = false;
            }
            else if(strData == "184")
            {
                flagMain = false;
            }
            else if(strData == "201")
            {

                flagRecieve_ch1 = true;
                flagRecieve_ch2 = true;
                sendPackage();
                flagMain = false;
            }
            else if(strData == "217")
            {
                flagRecieve_ch1 = true;
                sendPackage();
                flagMain = false;
            }
            else if(strData == "249")
            {
                flagRecieve_ch1 = true;
                sendPackage();
                flagMain = false;
            }
            else if(strData == "203")
            {

                flagRecieve_ch2 = true;
                sendPackage();
                flagMain = false;
            }
            else if(strData == "207")
            {
                flagRecieve_ch2 = true;
                sendPackage();
                flagMain = false;
            }
            else if((strData == "219")||(strData == "223")||(strData == "251")||(strData == "255"))
            {
                flagMain = false;
            }
            else
            {
                debugTextEdit(false, "Err read data!");
                flagMain = false;
            }
        }
        else
        {
            debugTextEdit(false, "Err recieve");
            return;
        }
    }
    return;
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
        flagRecieve_ch1 = true;
        flagRecieve_ch2 = true;
        Package_ch1.clear();
        Package_ch2.clear();
        sizeInfo_ch1 = 20;
        sizeInfo_ch2 = 20;
        sizePackage = 23;
    }
    else
    {
        debugTextEdit(false, "Reset err. No connect");
        return;
    }
}
//******************************************************************************
void GenerateData::clear_Log()
{
    ui->textEdit->clear();
}
//******************************************************************************
