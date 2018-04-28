#include <iostream>
#include "http_client.hpp"
#include "http_server.hpp"
using namespace std;

int main() {
    HttpClient client(6703, "localhost");  
    client.get("test_send_file.txt", "test_got_file.txt");
    return 0;
} 
