#include "View.h"
using namespace std;

#include <stdio.h>
#include <cstring>
#include <iostream>
#include <wiringPi.h>



View::View()
{
    lcd = new LCD(new I2C("/dev/i2c-1", 0x27));
    membersEntity = new MembersEntity();
    membersManagerState = CARD_READER;
    light1 = new Led(21);
    light2 = new Led(22);
    light3 = new Led(23);
    light4 = new Led(24);
    light5 = new Led(25);
    clocktime = 0;
    clocktime_sec = 0;
    clocktime_min = 0;
    clocktime_hour = 0;
    clockstate = 0;
    curTime = 0;
}

View::~View()
{


}

void View::setstate(int state)
{
    membersManagerState = state;
}

void View::print_view()
{
    switch(membersManagerState)
    {
        case CARD_READER:
            lcd->WriteStringXY(0, 0, "1] READER_MODE     ");
            lcd->backLightOn();
            light1->On();
            light2->Off();
            light3->Off();
            light4->Off();
            light5->Off();
            Clock();
            
        break;

        case CARD_REGISTER:
            lcd->WriteStringXY(0, 0, "2] REGISTER_MODE   ");
            lcd->backLightOn();
            light1->Off();
            light2->On();
            light3->Off();
            light4->Off();
            Clock();
        break;

        case CARD_REMOVE:
            lcd->WriteStringXY(0, 0, "3] REMOVE_MODE   ");
            lcd->backLightOn();
            light1->Off();
            light2->Off();
            light3->On();
            light4->Off();
            Clock();
        break;

        case CARD_MODIFY:
            lcd->WriteStringXY(0, 0, "4] MODIFY_MODE    ");
            lcd->backLightOn();
            light1->Off();
            light2->Off();
            light3->Off();
            light4->On();
            Clock();
            
        break;

        // case tcp:
        //     lcd->WriteStringXY(0, 0, "     WELCOME    ");
        //     lcd->backLightOn();
        //     light1->Off();
        //     light2->Off();
        //     light3->Off();
        //     light4->Off();
        //     light5->On();
        //     Clock();
    


    }
}

void View::updateTime(struct tm *timeData)
{

    char buff[30];
    sprintf(buff, "%02d:%02d:%02d",
    timeData->tm_hour,
    timeData->tm_min,
    timeData->tm_sec);
    lcd->WriteStringXY(1, 0, buff);

}

void View::Clock()
{
    curTime = time(NULL);
    struct tm *timeDate = localtime(&curTime);
    sprintf(buff, "    %02d:%02d:%02d", timeDate->tm_hour, timeDate->tm_min, timeDate->tm_sec);
    lcd->WriteStringXY(1, 0, buff);
}
