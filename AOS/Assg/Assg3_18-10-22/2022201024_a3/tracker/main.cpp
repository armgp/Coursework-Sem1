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


/* Data */
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
unordered_map <string, vector<string>> Groups;

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
            }
            else{
                send(newSocketFd, "<CREATE USER FAILED - USERID ALREADY EXISTS>", 45, 0);
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
                send(newSocketFd, "<ALREADY LOGGED IN ANOTHER SYSTEM>", 35, 0);
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

        //create_group <group_id>
        else if(command[0] == "create_group"){
            if(command.size() != 3){
                cout<<"<LOGIN TO CREATE A GROUP>\n";
                send(newSocketFd, "<LOGIN TO CREATE A GROUP>", 26, 0);
            }else{
                string groupid = command[1];
                string userid = command[2];
                if(Groups.find(groupid) == Groups.end()){
                    Groups[groupid].push_back(userid);
                    send(newSocketFd, "<CREATED GROUP>", 16, 0);
                    cout<<"<CREATED> Group: "<<groupid<<" - ADMIN: "<<userid<<"\n";
                }
                else{
                    send(newSocketFd, "<CREATE GROUP FAILED - GROUPID ALREADY EXISTS>", 47, 0);
                    cout<<"<CREATE GROUPFAILED>: Group-Id already exists!\n";
                }
            }
        }

        //list_groups
        else if(command[0] == "list_groups"){
            string response;
            if(Groups.size() == 0) response = "";
            else response = "\n\n";

            int i = 1;
            for(auto group : Groups){
                response+=to_string(i++);
                response+=". ";
                response+=group.first;
                response+="\n";
            }
            
            if(response.size() == 0){
                send(newSocketFd, "<NONE>", 18, 0);
                cout<<"<NO GROUPS EXIST>\n";
            }
            else {
                response+="\n\n";
                send(newSocketFd, response.c_str(), response.size(), 0);
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

        char* res = (char*)malloc(20000);

        struct sockaddr_in serverAddress;
        serverAddress.sin_family = client->domain;
        serverAddress.sin_port = htons(port);
        serverAddress.sin_addr.s_addr = htonl(client->interface);

        inet_pton(client->domain, serverIp.c_str(), &serverAddress.sin_addr);
        if(connect(client->socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1){
            cout<<"!! ERROR - CANNOT CONNECT SOCKET TO HOST !!\n";
            return res;
        }

        if(send(client->socket, req.c_str(), req.size(), 0) == -1){
            cout<<"!! ERROR - SENDING FROM REQ BUFFER TO CLIENT SOCKET FAILED !!\n";
            return res;
        }

        if(req == "quit"){
            if(close(client->socket) == -1){
                cout<<"!! ERROR - CLOSING SOCKET FILE DESCRIPTOR !!\n";
            }
            return res;
        }

        if(read(client->socket, res, 20000) == -1){
            cout<<"!! ERROR - READING FROM CLIENT SOCKET FILE DESCRIPTOR !!\n";
            return res;
        }

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
    if(client.socket == -1){
        cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
        return;
    }
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
    if(pthread_create(&serverThread, NULL, server, (void *)p) != 0){
        cout<<"!! ERROR - CANNOT CREATE SERVER THREAD !!\n";
    }

    while(true){
        string req;
        getline(cin, req);
        if(req!="") client(req, tracker.ip, 2021);
        if(req == "quit") {
            break; 
        }
    }

    if(pthread_join(serverThread, NULL) != 0){
        cout<<"!! ERROR - PTHREAD_JOIN FAILED !!\n";
    }

    return 0;
}