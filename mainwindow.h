#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QTime>
#include <QApplication>
#include "timer.h"
#include <QMouseEvent>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QVector< timer* > v;
    QTimer* tmr;
    int current = 0;
    void fillTimer();

private slots:

    void closeEvent(QCloseEvent *event);
    void on_pushButton_4_clicked();

    void on_textEdit_textChanged();

    void next_step();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_checkBox_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_textEdit_2_textChanged();

    void on_radioButton_2_clicked();

    void on_radioButton_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
