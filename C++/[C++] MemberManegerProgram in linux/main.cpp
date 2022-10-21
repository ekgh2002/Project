#include <iostream>
#include "GolfMembershipManager.h"
#include "MembersEntity.h"
#include <cstring>
#include <thread>
#include "Listener.h"
#include "Controller.h"
#include "MembersManageService.h"
#include "tcpServer.h"
#include "ComDev.h"
#include "View.h"
#include <wiringPi.h>

void serverThread(tcpServer *server)
{
    char recvBuff[BUFSIZ];
    int recvLen;

    while (server->waitAccept() > 0)
    {
        server->setClientState(true);
        while((recvLen = server->recvData(recvBuff, sizeof(recvBuff))) > 0)
        {
            recvBuff[recvLen] = '\0';
            server->sendData(recvBuff, recvLen);
            printf("received : %s\n", recvBuff);
        }
        server->closeSocket(server->getClientSocket());
        server->setClientState(false);
        printf("close client socket\n");
    }
}

int main(void)
{
    tcpServer *cardTcpServer = new tcpServer(5100);
    ComDev *comDev = new ComDev(cardTcpServer); 
    View *view = new View();
    MembersManageService *membersManageService = new MembersManageService(comDev, view);   
    Controller *controller = new Controller(membersManageService);
    Listener *listener = new Listener(controller);
    std::thread threadFunc(serverThread, cardTcpServer);
    
   
    // GolfMembershipManager golfMembershipManager;
    // golfMembershipManager.run();

    while(1)
    {
        listener->checkEvent();
        view->print_view();
        delay(50);
    }

    return 0; 
}
