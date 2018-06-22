#ifndef TIMER_H
#define TIMER_H

#include <QTimer>


class timer
{
public:
    int sec = 0;
    int start_sec=0;
    QString text = 0;
    bool isTimer = true;
    bool isActive = false;
    bool isChek = false;
    bool stop = false;
    QString message = "";
    timer();
};

#endif // TIMER_H
