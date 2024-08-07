#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug() << "Setting up serial port";
    serial = new QSerialPort(this);
    // 配置串口参数
    serial->setPortName("/dev/tty.usbserial"); // 修改为你的串口设备名称
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    // 添加调试输出
    qDebug() << "Serial port configured";

    // 填充波特率选择框
    ui->comboBox_baudRate->addItem("9600", QSerialPort::Baud9600);
    ui->comboBox_baudRate->addItem("19200", QSerialPort::Baud19200);
    ui->comboBox_baudRate->addItem("38400", QSerialPort::Baud38400);
    ui->comboBox_baudRate->addItem("57600", QSerialPort::Baud57600);
    ui->comboBox_baudRate->addItem("115200", QSerialPort::Baud115200);
    ui->comboBox_baudRate->addItem("1500000", 1500000);

    // 设置默认波特率
    ui->comboBox_baudRate->setCurrentIndex(0);

    // 连接按钮信号到槽函数
    //connect(ui->pushButton_clear, &QPushButton::clicked, this, &MainWindow::on_pushButton_clear_clicked);

    // 打开串口
    if (serial->open(QIODevice::ReadWrite)) {
        connect(serial, &QSerialPort::readyRead, this, &MainWindow::readSerialData);
    } else {
        ui->textBrowser_log->append("Failed to open serial port");
    }
}

MainWindow::~MainWindow()
{
    serial->close();
    delete ui;
}

void MainWindow::on_pushButton_send_clicked()
{
    QString data = ui->lineEdit_send->text();
    serial->write(data.toUtf8());
    ui->textBrowser_log->append("Sent: " + data);
}

void MainWindow::readSerialData()
{
    QByteArray data = serial->readAll();
    ui->textBrowser_log->append("Received: " + QString(data));
}
/*
void MainWindow::on_comboBox_baudRate_currentIndexChanged(int index)
{
    QSerialPort::BaudRate baudRate = static_cast<QSerialPort::BaudRate>(
        ui->comboBox_baudRate->itemData(index).toInt());
    serial->setBaudRate(baudRate);
    ui->textBrowser_log->append("Baud rate set to: " + ui->comboBox_baudRate->currentText());
}
*/
void MainWindow::on_comboBox_baudRate_currentIndexChanged(int index)
{
    qDebug() << "Changing baud rate";
    int baudRate = ui->comboBox_baudRate->itemData(index).toInt();
    if (!serial->setBaudRate(baudRate)) {
        qDebug() << "Failed to set baud rate to:" << baudRate;
    } else {
        qDebug() << "Baud rate set to:" << baudRate;
    }
    ui->textBrowser_log->append("Baud rate set to: " + ui->comboBox_baudRate->currentText());
}
