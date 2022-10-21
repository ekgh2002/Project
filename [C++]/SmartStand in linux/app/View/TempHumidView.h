#ifndef TEMPHUMIDVIEW_H
#define TEMPHUMIDVIEW_H

#include "LCD.h"
#include "Led.h"
#include "PWM.h"


class TempHumidView
{
private:
    LCD *lcd;
    PWM *pwm;
    int warningtime;


public:
    TempHumidView(LCD *lcd, PWM *pwm);
    virtual ~TempHumidView();
    void setTempHumidData(float temp, float humid);
};

#endif