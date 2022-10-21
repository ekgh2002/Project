#include "Service.h"
#include <iostream>
#include <wiringPi.h>

Service::Service(View *viewer, ClockView *clockview)
{
    this->view = viewer;
    this->clockview = clockview;
    lightState = LIGHT_OFF;
    motorState = PAN_OFF;
    panstate = PAN_OFF;
    warningactive = 0;
    warning_state = 1;
    bDistanceLight = false;
}

Service::~Service()
{

}

void Service::updateState(std::string strState)
{
    switch (lightState)
    {
        case LIGHT_OFF:
            if (strState == "modeButton") {
                lightState = LIGHT_1;
                view->setState(lightState);
            }

        break;

        case LIGHT_1:
            if (strState == "modeButton") {
                lightState = LIGHT_2;
            }
            if (strState == "powerButton") {
                lightState = LIGHT_OFF;
            }
            if (bDistanceLight)
            {
                view->setState(lightState);
            }
            else
            {
                view->setState(LIGHT_OFF);
            }
        break;

        case LIGHT_2:
            if (strState == "modeButton") {
                lightState = LIGHT_3;
            }
            if (strState == "powerButton") {
                lightState = LIGHT_OFF;
            }
            if (bDistanceLight)
            {
                view->setState(lightState);
            }
            else
            {
                view->setState(LIGHT_OFF);
            }
        break;

        case LIGHT_3:
            if (strState == "modeButton") {
                lightState = LIGHT_4;
            }
            if (strState == "powerButton") {
                lightState = LIGHT_OFF;
            }
            if (bDistanceLight)
            {
                view->setState(lightState);
            }
            else
            {
                view->setState(LIGHT_OFF);
            }
            
        break;

        case LIGHT_4:
            if (strState == "modeButton") {
                lightState = LIGHT_5;
            }
            if (strState == "powerButton") {
                lightState = LIGHT_OFF;
            }
            if (bDistanceLight)
            {
                view->setState(lightState);
            }
            else
            {
                view->setState(LIGHT_OFF);
            }
        break;

        case LIGHT_5:
            if (strState == "modeButton") {
                lightState = LIGHT_OFF;
            }
            if (strState == "powerButton") {
                lightState = LIGHT_OFF;              
            }
            if (bDistanceLight)
            {
                view->setState(lightState);
            }
            else
            {
                view->setState(LIGHT_OFF);
            }
        break;

        case LIGHT_WARN:
            if (strState == "modeButton") {
                lightState = LIGHT_1;
                view->setState(lightState);
            }
            if (strState == "powerButton") {
                lightState = LIGHT_OFF;              
            }
    }
}

void Service::updatemotorState(std::string strState1)
{

    switch (motorState)
    {
        case PAN_OFF:
            if(strState1 == "onoffbt")
            {
                motorState = PAN1;
                view->setmotorState(motorState);
                // printf("hi");
            }
            if(strState1 == "warning")
            {
                motorState = PAN1;
                view->setmotorState(motorState);               
            }
        break;

        case PAN1:
            if(strState1 == "onoffbt")
            {
                motorState = PAN_OFF;
                view->setmotorState(motorState);
                // printf("hi");
            }
            if(strState1 == "panpowerbutton")
            {
                motorState = PAN2;
                view->setmotorState(motorState);
                // printf("hi");
            }


        break;

        case PAN2:
            if(strState1 == "onoffbt")
            {
                motorState = PAN_OFF;
                view->setmotorState(motorState);
                // printf("hi");
            }
            if(strState1 == "panpowerbutton")
            {
                motorState = PAN3;
                view->setmotorState(motorState);
                // printf("hi");
            }    

        break;

        case PAN3:
            if(strState1 == "onoffbt")
            {
                motorState = PAN_OFF;
                view->setmotorState(motorState);
            }
            if(strState1 == "panpowerbutton")
            {
                motorState = PAN1;
                view->setmotorState(motorState);
            }

        break;
    }        
  
}

// void Service::updatewarningState(std::string strState4)
// {
//     if(warningactive == 1)
//     {
//         switch (warning_state)
//         {
//             case 0:
//                 if(strState4 == "onoffbt")
//                 {
//                     warning_state = 1;
//                     view->setmotorState(motorState);
//                     printf("hi");
//                 }
//             break;

//             case 1:
//                 if(strState4 == "onoffbt")
//                 {
//                     warning_state = 0;
//                     view->setmotorState(motorState);
//                     printf("hi");
//                 }
//                 if(strState4 == "panpowerbutton")
//                 {
//                     warning_state = 2;
//                     view->setmotorState(motorState);
//                     // printf("hi");
//                 }
//             break;

//             case 2:
//                 if(strState4 == "onoffbt")
//                 {
//                     warning_state = 0;
//                     view->setmotorState(motorState);
//                     printf("hi");
//                 }
//                 if(strState4 == "panpowerbutton")
//                 {
//                     warning_state = 3;
//                     view->setmotorState(motorState);
//                     printf("hi");
//                 }    
//             break;

//             case 3:
//                 if(strState4 == "onoffbt")
//                 {
//                     warning_state = 0;
//                     view->setmotorState(motorState);
//                 }
//                 if(strState4 == "panpowerbutton")
//                 {
//                     warning_state = 1;
//                     view->setmotorState(motorState);
//                 }
//             break;
//         }
//     }
// }



void Service::updateEvent(DHT_Data dhtData)
{
    float temp;
    temp = (float)dhtData.Temp + (float)(dhtData.TempDec/10.0);
    view->setState(lightState);

    // if(temp >= 25)
    // {
    //     motorState = PAN1;
    //     view->motorview();
    // }
    // else if(temp < 25)
    // {
    //     motorState = PAN_OFF;
    //     view->motorview();
    // }
    // view->lightwarning(dhtData);
}

void Service::updateDistance(int distance)
{

    static int distance_count = 0;
    printf("distance : %d\n", distance);
    if (distance < 0)
    {
        distance_count = distance_count+1;
    }
    else if(distance > 0)
    {
        distance_count = 0;
    }
    
    if (distance_count > 4)
    {
        bDistanceLight = false;
        view->setState(LIGHT_OFF);
        distance_count = 0;
    }
    else
    {
        bDistanceLight = true;
        view->setState(lightState);
    }
}