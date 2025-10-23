#ifndef PORTS_H
#define PORTS_H
#include <QString>
#include <QPlainTextEdit>
#include <QThread>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QThread>
#include "mainwindow.h"

extern int ports_speed; //波特率
extern int data_bit;
extern int stop_bit;
extern QString serialports;
extern int check;
extern QString msg;

extern bool my_send_msg_button;


void open_ports(QPlainTextEdit *show_windows,MainWindow *mainwindow);

//线程1-----------------------------------------------
class SerialThread : public QThread
{
    Q_OBJECT

public:
    SerialThread(QSerialPort *port, QObject *parent = nullptr);

    void run() override;

signals:
    void dataReceived(const QByteArray &data);

private:
    QSerialPort *m_port = nullptr;
};

//线程2-----------------------------------------------
class ControlThread : public QThread
{
    Q_OBJECT
public:
    ControlThread(QSerialPort *port, QObject *parent = nullptr);
    void send_msg_thread(QString input_msg_a);
    void run() override;

private:
   QSerialPort *m_port = nullptr;
   QByteArray input_msg;
};



#endif // PORTS_H
