#include "http_server.hpp"
#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <unistd.h>
using namespace std;

int video_audio_splitter(string a)
{
    FILE *term_cmd;
    int status;
    
    string b = "ffmpeg -i "+ a + " -s hd480 -c:v libx264 -crf 23 -c:a aac -strict -2 ver_480.mp4";
    cout << "\n Running Command : " << b << "\n" <<  flush;
    term_cmd = popen(b.c_str(), "r");
    if( term_cmd == NULL)
    {
        cout << "cmd failed\n";
        return 0;
    }
    status = pclose(term_cmd);
    if(status == -1)
    {
        cout << "close failed at split\n";
        return 0;
    }
    b = "ffmpeg -y  -i ver_480.mp4 -codec copy  -bsf h264_mp4toannexb  -map 0  -f segment  -segment_time 10  -segment_format mpegts  -segment_list \"./the_480.m3u8\"  -segment_list_type m3u8  \"./the_480_%d.ts\"";
    cout << "\n Running Command : " << b << "\n" << flush;
    term_cmd = popen(b.c_str(), "r");
    if( term_cmd == NULL)
    {
        cout << "cmd failed\n";
        return 0;
    }
    status = pclose(term_cmd);
    if(status == -1)
    {
        cout << "close failed at split\n";
        return 0;
    }
    b = "ffmpeg -y  -i " + a + " -codec copy  -bsf h264_mp4toannexb  -map 0  -f segment  -segment_time 10  -segment_format mpegts  -segment_list \"./the_720.m3u8\"  -segment_list_type m3u8  \"./the_720_%d.ts\"";
    cout << "\n Running Command : " << b << "\n" <<  flush;
    term_cmd = popen(b.c_str(), "r");
    if( term_cmd == NULL)
    {
        cout << "cmd failed\n";
        return 0;
    }
    status = pclose(term_cmd);
    if(status == -1)
    {
        cout << "close failed at split\n";
        return 0;
    }
    ofstream master_file("avengers_trailer.m3u8", ios::out);
    master_file << "#EXTM3U\n";
    master_file << "#EXT-X-STREAM-INF:PROGRAM-ID=1,BANDWIDTH=2149280,CODECS=\"mp4a.40.2,avc1.64001f\",RESOLUTION=1280x720,NAME=\"720\"\n";
    master_file << "the_720.m3u8\n";
    master_file << "#EXT-X-STREAM-INF:PROGRAM-ID=1,BANDWIDTH=836280,CODECS=\"mp4a.40.2,avc1.64001f\",RESOLUTION=848x480,NAME=\"480\"\n";
    master_file << "the_480.m3u8\n";
    master_file.close();
    return 0;
}

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
    video_audio_splitter("correct.mp4");
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
            ptr += 5;
            char *path_end = strstr(ptr, " ");
            char path[MAX_BUFFER_SIZE];
            strncpy(path, ptr, path_end - ptr);
            path[(int)(path_end - ptr)] = '\0';
            printf("Path %s\n", path);
            FILE * fp = fopen(path, "rb"); 
            if(fp == NULL) { 
                perror("File not found\n");
                exit(-1);
            } 
            char response[MAX_BUFFER_SIZE];
            std :: string message = "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\n\r\n";
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
