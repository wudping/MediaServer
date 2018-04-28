#include <iostream>
#include "http_client.hpp"
#include "http_server.hpp"
using namespace std;

int main() {
    HttpServer server(6703, 1);  
    server.respond_to_requests(); 
    return 0;
} 
