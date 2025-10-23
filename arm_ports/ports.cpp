#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "ports.h"
#include <QString>
#include <QMessageBox>
#include <QMap>
#include <QDebug>
#include "mainwindow.h"

QSerialPort ser;
//线程1--------------------------------------------------------
SerialThread::SerialThread(QSerialPort *port, QObject *parent)
    :QThread(parent),m_port(port)
            {}
void SerialThread::run()
{
    while (!isInterruptionRequested()) {
        if (m_port->waitForReadyRead(100)) {
            QByteArray data = m_port->readAll();
            emit dataReceived(data);
        }
    }
}
//线程2----------------------------------------------------------
ControlThread::ControlThread(QSerialPort *port, QObject *parent)
        :QThread(parent), m_port(port)
        {}

void ControlThread::send_msg_thread(QString input_msg_a)
{
    this->input_msg = input_msg_a.toUtf8();
}
void ControlThread::run()
{
    while (!isInterruptionRequested()) {//线程未收到中断请求
                QThread::msleep(200);
                QByteArray cmd = input_msg;
                m_port->write(cmd);
                m_port->waitForBytesWritten();
                qDebug()<<"发送完成"<<endl;
            }
}

void open_ports(QPlainTextEdit *show_windows,MainWindow *mainwindow)
{
    show_windows->appendPlainText("开始传");
    QString m_name =serialports;//串口名字
    QMap<int,QSerialPort::BaudRate> baudMap={
        {0,QSerialPort::Baud1200},
        {1,QSerialPort::Baud2400},
        {2,QSerialPort::Baud4800},
        {3,QSerialPort::Baud9600},
        {4,QSerialPort::Baud19200},
        {5,QSerialPort::Baud38400},
        {6,QSerialPort::Baud57600},
        {7,QSerialPort::Baud115200},
    };
    QMap<int,QSerialPort::DataBits> data_map={
        {0,QSerialPort::Data5},
        {1,QSerialPort::Data6},
        {2,QSerialPort::Data7},
        {3,QSerialPort::Data8},
    };
    QMap<int,QSerialPort::Parity> check_map={//校验位
        {0,QSerialPort::NoParity},
        {1,QSerialPort::OddParity},
        {2,QSerialPort::EvenParity}
    };

    ser.setPortName(m_name);  //串口名字
    ser.setBaudRate(baudMap[ports_speed]);//波特率
    ser.setParity(check_map[check]);//校验位
    ser.setDataBits(data_map[data_bit]);//数据位

    if(stop_bit == 1)     //停止位
    {
        ser.setStopBits(QSerialPort::OneStop);
        show_windows->appendPlainText("1位");
    }
    else {
        ser.setStopBits(QSerialPort::TwoStop);
        show_windows->appendPlainText("2位");
    }

    if(!ser.open(QIODevice::ReadWrite))
    {
        QMessageBox::information(NULL,"error","串口打开失败");
    }

    SerialThread *my_serialThread = new SerialThread(&ser, show_windows);
    ControlThread *my_controlThread = new ControlThread(&ser, show_windows);
    mainwindow->m_controlThread = my_controlThread;  //--------------------1

    my_serialThread->start();
    my_controlThread->start();

    QObject::connect(my_serialThread,&SerialThread::dataReceived,
                     show_windows,
                     [=](const QByteArray &data){
        show_windows->appendPlainText(QString::fromUtf8(data));
    },Qt::QueuedConnection);
//    QObject::connect(&my_controlThread,&MainWindow::s

}
