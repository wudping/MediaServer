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
#include <iostream>

class HttpClient {
    int sock;
    struct sockaddr_in server;
    char * host;
    const int MAX_BUFFER_SIZE = 1024;
public:
    HttpClient(int port_number, std::string host);
    void get(std::string path, std::string file);
};
