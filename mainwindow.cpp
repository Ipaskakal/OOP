#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCloseEvent>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->textEdit_3->setText("00:00:00");
    ui->textEdit_4->setText("00:00:00");
    ui->radioButton->setChecked(1);

    timer *new_timer = new timer();
    v.push_back(new_timer);
    ui->listWidget->addItem("0");
    ui->listWidget->item(current)->setBackgroundColor("red");

    tmr = new QTimer(this);
    connect(tmr, SIGNAL(timeout()), this, SLOT(next_step()));
    tmr->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
     QMessageBox::StandardButton ret;
    ret = QMessageBox::question( this,  QApplication::applicationName(), tr(" Do you want to close programm ? "),
                                 QMessageBox::Yes | QMessageBox::No , QMessageBox::No );
    if (ret == QMessageBox::No)
        event->ignore();
    else{
        QFile file("save.txt");
        qDebug()<< "x";
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {

            QTextStream out(&file);
            for(int i=0;i<v.size();i++)
            {
                 out << v[i]->text<<" ";
            }
            qDebug()<< "x";

        }
       file.close();
    }

}

//StartCurrent
void MainWindow::on_pushButton_4_clicked()
{
    if (current<v.size()&& v[current]->text!=0 )
    {
    v[current]->isActive = true;

    if(v[current]->stop == false)
    {
        QStringList list=v[current]->text.split(":");
       if(list.size()==3&&list[0].toInt()<24 && list[1].toInt()<60 && list[2].toInt()<60
               && list[0].size()==2  && list[2].size()==2  && list[2].size()==2){
        v[current]->sec = (list[0].toInt())*3600+(list[1].toInt())*60+(list[2].toInt());
         v[current]->start_sec = v[current]->sec;
        } else {
            v[current]->isActive = false;
            return;
        }
    }
    else v[current]->stop = false;
    ui->label->setText(QString::number(v[current]->sec));
    ui->listWidget->item(current)->setText((v[current]->text));
     ui->listWidget->item(current)->setTextColor("blue");
    }
}

void MainWindow::on_textEdit_textChanged()
{
    v[current]->text = ui->textEdit->toPlainText();
}

void MainWindow::next_step() {
    for(int i = 0; i < v.size(); ++i) {
        if(v[i]->isActive)
        {
            if(v[i]->isTimer) {
                --v[i]->sec;
                qDebug() << "alarm";
                ui->listWidget->item(i)->setText(QString::number(v[i]->sec));
                if(v[i]->sec == 0) {
                    v[i]->isActive = false;
                    v[i]->sec=v[i]->start_sec;
                    ui->listWidget->item(i)->setText(QString::number(v[i]->sec));
                    if(QTime::currentTime().toString() < ui->textEdit_3->toPlainText() ||
                       QTime::currentTime().toString() > ui->textEdit_4->toPlainText()) {
                        QApplication::beep();
                        ui->listWidget->item(i)->setTextColor("black");
                        QMessageBox msgBox;
                        msgBox.setText(v[i]->message);
                        msgBox.exec();
                    }
                }
            } else {
                qDebug() << "alarm";
                if(QTime::currentTime().toString() ==v[i]->text && (
                   QTime::currentTime().toString() < ui->textEdit_3->toPlainText() ||
                   QTime::currentTime().toString() > ui->textEdit_4->toPlainText() )) {
                    v[i]->isActive = false;
                    QApplication::beep();
                    QMessageBox msgBox;
                    msgBox.setText(v[i]->message);
                    msgBox.exec();
                }
            }
        }

    }
    if(v[current]->isTimer)
        ui->label->setText(QString::number(v[current]->sec));
    else
        ui->label->setText(QTime::currentTime().toString());
}

void MainWindow::on_pushButton_2_clicked()
{
    QStringList list=v[current]->text.split(":");
    if(list.size()==3 && list[0].toInt()<24 && list[1].toInt()<60 && list[2].toInt()<60
            && list[0].size()==2  && list[1].size()==2  && list[2].size()==2)
    ui->listWidget->item(current)->setText((v[current]->text));
    timer *new_timer = new timer();
    v.push_back(new_timer);
    ui->listWidget->item(current)->setBackgroundColor("white");
    current = v.size() - 1;

    ui->listWidget->addItem("00");
     ui->listWidget->item(current)->setBackgroundColor("red");
    fillTimer();
}

void MainWindow::on_pushButton_3_clicked()
{
    if(current < v.size() - 1)
    {
        ui->listWidget->item(current)->setBackgroundColor("white");
        current++;
        ui->listWidget->item(current)->setBackgroundColor("red");
        fillTimer();
    }
}

void MainWindow::on_pushButton_clicked()
{
    if(current > 0)
    {
        ui->listWidget->item(current)->setBackgroundColor("white");
        current--;
        ui->listWidget->item(current)->setBackgroundColor("red");
        fillTimer();
    }
}

void MainWindow::fillTimer() {
    ui->textEdit->setText(v[current]->text);
    ui->label->setText(QString::number(v[current]->sec));
    ui->textEdit_2->setText(v[current]->message);
    if(v[current]->isChek) {
        ui->checkBox->setChecked(1);
    } else ui->checkBox->setChecked(0);
    if(v[current]->isTimer)
        ui->radioButton->setChecked(1);
    else
        ui->radioButton_2->setChecked(1);
}

void MainWindow::on_checkBox_clicked()
{
    if(ui->checkBox->isChecked()) {
        v[current]->isChek = true;
    } else v[current]->isChek = false;
}

void MainWindow::on_pushButton_5_clicked()
{
    for(int i = 0; i < v.size(); ++i) {
        if(v[i]->isChek) {
            v[i]->isActive = true;
            ui->listWidget->item(i)->setTextColor("blue");
            if(v[i]->stop == false)
                v[i]->sec = v[i]->text.toInt();
            else v[i]->stop = false;
        }
    }
}

void MainWindow::on_pushButton_6_clicked()
{
    v[current]->isActive = false;
    ui->listWidget->item(current)->setTextColor("black");
    v[current]->stop = true;
}

void MainWindow::on_pushButton_7_clicked()
{
    for(int i = 0; i < v.size(); ++i) {
        if(v[i]->isChek) {
            v[i]->isActive = false;
            ui->listWidget->item(i)->setTextColor("black");
            v[i]->stop = true;
        }
    }
}

void MainWindow::on_textEdit_2_textChanged()
{
    v[current]->message = ui->textEdit_2->toPlainText();
}

void MainWindow::on_radioButton_2_clicked()
{
    v[current]->isTimer = false;
}

void MainWindow::on_radioButton_clicked()
{
    v[current]->isTimer = true;
}

void MainWindow::on_pushButton_8_clicked()
{
    if(v[current]->stop == false)
    {
        QStringList list=v[current]->text.split(":");
        if(list.size()==3 && list[0].toInt()<24 && list[1].toInt()<60 && list[2].toInt()<60
                && list[0].size()==2  && list[1].size()==2  && list[2].size()==2){
        v[current]->sec = (list[0].toInt())*3600+(list[1].toInt())*60+(list[2].toInt());
         v[current]->start_sec = v[current]->sec;
         ui->listWidget->item(current)->setText((v[current]->text));
        } else return;
    }
    else v[current]->stop = false;
    ui->label->setText(QString::number(v[current]->sec));
    /*timer *new_timer = new timer();
    v.push_back(new_timer);
    ui->listWidget->item(current)->setBackgroundColor("white");
    current = v.size() - 1;

    ui->listWidget->addItem("00");
    ui->listWidget->item(current)->setBackgroundColor("red");
    fillTimer();*/

}

void MainWindow::on_pushButton_9_clicked()
{
    v.removeAt(current);
     ui->listWidget->takeItem(current);
     --current;
     ui->listWidget->item(current)->setBackgroundColor("red");
     fillTimer();
}


