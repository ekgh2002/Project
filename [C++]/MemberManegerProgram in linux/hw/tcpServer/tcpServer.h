#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>


class tcpServer
{
private:
    int serverSock_fd;
    int clientSock_fd;
    int portNum;
    bool clientState;

    struct sockaddr_in serv_addr;
    struct sockaddr_in client_addr;
    socklen_t clnt_addr_size;
    

public:
    tcpServer(int port);
    virtual ~tcpServer();
    int getSeverSocket();
    int getClientSocket();
    bool getClientState();
    void createSocket();
    void setClientState(bool state);
    int waitAccept();
    int recvData(char *recvBuff, size_t size);  /*unsinged long type*/
    int sendData(char *sendBuff, size_t size);
    void closeSocket(int socket);

};

#endif