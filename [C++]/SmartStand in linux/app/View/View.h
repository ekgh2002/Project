#ifndef __VIEW_H__
#define __VIEW_H__

#include <string>
#include "Led.h"
#include "LightState.h"
#include "motorState.h"
#include "LCD.h"
#include "PWM.h"

// enum {LIGHT_OFF, LIGHT_1, LIGHT_2, LIGHT_3, LIGHT_4, LIGHT_5};

class View
{
private:
    int lightState;
    int motorState;
    int warningstate;
    Led *light1;
    Led *light2;
    Led *light3;
    Led *light4;
    Led *light5;
    LCD *lcd;
    PWM *pwm;

public:
    View(Led *led1, Led *led2, Led *led3, Led *led4, Led *led5, LCD *Lcd, PWM *pwm);
    virtual ~View();
    void setState(int state);
    void setmotorState(int state2);
    void lightView();
    void lightOn_1();
    void lightOn_2();
    void lightOn_3();
    void lightOn_4();
    void lightOn_5();
    void lightwarning();
    void lightOff();
    void pan1();
    void pan2();
    void pan3();
    void panoff();
    void motorview();
    void panwarning();
    // void warningpanview();

    // void LCDplay0();
    // void LCDplay1();
    // void LCDplay2();
    // void LCDplay3();
    // void LCDplay4();
    // void LCDplay5();
};

#endif /* __VIEW_H__ */