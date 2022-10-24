#include "ClockCheck.h"

ClockCheck::ClockCheck()
{
    prevSec = 0;
}

ClockCheck::~ClockCheck()
{

}

bool ClockCheck::isUpdate()
{
    time_t curSec = time(NULL);
    if (curSec == prevSec){
        return false;
    }
    else {
        prevSec = curSec;
        return true;
    }
    return false;
}