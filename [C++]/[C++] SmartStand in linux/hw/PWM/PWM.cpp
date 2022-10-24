#include "PWM.h"

PWM::PWM(int pin)
{
    this->pwmPin = pin;
    wiringPiSetup();
    softPwmCreate(pwmPin, 0, 100);

}

PWM::~PWM()
{

}

void PWM::Write(int value)
{
    softPwmWrite(pwmPin, value);
}