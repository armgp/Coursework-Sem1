/* includes */
#include <iostream>
#include <pthread.h>
#include <netinet/ip.h>
#include <arpa/inet.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <vector>
#include "logger.h"
#include <fstream>
/* defines */
#define TRACKERPORT 2020

using namespace std;

/* server code */
struct Server {
    int domain;
    int type;
    int protocol;
    u_long interface;
    int port;
    int backlog;
    struct sockaddr_in address;
    int socket;
};

struct Server serverConstructor(int domain, int type, int protocol, u_long interface, int port, int backlog){

    struct Server server;

    server.domain = domain;
    server.type = type;
    server.protocol = protocol;
    server.interface = interface;
    server.port = port;
    server.backlog = backlog;
    server.address.sin_family = domain;
    server.address.sin_port = htons(port);
    server.address.sin_addr.s_addr = htonl(interface);

    server.socket = socket(domain, type, protocol);

    if(server.socket < 0){
        perror("Failed to connect to socket\n");
        exit(1);
    }

    if((bind(server.socket, (struct sockaddr*)&server.address, sizeof(server.address))) < 0){
        perror("Failed to bind socket\n");
        exit(1);
    }

    if((listen(server.socket, server.backlog)) < 0){
        perror("Failed to start listening\n");
        exit(1);
    }

    return server;
}

void* server(void *args){

    cout<<"Server running..\n";
    struct Server server = serverConstructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 2022, 20);
    
    struct sockaddr* address = (struct sockaddr*)&server.address;
    socklen_t addressLen = (socklen_t)sizeof(server.address);
    while(true){
        //It extracts the first connection request on the queue of pending connections for the
        //listening socket, sockfd, creates a new connected socket, and
        //returns a new file descriptor referring to that socket.  The
        //newly created socket is not in the listening state.  The original
        //socket sockfd is unaffected by this call.
        int client = accept(server.socket, address, &addressLen);

        char req[1000];
        memset(req, 0, 1000);
        read(client, req, 1000);
        printf("%s\n", req);
        close(client);
    }
    return NULL;
}


/* client code */

struct Client {
    int socket;
    int domain;
    int type;
    int protocol;
    int port;
    u_long interface;

    char* (*request)(struct Client *client, string serverIp, string req);
};

char* request(struct Client *client, string serverIp, string req){
        struct sockaddr_in serverAddress;
        serverAddress.sin_family = client->domain;
        serverAddress.sin_port = htons(client->port);
        serverAddress.sin_addr.s_addr = htonl(client->interface);

        inet_pton(client->domain, serverIp.c_str(), &serverAddress.sin_addr);
        connect(client->socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

        send(client->socket, req.c_str(), req.size(), 0);

        char* res = (char*)malloc(20000);
        read(client->socket, res, 20000);

        return res;
}

struct Client clientConstructor(int domain, int type, int protocol, int port, u_long interface){
    struct Client client;
    client.domain = domain;
    client.port = port;
    client.interface = interface;

    client.socket = socket(domain, type, protocol);
    client.request = request;
    return client;
}

void client(string req, string ip, int port) {
    struct Client client = clientConstructor(AF_INET,  SOCK_STREAM, 0, port, INADDR_ANY);
    client.request(&client, ip, req);
}

/* main function */
int main(int n, char* argv[]){
    string ipAndPort = argv[1];
    string trackerInfoDest = argv[2];
    string ip;
    int port;
    for(int i=0; i<ipAndPort.size(); i++){
        if(ipAndPort[i] == ':'){
            ip = ipAndPort.substr(0, i);
            port = atoi(ipAndPort.substr(i+1).c_str());
            break;
        }
    }

    //127.0.0.3:2022
    cout<<"Strating Client Server at "<<ip<<":"<<port<<"\n";

    ifstream trackerInfoFile;
    trackerInfoFile.open(trackerInfoDest);
    ofstream trackerInfoJson;
    trackerInfoJson.open("traker_info.json");
    string content = "";

    int i = 0;
    for(i=0; !trackerInfoFile.eof(); i++) 
    content += trackerInfoFile.get();
    trackerInfoFile.close();
    content.erase(content.end()-1);  
    i--;

    trackerInfoJson << content;
    trackerInfoJson.close();
    
    // Logger::Info("%d", 3);
    pthread_t serverThread;
    pthread_create(&serverThread, NULL, server, NULL);

    while(true){
        string req;
        cin>>req;
        client(req, ip, port);
    }

    return 0;
}