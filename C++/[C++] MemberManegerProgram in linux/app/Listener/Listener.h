#ifndef LISTENER_H
#define LISTENER_H

#include "CardReader.h"
#include "Monitor.h"
#include "Controller.h"
#include "ManageButton.h"

class Listener
{
private:
    CardReader *rfid;
    ManageButton *modeButton;
    ManageButton *modeButton2;
    Controller *controller;
    tcpServer *tcpserver;
    uint8_t rfidData[16];
    
    
public:
    Listener(Controller *controller);
    virtual ~Listener();
    void checkEvent();
    bool checkRfid();



};

#endif