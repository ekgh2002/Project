#ifndef COMDEV_H
#define COMDEV_H
#include "tcpServer.h"


class ComDev
{
private:
    tcpServer *cardTcpServer;

public:
    ComDev(tcpServer *server);
    virtual ~ComDev();
    void sendData(int *cadrNum);

};

#endif