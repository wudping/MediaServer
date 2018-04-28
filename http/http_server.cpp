#include "http_server.hpp"

HttpServer::HttpServer(int port_number, int maxconn) {
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if(sockfd < 0)
        perror("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port_number);
    if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        perror("ERROR on binding");
    listen(sockfd, maxconn);
}

void HttpServer::respond_to_requests() { 
    socklen_t clilen;
    struct sockaddr_in cli_addr;
    while(1) {
        int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);     
        if(newsockfd < 0) {
            perror("Error on accept");
            exit(-1);
        }
        int pid = fork();
        
        if(pid < 0) {
            perror("Error on fork");
            exit(-1);
        }
        if(pid == 0) {
            char request[MAX_BUFFER_SIZE];
            if(recv(newsockfd, request, MAX_BUFFER_SIZE, 0)  < 0) { 
                perror(" server request receive failed " );
                exit(-1);
            }
            char *ptr = strstr(request, "GET");
            ptr += 4;
            char *path_end = strstr(ptr, " ");
            char path[MAX_BUFFER_SIZE];
            strncpy(path, ptr, path_end - ptr);
            FILE * fp = fopen(path, "rb"); 
            if(fp == NULL) { 
                perror("File not found\n");
                exit(-1);
            } 
            char response[MAX_BUFFER_SIZE];
            std :: string message = "HTTP/1.1 200 OK\r\n\r\n";
            if( send(newsockfd,(char*) message.c_str() ,message.length(), 0) < 0) {
                
                printf("send failed top\n");
                perror("Couldn't send to client");
                exit(-1);
            }

            while(1) { 
                bzero(&response, sizeof(response));
                int nbytes = fread(response, 1, MAX_BUFFER_SIZE, fp);
                if(nbytes == 0)
                    break;
                if( send(newsockfd, response, nbytes , 0) < 0) {
                    printf("send failed inside\n");
                    perror("Couldn't send to client");
                    exit(-1);
                }
            }
            close(newsockfd);
        
        }
        else { 
            close(newsockfd); 
        }
    }

}
