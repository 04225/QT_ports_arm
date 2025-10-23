#include "mainwindow.h"
#include <QDebug>
#include <dirent.h>
#include <QMessageBox>
#include "ports.h"

int check = 0;
int stop_bit = 0;
int ports_speed=0;
int data_bit =0;
QString serialports="ok";
QString msg = "ok";
bool my_send_msg_button = false;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setGeometry(0, 0, 800, 680);
    //波特率
    comboBox_bitrate = new QComboBox(this);
    label_bitrate = new QLabel("波特率",this);
    label_bitrate->setGeometry(0,20,50,30);
    comboBox_bitrate->setGeometry(50, 20, 120, 30);
    for(size_t i = 0;i< sizeof (bitrate)/sizeof (bitrate[0]);i++)
    {
        comboBox_bitrate->addItem(QString::number(bitrate[i]));
    }
    //串口
    QVector<QString> ports = get_SerialPorts();//获取串口
    comboBox_ports = new QComboBox(this);
    label_ports = new QLabel(" 串口 ",this);
    comboBox_ports->setGeometry(50,50,120,30);
    label_ports->setGeometry(0,50,50,30);
    for(QVector<QString>::Iterator j=ports.begin();j != ports.end();j++)
    {
        comboBox_ports->addItem(*j);
    }
    //数据位
    comboBox_data_bit = new QComboBox(this);
    label_data_bit = new QLabel(this);
    label_data_bit->setGeometry(0,80,50,30);
    label_data_bit->setText("数据位");
    comboBox_data_bit->setGeometry(50,80,120,30);
    for(int j=5;j<=8;j++)
    {
        comboBox_data_bit->addItem(QString::number(j));
    }
    //停止位置
    comboBox_stop_bit = new QComboBox(this);
    label_stop_bit = new QLabel(this);
    label_stop_bit->setText("停止位");
    for (int k=1;k<=2 ;k++ ) {
        comboBox_stop_bit->addItem(QString::number(k));
    }
    label_stop_bit->setGeometry(0,120,120,30);
    comboBox_stop_bit->setGeometry(50,120,120,30);
    //校验位
    comboBox_check_bit = new QComboBox(this);
    label_check_bit = new QLabel(this);
    label_check_bit->setText("校验位");
    comboBox_check_bit->addItem("无");
    comboBox_check_bit->addItem("奇校验");
    comboBox_check_bit->addItem("偶校验");
    label_check_bit->setGeometry(0,150,120,30);
    comboBox_check_bit->setGeometry(50,150,120,30);
    //信息显示
    show_msg = new QPlainTextEdit(this);
    show_msg->setGeometry(200,20,400,400);
    show_msg->setReadOnly(true);
    show_msg->setWindowTitle("输出信息");
    //信息发送
    send_msg = new QPlainTextEdit(this);
    send_msg->setGeometry(200,480,400,80);
    send_msg->setReadOnly(false);
    send_msg->setWindowTitle("发送信息");

    send_msg_button = new QPushButton(this);
    send_msg_button->setText("发送信息");
    send_msg_button->setGeometry(200,440,80,20);
    clear_msg_button =new QPushButton(this);
    clear_msg_button->setText("清空接收区");
    clear_msg_button->setGeometry(300,440,80,20);

    open_ports_button = new QPushButton(this);
    open_ports_button->setText("打开串口");
    open_ports_button->setGeometry(50,180,120,30);


    connect(comboBox_bitrate,SIGNAL(currentIndexChanged(int)),
            this,SLOT(comboBoxIndexChanged_bitrate(int)));
    connect(comboBox_ports,SIGNAL(currentIndexChanged(int)),
            this,SLOT(comboBoxIndexChanged_ser(int)));
    connect(clear_msg_button,SIGNAL(clicked()),
            this,SLOT(clear_msg_clicked()));
    connect(send_msg_button,SIGNAL(clicked()),
            this,SLOT(send_msg_clicked()));
    connect(comboBox_data_bit,SIGNAL(currentIndexChanged(int)), //数据位
            this,SLOT(comboBoxIndexChanged_data(int)));
    connect(comboBox_check_bit,SIGNAL(currentIndexChanged(int)),//校验位
            this,SLOT(comboBoxIndexChanged_check(int)));
    connect(comboBox_stop_bit,SIGNAL(currentIndexChanged(int)),//停止位
            this,SLOT(comboBoxIndexChanged_stop(int)));
    connect(open_ports_button,SIGNAL(clicked()),
            this,SLOT(open_ports_button_clicked()));

    connect(send_msg_button,SIGNAL(pressed()),
            this,SLOT(my_send_msg_button_pressed()));
    connect(send_msg_button,SIGNAL(released()),
            this,SLOT(my_send_msg_button_released()));
//    connect(send_msg_button,SIGNAL(clicked()),
//            this,SLOT())
}

bool MainWindow::isSerialPort(const ::QString &filename)//传入string是否含有串口
{
    if(filename.length() >= 6)
    {
        if(filename.mid(0,6) == "ttyUSB" ||filename.mid(0,6) == "ttyACM")
        {
            return true;
        }
    }
    if(filename.length() >= 4)
    {
        if(filename.mid(0,4) == "ttyS")
        {
            return true;
        }
    }
    return false;
}

//获取串口Vector
QVector<QString> MainWindow::get_SerialPorts()
{
    QVector<QString> SerialPorts;
    DIR* dir = nullptr;
    struct dirent* entry = nullptr;

    dir = opendir("/dev");
    if(dir == nullptr)
    {
        qDebug()<<"无法打开/dev目录"<<endl;
        return SerialPorts;
    }
    while ((entry = readdir(dir)) != nullptr) {
        QString filename = entry->d_name;
        if(isSerialPort(filename))
        {
            SerialPorts.push_back("/dev/"+filename);
        }
    }
    closedir(dir);
    return SerialPorts;
}


MainWindow::~MainWindow()
{
}

void MainWindow::comboBoxIndexChanged_bitrate(int index_bitrate)//波特率
{
    ports_speed = index_bitrate;
    show_msg->appendPlainText(QString::number(ports_speed));
}

void MainWindow::comboBoxIndexChanged_ser(int index_ports)//串口
{
    QString info = comboBox_ports->itemText(index_ports);
    serialports = info;
    show_msg->appendPlainText(serialports);
}

void MainWindow::clear_msg_clicked()//清空信息
{
    show_msg->clear();
}

void MainWindow::send_msg_clicked()
{
    QString input_msg = send_msg->toPlainText();
    show_msg->appendPlainText(input_msg);
    msg = input_msg;
    send_msg->clear();
}

void MainWindow::comboBoxIndexChanged_data(int index)//数据位
{
    bool ok;
    int input_data =comboBox_data_bit->itemText(index).toInt(&ok);
    if(ok)
    {
        data_bit = input_data;
        show_msg->appendPlainText(QString::number(data_bit));
    }
    else {
        QMessageBox::information(NULL,"error","00001");
    }
}

void MainWindow::comboBoxIndexChanged_check(int index)//校验位
{
    if(index == 0)
    {
        check = 0;
        show_msg->appendPlainText(QString::number(check));
    }
    else if (index == 1) {
        check = 1;
        show_msg->appendPlainText(QString::number(check));
    }
    else if (index == 2) {
        check = 2;
        show_msg->appendPlainText(QString::number(check));
    }
    else {
        QMessageBox::information(NULL,"error","0002");
    }
}

void MainWindow::comboBoxIndexChanged_stop(int index)//停止位
{
    bool ok;
    int input_stop =comboBox_stop_bit->itemText(index).toInt(&ok);
    if(ok)
    {
        //show_msg->appendPlainText(QString::number(input_stop));
        stop_bit = input_stop;
        show_msg->appendPlainText(QString::number(stop_bit));
    }
    else {
        QMessageBox::information(NULL,"error","0003");
    }
}

void MainWindow::open_ports_button_clicked()  //打开串口
{
    open_ports(show_msg,this);
}

void MainWindow::my_send_msg_button_pressed()
{
    my_send_msg_button = true;
    m_controlThread->send_msg_thread(msg);  // 传递msg
    show_msg->appendPlainText("发送完成");
}

void MainWindow::my_send_msg_button_released()
{
    my_send_msg_button = false;
    m_controlThread->requestInterruption();
}
