#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <string>
#include "Service.h"
#include "ClockService.h"
#include "DHT11_Data.h"
#include "TempHumidService.h"

class Controller
{
private:
    int lightState;
    int motorState;
    int warningactive;
    Service *service;
    ClockService *clockService;
    TempHumidService *tempHumidService;

public:
    Controller(Service *serv, ClockService *clockServ,  TempHumidService *tempHumidService);
    virtual ~Controller();
    void updateEvent(std::string strBtn);
    void updateTempHumid(DHT_Data dhtData);
    void updateDistance(int distance); 
};

#endif /* __CONTROLLER_H__ */