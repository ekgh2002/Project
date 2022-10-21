#ifndef SERVICE_H
#define SERVICE_H

#include <string>
#include "View.h"
#include "ClockView.h"
#include "LightState.h"
#include "DHT11_Data.h"
#include "motorState.h"
#include "clockstate.h"

// enum {LIGHT_OFF, LIGHT_1, LIGHT_2, LIGHT_3, LIGHT_4, LIGHT_5};


class Service
{
private:
    int lightState;
    int motorState;
    int panstate;
    int warning_state;
    int warningactive;
    View *view;
    ClockView *clockview;
    bool bDistanceLight;

public:
    Service(View *viewer, ClockView *clockview);
    virtual ~Service();
    void updateState(std::string strState);
    void updatemotorState(std::string strState1);
    void updateEvent(DHT_Data dhtData);
    void updateDistance(int distance);
    void clockupdate(std::string strState2);
    // void updatewarningState(std::string strState4);
};

#endif