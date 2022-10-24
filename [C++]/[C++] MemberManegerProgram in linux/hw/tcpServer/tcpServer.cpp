
#include "tcpServer.h"

tcpServer::tcpServer(int port)
{
    portNum = port;
    clientState = false;
    createSocket();
}

tcpServer::~tcpServer()
{
    close(serverSock_fd); 
}

int tcpServer::getSeverSocket()
{
    return serverSock_fd;
}

int tcpServer::getClientSocket()
{
    return clientSock_fd;
}


void tcpServer::createSocket()
{
        /* 서버 소켓 생성 */
    if((serverSock_fd = socket(AF_INET/* 인터넷 */, SOCK_STREAM/*TCP*/, 0)) < 0)
    {
        perror("socket()\n");
        return;
    }
    int option = 1;
    setsockopt(serverSock_fd, SOL_SOCKET, SO_REUSEADDR/*해당 주소를 재사용 하겠다.*/, &option, sizeof(option));

    /* 주소 구조체에 주소 지정 */
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // server가 가지고 있는 local IP모두 할당
    serv_addr.sin_port = htons(portNum); /* 사용할 포트 지정 */

    /* bind 함수를 사용하여 서버 소켓의 주소 설정 */
    if(bind(serverSock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("bind()\n");
        return;
    }

    /* 동시에 접속하는 클라이언트의 처리를 위한 대기 큐를 설정 */
    if(listen(serverSock_fd, 8) < 0)  // 버퍼같은 역할, 8개까지 처리하겠다.
    {
        perror("listen()\n");
        return;
    }
}

int tcpServer::waitAccept()
{
    char mesg[BUFSIZ];
    
    /* 클라이언트가 접속하면 접속을 허용하고 클라이언트 소켓 생성 */
    clnt_addr_size = sizeof(client_addr);
    clientSock_fd = accept(serverSock_fd, (struct sockaddr *)&client_addr, &clnt_addr_size);
    if (clientSock_fd < 0)
    {
        perror("accept() error\n");
        return -1;
    }
    /* 네트워크 주소를 문자열로 변경 */
    inet_ntop(AF_INET, &client_addr.sin_addr, mesg, BUFSIZ); 
    printf("Client is connected : %s\n", mesg);

    return clientSock_fd;
     
}


int tcpServer::recvData(char *recvBuff, size_t size)
{
    int len = read(clientSock_fd, recvBuff, size);
    printf("%s", recvBuff);
    return len;
}

int tcpServer::sendData(char *sendBuff, size_t size)
{
    if(!getClientState()) return -1;
    int len = write(clientSock_fd, sendBuff, size);
    printf("%s", sendBuff);
    return len;    
}

void tcpServer::closeSocket(int socket)
{
    close(socket);
}

bool tcpServer::getClientState()
{
    return clientState;
}

void tcpServer::setClientState(bool state)
{
    clientState = state;
}