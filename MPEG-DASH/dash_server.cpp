#include <iostream>
//#include "http_client.hpp"
#include "http_server.hpp"
using namespace std;
int main() {
    HttpServer server(6724, 10);  
    server.respond_to_requests(); 
    return 0;
} 
