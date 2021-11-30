#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/qserialport.h>
#include <QtSerialPort/QSerialPortInfo>
#include <QComboBox>

#include <QMessageBox>
#include <QElapsedTimer>

#include <QToolBar>

QSerialPort *serialPort;
QString portNameSerial;
QElapsedTimer timer;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->geometry().width(), this->geometry().height());
    timer.start();
    cbListPort = new QComboBox();
    startToolbar();
    configSerial();
}

MainWindow::~MainWindow()
{
    if(serialPort->isOpen()){
        serialPort->close();
    }
    delete ui;
}

void MainWindow::configSerial(void){
    serialPort = new QSerialPort(this);
    serialPort->setPortName(portNameSerial);
    serialPort->setBaudRate(QSerialPort::Baud115200);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    connect(serialPort, SIGNAL(readyRead()), this, SLOT(onSerial()));
}

void MainWindow::startToolbar(void){
    actionConnect = ui->toolBar->addAction(QIcon(":/disconnect"), "Disconnected");
    //connect(actionConnect, &QAction::triggered, this, &MainWindow::openSerial);

    reload();
    ui->toolBar->addWidget(cbListPort);

    actionReload = ui->toolBar->addAction(QIcon(":/update"), "Reload");
    connect(actionReload, &QAction::triggered, this, &MainWindow::reload);

    ui->toolBar->addSeparator();
    QWidget* placeholderLog = new QWidget(this);
    ui->toolBar->insertWidget(new QAction(this), placeholderLog);
    ui->toolBar->addSeparator();

    QWidget* placeholderTable = new QWidget(this);
    ui->toolBar->insertWidget(new QAction(this), placeholderTable);

    ui->toolBar->addAction("PROGRAM")->setDisabled(1);
    actionClearProgram = ui->toolBar->addAction(QIcon(":/cleartable"), "Clear Program");
    //connect(actionClearTable, &QAction::triggered, this, &MainWindow::clearTable);

    actionSaveProgram = ui->toolBar->addAction(QIcon(":/save"), "Save Program");
    //connect(actionSaveTable, &QAction::triggered, this, &MainWindow::saveTable);
    ui->toolBar->addSeparator();

    ui->toolBar->addAction("LOG")->setDisabled(1);
    actionClearLog = ui->toolBar->addAction(QIcon(":/clearlog"), "Clear Log");
    //connect(actionClearLog, &QAction::triggered, this, &MainWindow::clearLog);

    actionSaveLog = ui->toolBar->addAction(QIcon(":/save"), "Save Log");
    //connect(actionSaveLog, &QAction::triggered, this, &MainWindow::saveLog);
}

void MainWindow::onSerial(){
    QTextCursor cursorText = ui->textEdit_log->textCursor();
    cursorText.movePosition(QTextCursor::End);
    ui->textEdit_log->setTextCursor(cursorText);
    QByteArray data;
    if(serialPort->canReadLine()){
        data = serialPort->readAll();
        ui->textEdit_log->insertPlainText(QString::fromStdString(data.toStdString()));
    }
}

void MainWindow::openSerial(void)
{
    if(serialPort->isOpen() == false){
       configSerial();
       if(!serialPort->open(QIODevice::ReadWrite)){
           QMessageBox problemOpenSerial;
               problemOpenSerial.setWindowTitle("Warning");
               problemOpenSerial.setText("Problem to open serial communication!");
               problemOpenSerial.setDefaultButton(QMessageBox::Ok);
               problemOpenSerial.show();
               problemOpenSerial.exec();
       }else{
        serialPort->open(QIODevice::ReadWrite);
        actionConnect->setIcon(QIcon(":/connect"));
       }
    }else{
        actionConnect->setIcon(QIcon(":/disconnect"));
        serialPort->close();
    }
}

QStringList MainWindow::getPorts(void){
    QStringList portList;
    const auto infos = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &info : infos){
        QString portName = info.portName();
        portList.append(portName);
    }
    return portList;
}

void MainWindow::reload(void){
    cbListPort->clear();
    portNameSerial.clear();
    QStringList portList = getPorts();
    for(int i = 0; i < portList.size(); i++){
        cbListPort->addItem(portList[i]);
        cbListPort->setCurrentIndex(i);
        portNameSerial = portList[i];
    }
    connect(actionConnect, &QAction::triggered, this, &MainWindow::openSerial);

}


void MainWindow::on_pushButton_control_play_clicked()
{
    QString value_text = ui->textEdit_program->toPlainText();
    serialPort->write(value_text.toUtf8());
}

