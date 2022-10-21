#ifndef PWM_H
#define PWM_H

#include <wiringPi.h>
#include <softPwm.h>

#pragma once

class PWM
{
private:
    int pwmPin;

public:
    PWM(int pin);
    ~PWM();
    void Write(int value);
};

#endif