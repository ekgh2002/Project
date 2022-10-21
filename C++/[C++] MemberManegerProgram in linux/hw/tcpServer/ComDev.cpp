#include "ComDev.h"
#include <thread>


ComDev::ComDev(tcpServer *server)
{
    cardTcpServer = server;
}

ComDev::~ComDev()
{
    delete cardTcpServer;  //  소멸자에서는 항상 만들어 줘야함.
    
}

void ComDev::sendData(int *cardNum)
{
    char sendBuff[1000];
    sprintf(sendBuff, "%02x-%02x-%02x-%02x-%02x", cardNum[0], cardNum[1], cardNum[2], cardNum[3], cardNum[4]);
    cardTcpServer->sendData(sendBuff, strlen(sendBuff));
}