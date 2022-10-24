#include "GolfMembershipManager.h"

GolfMembershipManager::GolfMembershipManager()
{  
    // listener = new Listener();
}

GolfMembershipManager::~GolfMembershipManager()
{

}

void GolfMembershipManager::run()
{
    while(1)
    {
        listener->checkEvent();
        delay(10);
    }
}