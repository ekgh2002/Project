#ifndef CLOCKVIEW_H
#define CLOCKVIEW_H

#include <time.h>
#include "LCD.h"
#include "clockstate.h"

class ClockView
{
private:
    struct tm timeDate;
    LCD *lcd;
    int clocktime;
    int clocktime_sec;
    int clocktime_min;
    int clocktime_hour;
    int clockstate;

public:
    ClockView(LCD *lcd);
    virtual ~ClockView();
    void updateTime(struct tm *timeData);
    void counterclock();
    void setclockstate(int state3);
};

#endif