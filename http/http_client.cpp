#include "http_client.hpp"

HttpClient::HttpClient(int port_number, std::string h) { 
    host = (char*) h.c_str();
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror(" error in making socket : ");
        exit(-1);
    }

    bzero( & server, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_port = htons(port_number);


    struct hostent *server_addr = gethostbyname(host);
    if(server_addr == NULL) {
        printf("No such hosts found\n");
        exit(-1);
    }
    memcpy(&server.sin_addr.s_addr,server_addr->h_addr,server_addr->h_length);

    if(connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
        perror(" connection to server failed \n");
        exit(-1);
    }
}

void HttpClient::get(std::string p, std::string f) {
    char *path = (char*) p.c_str();
    char *file = (char*) f.c_str();
    char * message = NULL;
    int len_msg = 0;
    len_msg += strlen("%s %s%s%s HTTP/1.0\r\n");
    len_msg += strlen(path);
    len_msg += strlen("\r\n");

    message = (char*) malloc(len_msg);
    sprintf(message, "%s %s HTTP/1.0\r\n",
            "GET", path);
    strcat(message, "\r\n"); 

    int send_size = strlen(message);

    if( (send_size = send(sock, message, strlen(message), 0)) < 0) {
        perror( "request send failed " );
        exit(-1);
    }    
    
    FILE *fp = fopen(file, "wb");
    int first_time = 1;
    char response[MAX_BUFFER_SIZE];
    while(1) {
        bzero(&response, sizeof(response));
        int status = recv(sock, response, MAX_BUFFER_SIZE, 0);
        if(status < 0) {
            perror(" receive failed ");
            exit(-1);
        }
        if(status == 0)
            break;
        if(first_time) {
            char new_response[MAX_BUFFER_SIZE];
            int ind = 0;
            while(ind < status && !(response[ind] == '\r' && response[ind + 1] == '\n' && response[ind + 2] == '\r' && response[ind + 3] == '\n'))
                ind++;
            ind += 4;
            int cnt = 0;
            while(ind < status) {
                new_response[cnt] = response[ind];
                ind++;
                cnt++;
            }
            fwrite(new_response, 1, cnt, fp);
            first_time = 0;
        }
        else {
            fwrite(response, 1, status, fp);
        }
    }
}
