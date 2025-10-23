#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QLabel>
#include <dirent.h>
#include <QString>
#include <string.h>
#include <QVector>
#include <QLabel>
#include <QStatusBar>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <QPlainTextEdit>
#include <QPushButton>


class ControlThread;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QPlainTextEdit *show_msg;//显示窗口
    ControlThread* m_controlThread = nullptr;

private:
    QComboBox *comboBox_bitrate;
    QLabel *label_bitrate;//波特率标签
    int bitrate[8]={1200,2400,4800,9600,19200,38400,57600,115200};

    QLabel *label_ports;//串口标签
    QComboBox *comboBox_ports;

    QLabel *label_data_bit;//数据位
    QComboBox *comboBox_data_bit;

    QLabel *label_stop_bit;//停止位置
    QComboBox *comboBox_stop_bit;

    QLabel *label_check_bit;//校验位
    QComboBox *comboBox_check_bit;

    bool isSerialPort(const::QString &filename);  //匹配串口
    QVector<QString> get_SerialPorts();//获取串口--封装到vector



    QPlainTextEdit *send_msg;//发送窗口

    QPushButton *send_msg_button;  //发送按钮
    QPushButton *clear_msg_button;//清空msg

    QPushButton *open_ports_button;//打开串口



private slots:
    void comboBoxIndexChanged_bitrate(int);//选择波特率
    void comboBoxIndexChanged_ser(int);//串口改变
    void clear_msg_clicked();
    void send_msg_clicked();
    void comboBoxIndexChanged_data(int);//数据位
    void comboBoxIndexChanged_check(int);//校验位置
    void comboBoxIndexChanged_stop(int);//停止位
    void open_ports_button_clicked();
    void my_send_msg_button_pressed();//跟新按钮的状态
    void my_send_msg_button_released();
};
#endif // MAINWINDOW_H
