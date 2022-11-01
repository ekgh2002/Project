#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort> // Add
#include <qserialportinfo.h>        // Add
#include <stdio.h>                  // Add
#include <QMessageBox>              // Add
#include <QDebug>                   // Add
#include <QApplication>
#include <QWidget>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QSerialPort *port;  //AddQT
    //-----chart start-----//
    void add_point(double x, double y);
    void add_point2(double x2, double y2);
    void add_point3(double x3, double y3);
    void add_point4(double x4, double y4);
    void plot(int select);
    void plot2(int select2);
    void plot3(int select3);
    void plot4(int select4);
    //-----chart end-------//
private slots:
    void text_reading();

    void on_pushButton_scanagain_clicked();

    void on_pushButton_open_clicked();

    void on_pushButton_close_clicked();

    void on_pushButton_send_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_dht11_clicked();

    void on_checkBox_stateChanged(int arg1);



    void on_pushButton_clear_tmp_clicked();

    void on_pushButton_clear_wet_clicked();

    void alive();

private:
    Ui::MainWindow *ui;
    //----chart start
    QVector <double> qv_x, qv_y;     //  x, y 축 좌표값
    QVector <double> qv_x2, qv_y2;
    QVector <double> qv_x3, qv_y3;
    QVector <double> qv_x4, qv_y4;
    QString temp;                    //  온도
    QString wet;
    double max_temp;                 //  최고 온도
    double max_wet;
    double min_temp;                 //  최저 온도
    double min_wet;
    double time = 0;                 //  시간축(x)을 가리키는 변수
    double time2 = 0;
    double time3 = 0;
    int temp_min = 200;
    int temp_max = 0;
    int wet_min = 200;
    int wet_max = 0;
    int bar = 0;
/*
    QVector <int> v {1, 2, void MainWindow::alive()3, 4};    동적 메모리 할당
 */
    //----chart end
};
#endif // MAINWINDOW_H
