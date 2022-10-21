#ifndef TEMPHUMIDSERVICE_H
#define TEMPHUMIDSERVICE_H

#include "DHT11_Data.h"
#include "TempHumidView.h"

class TempHumidService
{
private:
    TempHumidView *tempHumidView;

public:
    TempHumidService(TempHumidView *tempHumidView);
    virtual ~TempHumidService();
    void updateEvent (DHT_Data dhtData);

};

#endif