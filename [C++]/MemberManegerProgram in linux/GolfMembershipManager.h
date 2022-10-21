#ifndef GOLFMEMBERSHIPMANAGER_H
#define GOLFMEMBERSHIPMANAGER_H

#include "Listener.h"
#include <iostream>

class GolfMembershipManager
{

private:
    Listener *listener;

public:
    GolfMembershipManager();
    virtual ~GolfMembershipManager();
    void run();

};

#endif