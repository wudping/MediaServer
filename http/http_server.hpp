#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>


class HttpServer {
    int sockfd;
    struct sockaddr_in serv_addr;
    const int MAX_BUFFER_SIZE = 1024;
public:
    HttpServer(int port_number, int maxconn);
    void respond_to_requests();
};
