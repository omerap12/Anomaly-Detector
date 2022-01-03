#include "Server.h"
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <sys/socket.h>

Server::Server(int port)throw (const char*) {
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0){
        throw "socket failed";
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);
    int len = sizeof server;

    int checkBind = bind(fd, (struct sockaddr *) &server, len);
    if (checkBind < 0){
        throw "bind failure";
    }
    int checkListening = listen(fd, 3);
    if (checkListening < 0){
        throw "listen failure";
    }
}


void Server::start(ClientHandler& ch)throw(const char*){
    does_connect = true;
    t = new thread([&ch, this](){
        while (does_connect){
            socklen_t sizeOfClient = sizeof(client);
            int clientA = accept(fd,(struct sockaddr *) &client, &sizeOfClient);
            if (clientA < 0){
                throw "connection error";
            }
            ch.handle(clientA);
            close(clientA);
        }
    });
}

void Server::stop(){
    does_connect = false;
    t->join(); 
    close(fd);

}

Server::~Server() {
}

