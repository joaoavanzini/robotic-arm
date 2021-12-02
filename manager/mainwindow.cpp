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
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>

QSerialPort *serialPort;
QString portNameSerial;
QElapsedTimer timer;

QString json_final;

QString content_m1;
QString content_m2;
QString content_m3;
QString content_m4;
QString content_m5;
QString content_m6;
QString content_m7;
QString content_m8;
QString content_m9;
QString content_m10;

int program_visible = 1;

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
    connect(actionClearProgram, &QAction::triggered, this, &MainWindow::clearProgram);

    actionSaveProgram = ui->toolBar->addAction(QIcon(":/save"), "Save Program");
    connect(actionSaveProgram, &QAction::triggered, this, &MainWindow::saveProgram);
    ui->toolBar->addSeparator();

    ui->toolBar->addAction("LOG")->setDisabled(1);
    actionClearLog = ui->toolBar->addAction(QIcon(":/clearlog"), "Clear Log");
    connect(actionClearLog, &QAction::triggered, this, &MainWindow::clearLog);

    actionSaveLog = ui->toolBar->addAction(QIcon(":/save"), "Save Log");
    connect(actionSaveLog, &QAction::triggered, this, &MainWindow::saveLog);
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
        ui->textEdit_information->clear();
        ui->textEdit_information->insertPlainText("Connected to robot!");
       }
    }else{
        actionConnect->setIcon(QIcon(":/disconnect"));
        serialPort->close();
        ui->textEdit_information->clear();
        ui->textEdit_information->insertPlainText("Disconnected to robot!");
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

void MainWindow::clearLog(void){
    ui->textEdit_log->clear();
}

void MainWindow::saveLog(void){
    QString path = QFileDialog::getSaveFileName(this, "Save File", "robot-log", "txt (*.txt *.doc *.docx)");
    if(path.isEmpty() == false){
        QFile file;
        file.setFileName(path);
        bool ret = file.open(QIODevice::WriteOnly);
        if(ret){
            QString str = ui->textEdit_log->toPlainText();
            file.write(str.toUtf8());
            file.close();
        }else{
            QMessageBox savelogMessage;
            savelogMessage.setWindowTitle("Warning");
            savelogMessage.setText("Problem to write the file!");
            savelogMessage.setDefaultButton(QMessageBox::Ok);
            savelogMessage.show();
            savelogMessage.exec();
        }
    }else{
        QMessageBox savelogMessageCancel;
        savelogMessageCancel.setWindowTitle("Warning");
        savelogMessageCancel.setText("Operation canceled!");
        savelogMessageCancel.setDefaultButton(QMessageBox::Ok);
        savelogMessageCancel.show();
        savelogMessageCancel.exec();
    }
}

void MainWindow::clearProgram(){
    ui->textEdit_program->clear();    
    program_visible = 0;
}

void MainWindow::saveProgram(){
    QString path = QFileDialog::getSaveFileName(this, "Save File", "robot-program", "rbt (*.rbt)");
    if(path.isEmpty() == false){
        QFile file;
        file.setFileName(path);
        bool ret = file.open(QIODevice::WriteOnly);
        if(ret){
            QString str = ui->textEdit_program->toPlainText();
            file.write(str.toUtf8());
            file.close();
        }else{
            QMessageBox savelogMessage;
            savelogMessage.setWindowTitle("Warning");
            savelogMessage.setText("Problem to write the file!");
            savelogMessage.setDefaultButton(QMessageBox::Ok);
            savelogMessage.show();
            savelogMessage.exec();
        }
    }else{
        QMessageBox savelogMessageCancel;
        savelogMessageCancel.setWindowTitle("Warning");
        savelogMessageCancel.setText("Operation canceled!");
        savelogMessageCancel.setDefaultButton(QMessageBox::Ok);
        savelogMessageCancel.show();
        savelogMessageCancel.exec();
    }
}

QString MainWindow::jog_list(QString pos1, QString pos2, QString pos3){
    return "[" + pos1 + "," + pos2 + "," + pos3 + "]";
}

void MainWindow::on_pushButton_control_play_clicked()
{
    QString value_text_program = ui->textEdit_program->toPlainText().replace("\n", " ");
    QStringList splitted = value_text_program.split(" ");


    if (splitted[0] == "--" && splitted[1] == "start" && splitted[2] == "movement"){

        if(splitted[4] == "-" && splitted[9] == "-" && splitted[14] == "-" && splitted[19] == "-" && splitted[24] == "-"  && splitted[29] == "-" && splitted[34] == "-"){
            splitted[5] = '"' + splitted[5];
            splitted[10] = '"' + splitted[10];
            splitted[15] = '"' + splitted[15];
            splitted[20] = '"' + splitted[20];
            splitted[25] = '"' + splitted[25];
            splitted[30] = '"' + splitted[30];
            splitted[35] = '"' + splitted[35];

            if (splitted[39] == "--" && splitted[40] == "end" && splitted[41] == "movement" && (splitted[3] == splitted[42])){
                json_final = "{" + splitted[5] + '"' + ":" + jog_list(splitted[6], splitted[7], splitted[8]) + ',' +
                        splitted[10] + '"' + ":" + jog_list(splitted[11], splitted[12], splitted[13]) + ',' +
                        splitted[15] + '"' + ":" + jog_list(splitted[16], splitted[17], splitted[18]) + ',' +
                        splitted[20] + '"' + ":" + jog_list(splitted[21], splitted[22], splitted[23]) + ',' +
                        splitted[25] + '"' + ":" + jog_list(splitted[26], splitted[27], splitted[28]) + ',' +
                        splitted[30] + '"' + ":" + jog_list(splitted[31], splitted[32], splitted[33]) + ',' +
                        splitted[35] + '"' + ":" + jog_list(splitted[36], splitted[37], splitted[38]) + '}';

                ui->textEdit_information->clear();
                ui->textEdit_information->insertPlainText("Sending " + splitted[3] + "...\n");
                ui->textEdit_log->insertPlainText(">>" + json_final.toUtf8() + "\n");

                if(serialPort->isOpen()){
                    if(serialPort->isWritable()){
                        ui->textEdit_information->insertPlainText("Executing " + splitted[3] + "...\n");
                        serialPort->write(json_final.toUtf8());
                        ui->textEdit_information->insertPlainText("Shutting down " + splitted[3] + "...\n");
                    }else{
                           ui->textEdit_information->insertPlainText("Serial port is no writable!\n");
                        }
                }else{
                    ui->textEdit_information->insertPlainText("Serial port is not open!\n");
                 }
            }else{
                ui->textEdit_information->clear();
                ui->textEdit_information->insertPlainText("Wrong program formation!\n");
             }
        }else{
            ui->textEdit_information->clear();
            ui->textEdit_information->insertPlainText("Wrong program formation!\n");
         }
    }else{
        ui->textEdit_information->clear();
       ui->textEdit_information->insertPlainText("Wrong program formation!\n");
    }

}


void MainWindow::on_pushButton_home_clicked()
{
    ui->textEdit_program->insertPlainText("-- start movement <name>\n- <jog> <angle> <vel> <complete>\n- <jog> <angle> <vel> <complete>\n- <jog> <angle> <vel> <complete>\n-- end movement <name>");
}


void MainWindow::on_pushButton_move_1_clicked()
{
    if(program_visible == 1){
        content_m1 = ui->textEdit_program->toPlainText();
    }

    if (program_visible == 0){
        ui->textEdit_program->setText(content_m1);
        program_visible = 1;
    }
}


void MainWindow::on_pushButton_move_2_clicked()
{
    if(program_visible == 1){
        content_m2 = ui->textEdit_program->toPlainText();
    }

    if (program_visible == 0){
        ui->textEdit_program->setText(content_m2);
        program_visible = 1;
    }
}


void MainWindow::on_pushButton_move_3_clicked()
{
    if(program_visible == 1){
        content_m3 = ui->textEdit_program->toPlainText();
    }

    if (program_visible == 0){
        ui->textEdit_program->setText(content_m3);
        program_visible = 1;
    }
}


void MainWindow::on_pushButton_move_4_clicked()
{
    if(program_visible == 1){
        content_m4 = ui->textEdit_program->toPlainText();
    }

    if (program_visible == 0){
        ui->textEdit_program->setText(content_m4);
        program_visible = 1;
    }
}


void MainWindow::on_pushButton_move_5_clicked()
{
    if(program_visible == 1){
        content_m5 = ui->textEdit_program->toPlainText();
    }

    if (program_visible == 0){
        ui->textEdit_program->setText(content_m5);
        program_visible = 1;
    }

}


void MainWindow::on_pushButton_move_6_clicked()
{
    if(program_visible == 1){
        content_m6 = ui->textEdit_program->toPlainText();
    }

    if (program_visible == 0){
        ui->textEdit_program->setText(content_m6);
        program_visible = 1;
    }
}


void MainWindow::on_pushButton_move_7_clicked()
{
    if(program_visible == 1){
        content_m7 = ui->textEdit_program->toPlainText();
    }

    if (program_visible == 0){
        ui->textEdit_program->setText(content_m7);
        program_visible = 1;
    }
}


void MainWindow::on_pushButton_move_8_clicked()
{
    if(program_visible == 1){
        content_m8 = ui->textEdit_program->toPlainText();
    }

    if (program_visible == 0){
        ui->textEdit_program->setText(content_m8);
        program_visible = 1;
    }
}


void MainWindow::on_pushButton_move_9_clicked()
{
    if(program_visible == 1){
        content_m9 = ui->textEdit_program->toPlainText();
    }

    if (program_visible == 0){
        ui->textEdit_program->setText(content_m9);
        program_visible = 1;
    }
}


void MainWindow::on_pushButton_move_10_clicked()
{
    if(program_visible == 1){
        content_m10 = ui->textEdit_program->toPlainText();
    }

    if (program_visible == 0){
        ui->textEdit_program->setText(content_m10);
        program_visible = 1;
    }
}


void MainWindow::on_textEdit_program_textChanged()
{
    if(ui->textEdit_program->toPlainText() != ""){
        program_visible = 1;
    }
}

