#include <iostream>
#include <wiringPi.h>
#include "Button.h"
#include "Led.h"
#include "Listener.h"
#include "Controller.h"
#include "View.h"
#include "Service.h"
#include "LCD.h"
#include "ClockView.h"
#include "ClockService.h"
#include "ClockCheck.h"
#include "I2C.h"
#include "DHT11.h"
#include "TempHumidService.h"
#include "TempHumidView.h"
#include "UltraSonic.h"
#include "PWM.h"

int main()
{
    std::cout << "Hello World!" << std::endl;
    Button modeButton(2);
    Button powerButton(3);
    Button onoffbt(27);
    Button panpowerbutton(28);
    Button clockbutton(29);
    ClockCheck clockCheck;
    Led led1(21);
    Led led2(22);
    Led led3(23);
    Led led4(24);
    Led led5(25);
    PWM pwm(26);
    DHT11 dht(7);
    UltraSonic ultraSonic(5, 4);
    LCD lcd(new I2C("/dev/i2c-1", 0x27));
    View view(&led1, &led2, &led3, &led4, &led5, &lcd, &pwm);
    TempHumidView tempHumidView(&lcd, &pwm);
    ClockView clockView(&lcd);
    Service service(&view, &clockView);
    ClockService clockService(&clockView);
    TempHumidService tempHumidService(&tempHumidView);
    Controller control(&service, &clockService, &tempHumidService);
    Listener listener(&modeButton, &powerButton, &panpowerbutton, &onoffbt, &clockbutton, &control, &clockCheck, &dht, &ultraSonic);
    DHT_Data dhtData;

    while (1)
    {
        listener.checkEvent();
        view.lightView();
        view.motorview();
        clockView.counterclock();

        delay(100);
        // pwm.Write(40);
        // delay(2000);
        // pwm.Write(70);
        // delay(2000);


    }

    return 0;
}