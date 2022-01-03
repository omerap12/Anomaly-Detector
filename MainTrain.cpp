#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "Server.h"




using namespace std;

void writeStr(string input,int serverFD){
    write(serverFD,input.c_str(),input.length());
    write(serverFD,"\n",1);
}

string readStr(int serverFD){
    string serverInput="";
    char c=0;
    read(serverFD,&c,sizeof(char));
    while(c!='\n'){
        serverInput+=c;
        read(serverFD,&c,sizeof(char));
    }
    return serverInput;
}

void readMenue(ofstream& out,int serverFD){
    bool done=false;
    while(!done){
        // read string line
        string serverInput = readStr(serverFD);
        if(serverInput=="6.exit")
            done=true;
        out<<serverInput<<endl;
    }
}

int initClient(int port)throw (const char*){
    int serverFD, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    serverFD = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFD < 0)
        throw "socket problem";

    server = gethostbyname("localhost");
    if(server==NULL)
        throw "no such host";

    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);

    serv_addr.sin_port = htons(port);
    if (connect(serverFD,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        throw "connection problem";

    return serverFD;
}

void clientSide1(int port,string outputFile)throw (const char*){
    int serverFD = initClient(port);
    ofstream out(outputFile);
    readMenue(out,serverFD);
    out.close();
    string input="6";
    writeStr(input,serverFD);
    close(serverFD);
    cout<<"end of client 1"<<endl;
}


void clientSide2(int port,string outputFile)throw (const char*){

    int serverFD = initClient(port);

    ofstream out(outputFile);
    ifstream in("input.txt");
    string input="";
    while(input!="6"){
        readMenue(out,serverFD);
        getline(in,input);
        writeStr(input,serverFD);
        if(input=="1"){
            out<<readStr(serverFD)<<endl; // please upload...
            while(input!="done"){
                getline(in,input);
                writeStr(input,serverFD);
            }
            out<<readStr(serverFD)<<endl; // Upload complete
            out<<readStr(serverFD)<<endl; // please upload...
            input="";
            while(input!="done"){
                getline(in,input);
                writeStr(input,serverFD);
            }
            out<<readStr(serverFD)<<endl; // Upload complete
        }

        if(input=="3"){
            out<<readStr(serverFD)<<endl; // Anomaly... complete
        }
        if(input=="5"){
            out<<readStr(serverFD)<<endl; // please upload...
            while(input!="done"){
                getline(in,input);
                writeStr(input,serverFD);
            }
            out<<readStr(serverFD)<<endl; // Upload complete
            out<<readStr(serverFD)<<endl; // TPR
            out<<readStr(serverFD)<<endl; // FPR
        }
    }
    in.close();
    out.close();

    close(serverFD);
    cout<<"end of client 2"<<endl;
}

size_t check(string outputFile,string expectedOutputFile){
    ifstream st(outputFile);
    ifstream ex(expectedOutputFile);
    size_t i=0;
    string lst,lex;
    while(!ex.eof()){
        getline(st,lst);
        getline(ex,lex);
        if(lst.compare(lex)!=0)
            i++;
    }
    st.close();
    ex.close();
    return i;
}


int main(){
    srand (time(NULL));
    int port=5000+ rand() % 1000;
    string outputFile1="output_menu";
    string outputFile2="output";
    int x=rand() % 1000;
    outputFile1+=to_string(x);
    outputFile1+=".txt";
    outputFile2+=to_string(x);
    outputFile2+=".txt";

    try{
        AnomalyDetectionHandler adh;
        Server server(port);
        server.start(adh); // runs on its own thread

        // let's run 2 clients
        clientSide1(port,outputFile1);
        clientSide2(port,outputFile2);
        server.stop(); // joins the server's thread
    }catch(const char* s){
        cout<<s<<endl;
    }
    size_t mistakes = check(outputFile1,"expected_output_menu.txt");
    mistakes += check(outputFile2,"expected_output.txt");

    if(mistakes>0)
        cout<<"you have "<<mistakes<<" mistakes in your output (-"<<(mistakes*2)<<")"<<endl;

    cout<<"done"<<endl;
    return 0;
}
