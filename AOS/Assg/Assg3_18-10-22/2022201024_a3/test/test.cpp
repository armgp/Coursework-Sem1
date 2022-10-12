/* includes */
#include <iostream>
#include <pthread.h>
// #include <sys/socket.h>
//#include <netinit/in.h> both subset of #include <netinet/ip.h>
#include <netinet/ip.h>

#include <arpa/inet.h> //inet_pton
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //write read functions

#include "logger.h"

using namespace std;

/* server code */
struct Server {
    //domain: integer, specifies communication domain.
    // We use AF_ LOCAL as defined in the POSIX standard for 
    //communication between processes on the same host. 
    //For communicating between processes on different hosts connected by IPV4,
    // we use AF_INET and AF_I NET6 for processes connected by IPV6.
    int domain;

    //communication type
    //SOCK_STREAM: TCP(reliable, connection oriented)
    //SOCK_DGRAM: UDP(unreliable, connectionless)
    int type;

    int protocol;
    
    //network interface (INDARR_ANY gives all interfaces, not just any random one)
    u_long interface;

    //the port to which the application must bind.
    int port;

    //The backlog argument defines the maximum length to which the
    //queue of pending connections for sockfd may grow.  If a
    //connection request arrives when the queue is full, the client may
    //receive an error with an indication of ECONNREFUSED or, if the
    //underlying protocol supports retransmission, the request may be
    //ignored so that a later reattempt at connection succeeds.
    int backlog;

    struct sockaddr_in address;

    //socket descriptor, an integer (like a file-handle)
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

    //This convention, known as network byte order (big endian), 
    //defines the bit-order of network addresses as 
    //they pass through the network. 
    //The TCP/IP standard network byte order is big-endian. 
    //In order to participate in a TCP/IP network, 
    //little-endian systems usually bear the burden of conversion 
    //to network byte order. (hton and htonl)
    //https://www.ibm.com/docs/ja/zvm/7.2?topic=domains-network-byte-order-host-byte-order
    server.address.sin_port = htons(port);
    server.address.sin_addr.s_addr = htonl(interface);

    server.socket = socket(domain, type, protocol);

    //Upon successful completion, socket() shall return a non-negative integer, the socket file descriptor. 
    //Otherwise, a value of -1 shall be returned and errno set to indicate the error.
    if(server.socket < 0){
        perror("Failed to connect to socket\n");
        exit(1);
    }

    //bind socket to an address
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

    //1.ipv4
    //2. tcp
    //3. protocol of 0 causes socket() to use an unspecified default protocol appropriate for the requested socket type,
    //4. When a process wants to receive new incoming packets or connections, it should bind a socket to a local interface address using bind(2).
    //   In this case, only one IP socket may be bound to any given local (address, port) pair. 
    //   When INADDR_ANY is specified in the bind call, the socket will be bound to all local interfaces.
    struct Server server = serverConstructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 1100, 20);
    
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

        //make connection
        //----------------
        //This function converts the character string serverIp into a network
        //address structure in the client->domain address family, then copies the
        //network address structure to serverAddress.sin_adr. 
        //The client->domain argument must be either AF_INET or AF_INET6.
        inet_pton(client->domain, serverIp.c_str(), &serverAddress.sin_addr);
        //The connect() system call connects the socket referred to by the
        //file descriptor client->socket to the address specified by serverAddress.The
        //addrlen argument specifies the size of address.  The format of the
        //address in addr is determined by the address space of the socket
        //sockfd
        connect(client->socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

        //send req
        send(client->socket, req.c_str(), req.size(), 0);

        //read res
        char* res = (char*)malloc(20000);
        read(client->socket, res, 20000);
        return res;
}

struct Client clientConstructor(int domain, int type, int protocol, int port, u_long interface){
    struct Client client;
    client.domain = domain;
    client.port = port;
    client.interface = interface;

    //socket connection established
    client.socket = socket(domain, type, protocol);
    client.request = request;
    return client;
}

void client(string req) {
    struct Client client = clientConstructor(AF_INET,  SOCK_STREAM, 0, 1100, INADDR_ANY);
    client.request(&client, "127.0.0.1", req);
    cout<<"---";
}

/* main function */
int main(){
    // create the server using a thread
    cout<<"Strating server...\n";
    // Logger::Info("%d", 3);
    pthread_t serverThread;
    pthread_create(&serverThread, NULL, server, NULL);

    while(true){
        string req;
        cin>>req;
        client(req);
    }

    return 0;
}