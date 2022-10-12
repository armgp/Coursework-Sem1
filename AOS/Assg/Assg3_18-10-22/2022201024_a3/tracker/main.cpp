/* includes */
#include <iostream>
#include <pthread.h>
#include <netinet/ip.h>
#include <arpa/inet.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/json.h>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "logger.h"

using namespace std;


/* Class Declarations */
class Tracker {
public:
    int id;
    string ip;
    int port;
    Tracker(){}
    Tracker(int _id, string _ip, int _port){
        id = _id;
        ip = _ip;
        port = _port;
    }
} tracker;

unordered_map <string, string> UsersMap;
unordered_map <string, struct sockaddr*> LoggedUsers;

/* utils */
Tracker getTrackerDetails(string trackerInfoDest, int trackerNo){
    string jsonfile = "traker_info.json";
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

    ifstream jsonFile;
    jsonFile.open(jsonfile);
    Json::Value jsonData;
    Json::Reader reader;
    Json::FastWriter fastWriter; //converts Json::Value to string
    

    reader.parse(jsonFile, jsonData);
    string id_s = fastWriter.write(jsonData["trackers"][trackerNo-1]["id"]);
    string ip = fastWriter.write(jsonData["trackers"][trackerNo-1]["ip"]);
    ip.pop_back();
    ip.pop_back();
    ip = ip.substr(1);
    string port_s = fastWriter.write(jsonData["trackers"][trackerNo-1]["port"]);
    int id = atoi(id_s.c_str());
    int port = atoi(port_s.c_str());

    Tracker tracker(id, ip, port);
    remove(jsonfile.c_str());
    return tracker;
}

vector<string> processCommand(string s){
    int n = s.size();
    int st = 0;
    vector<string> res;
    for(int i=0; i<n; i++){
        if(s[i]==' '){
            res.push_back(s.substr(st, (i-st)));
            st = i+1;
        }
    }
    res.push_back(s.substr(st));
    return res;
}

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

void* server(void *arg){
    int* port = (int *)arg;
    cout<<"[Tracker Server]:    PORT=> "<<*port<<"\n";
    struct Server server = serverConstructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, *port, 20);

    // struct sockaddr* address = (struct sockaddr*)&server.address;
    // socklen_t addressLen = (socklen_t)sizeof(server.address);

    struct sockaddr* address;
    socklen_t addressLen;

    while(true){
        int newSocketFd = accept(server.socket, address, &addressLen);
        char req[1000];
        memset(req, 0, 1000);
        read(newSocketFd, req, 1000);
        string request(req);

        if(request == "quit"){
            close(server.socket);
            break;
        }

        vector<string> command = processCommand(request);

        // create_user <user_id> <password>
        if(command[0] == "create_user"){
            string userId = command[1];
            string password = command[2];
            if(UsersMap.find(userId) == UsersMap.end()){
                UsersMap[userId] = password;
                send(newSocketFd, "<CREATED USER>", 15, 0);
                cout<<"<CREATED> User:"<<userId<<"\n";
            }else{
                send(newSocketFd, "<CREATE USER FAILED>", 21, 0);
                cout<<"<CREATE USER FAILED>: User-Id already exists!\n";
            }
        }

        // login <user_id> <password>
        else if(command[0] == "login"){
            string userId = command[1];
            string password = command[2];
            if(UsersMap.find(userId) != UsersMap.end() && 
               LoggedUsers.find(userId)==LoggedUsers.end() && 
               UsersMap[userId] == password){
                LoggedUsers[userId] = address;
                cout<<"<LOGGED IN AS>: "<<userId<<"\n";
                send(newSocketFd, "<LOGGED IN>", 12, 0);
            }else if(UsersMap.find(userId) != UsersMap.end() && LoggedUsers.find(userId)!=LoggedUsers.end()){
                cout<<"<USER ALREADY LOGGED ON ANOTHER SYSTEM>\n";
                send(newSocketFd, "<ALREADY LOGGED IN>", 22, 0);
            }
            else{
                cout<<"<INVALID CREDENTIALS>\n";
                send(newSocketFd, "<INVALID CREDENTIALS>", 22, 0);
            }
        }
        
        //logout
        else if(command[0] == "logout"){
            string userid = command[1];
            if(UsersMap.find(userid) == UsersMap.end()){
                cout<<"<USER NOT LOGGED IN>\n";
                send(newSocketFd, "<USER NOT LOGGED IN>", 21, 0);
            }else{
                UsersMap.erase(userid);
                cout<<"<"<<userid<<" LOGGED OUT>\n";
                send(newSocketFd, "<LOGGED OUT>", 21, 0);
            }
        }

        else{
            cout<<request<<"\n";
        }

        close(newSocketFd);
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

    char* (*request)(struct Client *client, string serverIp, int port, string req);
};

char* request(struct Client *client, string serverIp, int port, string req){
        struct sockaddr_in serverAddress;
        serverAddress.sin_family = client->domain;
        serverAddress.sin_port = htons(port);
        serverAddress.sin_addr.s_addr = htonl(client->interface);

        inet_pton(client->domain, serverIp.c_str(), &serverAddress.sin_addr);
        connect(client->socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

        send(client->socket, req.c_str(), req.size(), 0);

        char* res = (char*)malloc(20000);

        if(req == "quit"){
            close(client->socket);
            return res;
        }

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

    //send request to tracker server(listening at tracer.port)
    client.request(&client, ip, tracker.port, req);
}

/* main function */
int main(int n, char* argv[]){
    string trackerInfoDest = argv[1];
    int trackerNo = atoi(argv[2]);
    tracker = getTrackerDetails(trackerInfoDest, trackerNo);

    cout<<"[Tracker Client]:    TrackerId=> "<<tracker.id<<" | IP=> "<<tracker.ip<<" | PORT=> "<<tracker.port<<"\n";

    // Logger::Info("%d", 3);
    pthread_t serverThread;
    int* p = &tracker.port;
    pthread_create(&serverThread, NULL, server, (void *)p);

    while(true){
        string req;
        getline(cin, req);
        if(req!="") client(req, tracker.ip, 2021);
        if(req == "quit") {
            break; 
        }
    }

    pthread_join(serverThread, NULL);

    return 0;
}