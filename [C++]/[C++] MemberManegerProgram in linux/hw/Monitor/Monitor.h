#ifndef MONITOR_H
#define MONITOR_H
#include <iostream>

class Monitor
{
private:

public:
    Monitor();
    virtual ~Monitor();
    void print(uint8_t *data, int size);

};

#endif