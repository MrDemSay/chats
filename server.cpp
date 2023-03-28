#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

#define DEFAULT_PORT 1601
#define ERROR_S "SERVER ERROR: "
#define BUFFER_SIZE 1024
#define CLIENT_CLOSE_CONNECTION_SYMBOL '#'

bool is_client_connection_close(const char* msg);

int main(int args, char const* argv[]) {
    int client;
    int server;

    struct sockaddr_in server_address;

    client = socket(AF_INET, SOCK_STREAM, 0);
    if (client < 0) {
        std::cout << ERROR_S << "establishing socket error." << std::endl;
        exit(0);
    }
    std::cout << "SERVER: socket for server was created." << std::endl;
    
    server_address.sin_port = htons(DEFAULT_PORT);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htons(INADDR_ANY);

    int ret = bind(client, reinterpret_cast<struct sockaddr*>(&server_address), sizeof(server_address));

    if (ret < 0) {
        std::cout << ERROR_S << "binding connection. Socket has already benn establishing. \n";
        return -1;
    }

    socklen_t size = sizeof(server_address);
    std::cout << "SERVER: " << "Listening clients... \n";
    listen(client, 1);

    server = accept(client, reinterpret_cast<struct sockaddr*>(&server_address), &size);
    if(server < 0) {
        std::cout << ERROR_S << "Can`t acceptiong client.\n";
    }

    char buffer[BUFFER_SIZE];
    bool isExit = false;
    while(server > 0) {
        strcpy(buffer, "=> Server connected!\n");
        send(server, buffer, BUFFER_SIZE, 0);
        std::cout << "=> Connected to the client #1 \n"
                  << "Enter" << CLIENT_CLOSE_CONNECTION_SYMBOL << "to end the connection\n\n";
    
        std::cout << "Client: ";
        recv(server, buffer, BUFFER_SIZE, 0);
        std::cout << buffer << std::endl;
        if(is_client_connection_close(buffer)) {
            isExit = true;
        }

        while(!isExit) {
            std::cout << "Server: ";
            std::cin.getline(buffer, BUFFER_SIZE);
            send(server, buffer, BUFFER_SIZE, 0);
            if(is_client_connection_close(buffer)) { 
                break; 
            }
            std::cout << "Client: ";
            recv(server, buffer, BUFFER_SIZE, 0);
            std::cout << buffer << std::endl;
            if(is_client_connection_close(buffer)) {
                break;
            }
        }
        std::cout << "GoodBye. Connection closed. \n";
        isExit = false;
        exit(1);
    }
    return 0;
}

bool is_client_connection_close(const char* msg) {
    for(auto i = 0; i < strlen(msg); ++i) {
        if(msg[i] == CLIENT_CLOSE_CONNECTION_SYMBOL) {
            return true;
        }
    }
    return false;
}