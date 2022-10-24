#include "View.h"
#include <wiringPi.h>
#include <LCD.h>
#include <PWM.h>

View::View(Led *led1, Led *led2, Led *led3, Led *led4, Led *led5, LCD *Lcd, PWM *pwm)
{
    this->light1 = led1;
    this->light2 = led2;
    this->light3 = led3;
    this->light4 = led4;
    this->light5 = led5;
    this->lcd = Lcd;
    this->pwm = pwm;
    lightState = LIGHT_OFF;
    motorState = PAN_OFF;
    warningstate = 0;
}

View::~View()
{

}
void View::setState(int state)
{
    lightState = state;
}

void View::setmotorState(int state2)
{
    motorState = state2;
}

void View::lightView()
{
    switch (lightState)
    {
        case LIGHT_OFF:
            lightOff();
     
        break;

        case LIGHT_1:
            lightOn_1();
      
        break;

        case LIGHT_2:
            lightOn_2();
            
        break;

        case LIGHT_3:
            lightOn_3();
        
        break;

        case LIGHT_4:
            lightOn_4();
       
        break;

        case LIGHT_5:
            lightOn_5();    
        break; 

        case LIGHT_WARN:
            lightwarning();
        break;
    }
}

void View::motorview()
{
    switch (motorState)
    {
        case PAN_OFF:
            panoff();

        break;

        case PAN1:
            pan1();
        
        break;

        case PAN2:
            pan2();
         
        break;

        case PAN3:
            pan3();
       
        break;
    }

}

// void View::warningpanview()
// {
//     switch (warningstate)
//     {
//         case 0:
//             panoff();

//         break;

//         case 1:
//             pan1();
        
//         break;

//         case 2:
//             pan2();
         
//         break;

//         case 3:
//             pan3();
       
//         break;
//     }    
// }

void View::lightOn_1()
{
     char buff[30];
     sprintf(buff, " Light 1 ");
     lcd->WriteStringXY(0, 0, buff);
    lcd->backLightOn();
    light1->On();
    light2->Off();
    light3->Off();
    light4->Off();
    light5->Off();
}

void View::lightOn_2()
{
     char buff[30];
     sprintf(buff, " Light 2 ");
     lcd->WriteStringXY(0, 0, buff);
    lcd->backLightOn();
    light1->On();
    light2->On();
    light3->Off();
    light4->Off();
    light5->Off();
}

void View::lightOn_3()
{
     char buff[30];
     sprintf(buff, " Light 3 ");
     lcd->WriteStringXY(0, 0, buff);
    lcd->backLightOn();
    light1->On();
    light2->On();
    light3->On();
    light4->Off();
    light5->Off();

}

void View::lightOn_4()
{
     char buff[30];
     sprintf(buff, " Light 4 ");
     lcd->WriteStringXY(0, 0, buff);
    lcd->backLightOn();
    light1->On();
    light2->On();
    light3->On();
    light4->On();
    light5->Off();
}

void View::lightOn_5()
{
     char buff[30];
     sprintf(buff, " Light 5 ");
     lcd->WriteStringXY(0, 0, buff);
    lcd->backLightOn();
    light1->On();
    light2->On();
    light3->On();
    light4->On();
    light5->On();
}

void View::lightOff()
{
     char buff[30];
     sprintf(buff, "LightOFF ");
     lcd->WriteStringXY(0, 0, buff);
    lcd->backLightOff();
    light1->Off();
    light2->Off();
    light3->Off();
    light4->Off();
    light5->Off();
}

void View::lightwarning()
{
     char buff[30];
     sprintf(buff, "LightWARN");
     lcd->WriteStringXY(0, 0, buff);
    lcd->backLightOn();
    light1->Off();
    light2->Off();
    light3->Off();
    light4->Off();
    light5->Off();
}

void View::pan1()
{
    pwm->Write(40);
    // printf("hi");
}
void View::pan2()
{
    pwm->Write(70);
    // printf("hi");
}
void View::pan3()
{
    pwm->Write(99);
    // printf("hi");
}
void View::panoff()
{
    pwm->Write(0);
    // printf("hi");
}

// void View::panwarning()
// {
//     char buff[30];
//     sprintf(buff, "WARNING ");
//     lcd->WriteStringXY(0, 0, buff);
//     lcd->backLightOff();
//     pwm->Write(40);
//     light1->Off();
//     light2->Off();
//     light3->Off();
//     light4->Off();
//     light5->Off();
// }



// void View::LCDplay0()
// {
//     lcd->WriteStringXY(0, 0, "      OFF   ");
// }

// void View::LCDplay1()
// {
//     lcd->WriteStringXY(0, 0, "     power1");
// }

// void View::LCDplay2()
// {
//     lcd->WriteStringXY(0, 0, "     power2");
// }

// void View::LCDplay3()
// {
//     lcd->WriteStringXY(0, 0, "     power3");
// }

// void View::LCDplay4()
// {
//     lcd->WriteStringXY(0, 0, "     power4");
// }

// void View::LCDplay5()
// {
//     lcd->WriteStringXY(0, 0, "     power5");
// }
// heyhey