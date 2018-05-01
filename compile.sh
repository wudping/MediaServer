g++ -I http http/http_client.cpp HLS/hls_client.cpp -o files/client/hls_client  `pkg-config opencv --cflags --libs` -std=c++11
g++ -I http http/http_server.cpp HLS/hls_server.cpp -o files/server/hls_server
g++ -I http http/http_client.cpp MPEG-DASH/dash_client.cpp -o files/client/dash_client `pkg-config opencv --cflags --libs` -std=c++11
g++ -I http http/http_server.cpp MPEG-DASH/dash_server.cpp -o files/server/dash_server

