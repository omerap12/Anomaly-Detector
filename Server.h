#ifndef SERVER_H_
#define SERVER_H_
#include <thread>
#include "commands.h"
#include "CLI.h"
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

class ClientHandler{
public:
    virtual void handle(int clientID)=0;
};


class dioForSocket : public DefaultIO {
    int socketFileDescriptor;
public:
    dioForSocket(int fileD) {
        this->socketFileDescriptor = fileD;
    }

    void write (float f) override {
        ostringstream ss;
        ss <<f;
        string s(ss.str());
        write(s);
    }

    void write (string text) override {
        int flag = 0;
        const char* textChar = text.c_str();
        send(this->socketFileDescriptor, textChar, text.size(), flag);
    }

    string read() override{
        int flag = 0;
        string inputToServer = "";
        char end = '0';
        recv(this->socketFileDescriptor, &end, sizeof(char), flag);
        while (end != '\n') {
            inputToServer = inputToServer + end;
            recv(this->socketFileDescriptor, &end, sizeof(char), flag);

        }
        return inputToServer;
    }

    void read (float *f) override {
        int flag = 0;
        recv(this->socketFileDescriptor, f, sizeof(*f), flag);
    }
};


class AnomalyDetectionHandler:public ClientHandler{
public:
    virtual void handle(int clientID){
        dioForSocket dio(clientID);
        CLI c (&dio);;
        c.start();
    }
};


class Server {
    thread* t;

public:
    bool does_connect;
    int fd;
    struct sockaddr_in server;
    struct sockaddr_in client;
    Server(int port) throw (const char*);
    virtual ~Server();
    void start(ClientHandler& ch) throw(const char*);
    void stop();
};

#endif /* SERVER_H_ */
