#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDialog>
#include "qcustomplot.h"    // add chart



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //-------------- setting speed ------------//
    ui->comboBox_speed->addItem("9600", QSerialPort::Baud9600);
    ui->comboBox_speed->addItem("115200", QSerialPort::Baud115200);

    //-------------- setting databit ----------//
    ui->comboBox_databit->addItem("8", QSerialPort::Data8);
    ui->comboBox_databit->addItem("7", QSerialPort::Data7);

    //-------------- setting parity bits ------//
    ui->comboBox_parity->addItem("None", QSerialPort::NoParity);
    ui->comboBox_parity->addItem("Even", QSerialPort::EvenParity);
    ui->comboBox_parity->addItem("Odd", QSerialPort::OddParity);
    //-------------- setting dht11 control-----//
//    ui->comboBox_dht11->addItem("dht11time1500");
//    ui->comboBox_dht11->addItem("dht11time2000");
//    ui->comboBox_dht11->addItem("dht11time3000");


    port = new QSerialPort(this);
    QObject::connect(port, SIGNAL(readyRead()), this, SLOT(text_reading()));
    on_pushButton_scanagain_clicked();

    //--------------- Add chart start
    ui->plot->setInteraction(QCP::iRangeDrag, true);
    ui->plot->setInteraction(QCP::iRangeZoom, true);
    ui->plot->addGraph();
    ui->plot->yAxis->setLabel("온도");
    ui->plot->yAxis->setRange(0.0, 40.0);
    ui->plot->xAxis->setLabel("시간");
    ui->plot->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);  //점 찍는 스타일
    ui->plot->graph(0)->setLineStyle(QCPGraph::lsLine);
    time = 0;

    ui->plot2->setInteraction(QCP::iRangeDrag, true);
    ui->plot2->setInteraction(QCP::iRangeZoom, true);
    ui->plot2->addGraph();
    ui->plot2->yAxis->setLabel("습도");
    ui->plot2->yAxis->setRange(20.0, 80.0);
    ui->plot2->xAxis->setLabel("시간");
    ui->plot2->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);  //점 찍는 스타일
    ui->plot2->graph(0)->setLineStyle(QCPGraph::lsLine);
    QPen pen;
    pen.setColor(QColor(255,0,0));
    ui->plot->graph(0)->setPen(pen);
    time2 = 0;

    ui->plot3->setInteraction(QCP::iRangeDrag, true);
    ui->plot3->setInteraction(QCP::iRangeZoom, true);
    ui->plot3->addGraph();
    ui->plot3->addGraph();
    ui->plot3->yAxis->setLabel("온도, 습도");
    ui->plot3->yAxis->setRange(20.0, 80.0);
    ui->plot3->xAxis->setLabel("시간");
    ui->plot3->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);  //점 찍는 스타일
    ui->plot3->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->plot3->graph(1)->setScatterStyle(QCPScatterStyle::ssCircle);  //점 찍는 스타일
    ui->plot3->graph(1)->setLineStyle(QCPGraph::lsLine);
    QPen pen2;
    pen2.setColor(QColor(255,0,0));
    ui->plot3->graph(0)->setPen(pen);
    time3 = 0;

//    alive();
    //--------------- Add chart end
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::text_reading()
{
#if 1
    QList <QByteArray> receivedList;  //  QByteArray를 list 자료구조로 만든다.
    char tmp_ch[6];
    char wet_ch[6];
    char msg[40];


    while (port->canReadLine())        //  \n이 존재하면
    {
        receivedList.append(port->readLine());   //  [Tmp]\n까지 읽어서 receive
    }

    for(QByteArray received : receivedList) {
        sprintf(msg, "%s", received.data());
        if ( !strncmp(msg, "[Tmp]", strlen("[Tmp]")))
        {
            strcpy(tmp_ch, msg+5);
            int tmp = atoi(tmp_ch);
            ui->lcdNumber_temp->display(tmp);
            ui->lcdNumber_tempcur->display(tmp);
            if (tmp >= temp_min)
                temp_min = temp_min;
            else if (tmp < temp_min)
                temp_min = tmp;
            ui->lcdNumber_tmpmin->display(temp_min);
            if (tmp >= temp_max)
                temp_max = tmp;
            else if (tmp < temp_max)
                temp_max = temp_max;
            ui->lcdNumber_tmpmax->display(temp_max);
            //----- chart start-----//
            add_point((double) time, (double) tmp);
            time += 2;              //  시간축을 shift 한다.
            ui->plot->yAxis->setRange(0.0, 40.0);   //y축 범위
            ui->plot->xAxis->setRange(0, time + 2);   //x축 확장
            plot(0);                //  그래프 그리4
            add_point3((double) time, (double) tmp);
            ui->plot3->yAxis->setRange(0.0, 40.0);   //y축 범위
            ui->plot3->xAxis->setRange(0, time + 2);   //x축 확장
            plot3(0);                //  그래프 그리기
            //----- chart end-------//
        }
        else if ( !strncmp(msg, "[Wet]", strlen("[Wet]")))
        {
            strcpy(wet_ch, msg+5);
            int wet = atoi(wet_ch);
            ui->lcdNumber_wet->display(wet);
            ui->lcdNumber_wetcur->display(wet);
            if (wet >= wet_min)
                wet_min = wet_min;
            else if (wet < wet_min)
                wet_min = wet;
            ui->lcdNumber_wetmin->display(wet_min);
            if (wet >= wet_max)
                wet_max = wet;
            else if (wet < wet_max)
                wet_max = wet_max;
            ui->lcdNumber_wetmax->display(wet_max);
            //----- chart start-----//
            add_point2((double) time2, (double) wet);
            time2 += 2;              //  시간축을 shift 한다.
            bar += 1;
            ui->plot2->yAxis->setRange(20.0, 80.0);   //y축 범위
            ui->plot2->xAxis->setRange(0, time2 + 2);   //x축 확장
            plot2(0);                //  그래프 그리기
            add_point4((double) time, (double) wet);
            ui->plot3->yAxis->setRange(20.0, 80.0);   //y축 범위
            ui->plot3->xAxis->setRange(0, time2 + 2);   //x축 확장
            plot4(0);
                            //  그래프 그리기

            //----- chart end-------//
        }

        bar = bar % 4;

        switch (bar)
        {
            case 0 :
                ui->label_alive->setText("o");
                break;
            case 1 :
                ui->label_alive->setText("oo");
                break;
            case 2 :
                ui->label_alive->setText("ooo");
                break;
            case 3 :
                ui->label_alive->setText("oooo");
                break;
        }

        ui->textEdit_receive->append(received);  //  1개씩 추가
        ui->textEdit_receive->show();      //  display

        qDebug() << received;
    }
#endif
#if 0
    QByteArray received;

    while (port->canReadLine())        //  \n이 존재하면
    {
        received = port->readLine();   //  \n까지 읽어서 receive
    }
    ui->textEdit_receive->append(received);  //  1개씩 추가
    ui->textEdit_receive->show();      //  display

    qDebug() << received;
#endif
}

void MainWindow::alive()
{

}

//------ chart start -----//
void MainWindow::add_point(double x, double y)
{
    qv_x.append(x);
    qv_y.append(y);
}

void MainWindow::add_point2(double x2, double y2)
{
    qv_x2.append(x2);
    qv_y2.append(y2);
}
void MainWindow::add_point3(double x3, double y3)
{
    qv_x3.append(x3);
    qv_y3.append(y3);
}
void MainWindow::add_point4(double x4, double y4)
{
    qv_x4.append(x4);
    qv_y4.append(y4);
}

void MainWindow::plot(int select)   //  그래프 그리기
{
    if(select == 0)
    {
        ui->plot->graph(0)->setData(qv_x, qv_y);
        ui->plot->replot();
        ui->plot->update();
    }
}

void MainWindow::plot2(int select2)   //  그래프 그리기
{
    if(select2 == 0)
    {
        ui->plot2->graph(0)->setData(qv_x2, qv_y2);
        ui->plot2->replot();
        ui->plot2->update();
    }
}

void MainWindow::plot3(int select3)   //  그래프 그리기
{
    if(select3 == 0)
    {
        ui->plot3->graph(0)->setData(qv_x3, qv_y3);
        ui->plot3->graph(1)->setData(qv_x4, qv_y4);
        ui->plot3->replot();
        ui->plot3->update();
    }
}

void MainWindow::plot4(int select4)   //  그래프 그리기
{
    if(select4 == 0)
    {
        ui->plot3->graph(0)->setData(qv_x3, qv_y3);
        ui->plot3->graph(1)->setData(qv_x4, qv_y4);
        ui->plot3->replot();
        ui->plot3->update();
    }
}
//------ chart end   -----//

void MainWindow::on_pushButton_scanagain_clicked()
{
    ui->comboBox_port->clear();  //  combobox clear
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()) {
        ui->comboBox_port->addItem(serialPortInfo.portName());
    }
}

void MainWindow::on_pushButton_open_clicked()
{
    port->setPortName(ui->comboBox_port->currentText());

    if (ui->comboBox_speed->currentIndex() == 0)
        port->setBaudRate(QSerialPort::Baud9600);
    else if (ui->comboBox_speed->currentIndex() == 1)
        port->setBaudRate(QSerialPort::Baud115200);

    if (ui->comboBox_databit->currentIndex() == 0)
        port->setDataBits(QSerialPort::Data8);
    else if (ui->comboBox_databit->currentIndex() == 1)
        port->setDataBits(QSerialPort::Data7);

    if (ui->comboBox_parity->currentIndex() == 0)
        port->setParity(QSerialPort::NoParity);
    else if (ui->comboBox_parity->currentIndex() == 1)
        port->setParity(QSerialPort::EvenParity);
    else if (ui->comboBox_parity->currentIndex() == 2)
        port->setParity(QSerialPort::OddParity);

    if (!port->open(QIODevice::ReadWrite))  //  ==0
    {
        qDebug() << "\n Serial port open error! \n";
    }
    else
    {
        ui->progressBar->setValue(100);
        qDebug() << "\n Serial port open success! \n";
    }
}



void MainWindow::on_pushButton_close_clicked()
{
    port->close();
    ui->progressBar->setValue(0);
}

void MainWindow::on_pushButton_send_clicked()
{
    QByteArray sendData;

    sendData = QString(ui->lineEdit_send->text() + "\n").toUtf8();  //  한글 컨버젼, 한글 아닐때는 관계x
    port->write(sendData.data());

    qDebug() << sendData;
}

void MainWindow::on_pushButton_clear_clicked()
{
    ui->lineEdit_send->setText("");
    ui->textEdit_receive->setText("");
    ui->lcdNumber_temp->display("");
    ui->lcdNumber_wet->display("");
}

void MainWindow::on_pushButton_dht11_clicked()
{
    QByteArray sendData1;

    if (ui->comboBox_dht11->currentIndex() == 0)
    {
    sendData1 = QString(ui->lineEdit_send->text() + "dht11time1500\n").toUtf8();
    port->write(sendData1.data());
    qDebug() << sendData1;
    }
    else if (ui->comboBox_dht11->currentIndex() == 1)
    {
    sendData1 = QString(ui->lineEdit_send->text() + "dht11time2000\n").toUtf8();
    port->write(sendData1.data());
    qDebug() << sendData1;
    }
    else if (ui->comboBox_dht11->currentIndex() == 2)
    {
    sendData1 = QString(ui->lineEdit_send->text() + "dht11time2500\n").toUtf8();
    port->write(sendData1.data());
    qDebug() << sendData1;
    }
    else if (ui->comboBox_dht11->currentIndex() == 3)
    {
    sendData1 = QString(ui->lineEdit_send->text() + "dht11time3000\n").toUtf8();
    port->write(sendData1.data());
    qDebug() << sendData1;
    }

}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    QByteArray sendData2;
    switch(arg1)
    {
        case Qt::Unchecked :
            sendData2 = QString("led2\n").toUtf8();
            port->write(sendData2.data());
            ui->progressBar_led->setValue(0);
            break;

        case Qt::Checked :
            sendData2 = QString("led2\n").toUtf8();
            port->write(sendData2.data());
            ui->progressBar_led->setValue(100);
            break;
    }
    qDebug() << sendData2;
}

void MainWindow::on_pushButton_clear_tmp_clicked()
{
    qv_x.clear();
    qv_y.clear();
    time = 0;
    ui->lcdNumber_tempcur->display("");
    ui->lcdNumber_tmpmax->display("");
    ui->lcdNumber_tmpmin->display("");
}

void MainWindow::on_pushButton_clear_wet_clicked()
{
    qv_x2.clear();
    qv_y2.clear();
    time2 = 0;
    ui->lcdNumber_wetcur->display("");
    ui->lcdNumber_wetmax->display("");
    ui->lcdNumber_wetmin->display("");
}
