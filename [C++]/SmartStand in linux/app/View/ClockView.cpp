#include "ClockView.h"
#include <wiringPi.h>

ClockView::ClockView(LCD *Lcd)
{
    this->lcd = Lcd;  //  this 내객체에서 선언되어있는 변수 lcd 매개변수에서 들어오는 lcd
    //timeDate = 0;
    clocktime = 0;
    clocktime_sec = 0;
    clocktime_min = 0;
    clocktime_hour = 0;
    clockstate = CLOCK;
}

ClockView::~ClockView()
{
}

void ClockView::setclockstate(int state3)
{
    clockstate = state3;
}


void ClockView::updateTime(struct tm *timeData)
{
    if(clockstate == CLOCK)
    {
        char buff[30];
        sprintf(buff, "%02d:%02d:%02d",
        timeData->tm_hour,
        timeData->tm_min,
        timeData->tm_sec);
        lcd->WriteStringXY(1, 0, buff);
    }
}

void ClockView::counterclock()
{
    if(clockstate == Timer)
    {
        static unsigned int countertime = 0;
        if(millis() - countertime > 1000)
        {
            countertime = millis();
            clocktime = clocktime + 1;
        }
        clocktime_sec = clocktime % 60;
        clocktime_min = (clocktime/60) % 60;
        clocktime_hour = (clocktime/3600) % 24;
        
        char buff[30];
        sprintf(buff, "%02d:%02d:%02d",
        clocktime_hour, clocktime_min, clocktime_sec);
        lcd->WriteStringXY(1, 0, buff); 
    }
    if(clockstate == STOP)
    {
        char buff[30];
        sprintf(buff, "%02d:%02d:%02d",
        clocktime_hour, clocktime_min, clocktime_sec);
        lcd->WriteStringXY(1, 0, buff);
    }

    if(clockstate == RUN)
    {
        static unsigned int countertime = 0;
        if(millis() - countertime > 1000)
        {
            countertime = millis();
            clocktime = clocktime + 1;
        }
        clocktime_sec = clocktime % 60;
        clocktime_min = (clocktime/60) % 60;
        clocktime_hour = (clocktime/3600) % 24;
        char buff[30];
        sprintf(buff, "%02d:%02d:%02d",
        clocktime_hour, clocktime_min, clocktime_sec);
        lcd->WriteStringXY(1, 0, buff);
    }
    if(clockstate == RESET)
    {
        clocktime_hour = 0;
        clocktime_min = 0;
        clocktime_sec = 0;
        clocktime = 0;
        char buff[30];
        sprintf(buff, "%02d:%02d:%02d",
        clocktime_hour, clocktime_min, clocktime_sec);
        lcd->WriteStringXY(1, 0, buff);
    }    


        
    // printf("hi");

}

