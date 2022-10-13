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
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/json.h>
#include <fstream>
#include <sys/stat.h>
#include "logger.h"

using namespace std;

/* defines */
#define DESTDIR "/home/user/Documents/Coursework/AOS/Assg/Assg3_18-10-22/2022201024_a3/client"

/* data */
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

string userid = "";
vector<string> createdDirectories;

/* utils */
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

void getArgDetails(string& ip, int& port, string& trackerInfoDest, char* arg[]){
    string ipAndPort = arg[1];
    trackerInfoDest = arg[2];
    int n = ipAndPort.size();
    for(int i=0; i<n; i++){
        if(ipAndPort[i] == ':'){
            ip = ipAndPort.substr(0, i);
            port = atoi(ipAndPort.substr(i+1).c_str());
            return;
        }
    }
}

Tracker getTrackerDetails(string trackerInfoDest){
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
    string id_s = fastWriter.write(jsonData["trackers"][0]["id"]);
    string ip = fastWriter.write(jsonData["trackers"][0]["ip"]);
    ip.pop_back();
    ip.pop_back();
    ip = ip.substr(1);
    string port_s = fastWriter.write(jsonData["trackers"][0]["port"]);
    int id = atoi(id_s.c_str());
    int port = atoi(port_s.c_str());

    Tracker tracker(id, ip, port);
    remove(jsonfile.c_str());
    return tracker;
}

int createUserDirectory(string userId){
    string destinationDirectory = DESTDIR;
    string path=destinationDirectory+"/"+userId;
    string pathDownloads = path+"/downloads";
    string pathUploads = path+"/uploads";
    mode_t perms = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH;
    int d = mkdir(path.c_str(), perms);
    int dd = mkdir(pathDownloads.c_str(), perms);
    int du = mkdir(pathUploads.c_str(), perms);
    if (d==-1 || dd==-1 || du==-1){
        return 1;
    }
    createdDirectories.insert(createdDirectories.begin(), pathDownloads);
    createdDirectories.insert(createdDirectories.begin(), pathUploads);
    createdDirectories.push_back(path);
    return 0;
}

void deleteAllDirs(){
    int stat = 0;
    for(string path : createdDirectories) {
        stat+=rmdir(path.c_str());
    }
    if(stat<0){
        cout<<"**********[<ERROR WHILE CREATING CLIENT DIRECTORY>]**********\n";
    }else{
        cout<<"**********[<SUCCESSFULLY CLEANED CLIENT DIRECTORY>]**********\n";
    }
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
    cout<<"[Peer Server]:    PORT=> "<<*port<<"\n";
    struct Server server = serverConstructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, *port, 20);
    
    struct sockaddr* address = (struct sockaddr*)&server.address;
    socklen_t addressLen = (socklen_t)sizeof(server.address);
    while(true){
        int newSocketFd = accept(server.socket, address, &addressLen);
        char req[1000];
        memset(req, 0, 1000);
        read(newSocketFd, req, 1000);
        string request(req);

        printf("%s\n", req);
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
        serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

        inet_pton(client->domain, serverIp.c_str(), &serverAddress.sin_addr);
        if(connect(client->socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1){
            cout<<"!! ERROR - CANNOT CONNECT SOCKET TO HOST !!\n";
            return res;
        }

        if(send(client->socket, req.c_str(), req.size(), 0) == -1){
            cout<<"!! ERROR - SENDING FROM REQ BUFFER TO CLIENT SOCKET FAILED !!\n";
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

    vector<string> command = processCommand(req);

    // create_user <user_id> <password>
    if(command[0] == "create_user"){
        if(command.size() != 3){
            cout<<"Invalid number of arguments. Try => create_user <user_id> <password>\n";
            return;
        }
        struct Client client = clientConstructor(AF_INET,  SOCK_STREAM, 0, port, INADDR_ANY);
        if(client.socket == -1){
            cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
            return;
        }
        char* res = client.request(&client, tracker.ip, tracker.port, req);
        cout<<"**********["<<res<<"]**********\n";
        string response(res);
        if(response == "<CREATED USER>"){
            if(createUserDirectory(command[1]) == 1){
                cout<<"**********[<ERROR WHILE CREATING CLIENT DIRECTORY>]**********\n";
            }
        }
    }
    
    // login <user_id> <password>
    else if(command[0] == "login"){
        if(command.size() != 3){
            cout<<"Invalid number of arguments. Try => login <user_id> <password>\n";
            return;
        }
        if(userid.size()==0){
            struct Client client = clientConstructor(AF_INET,  SOCK_STREAM, 0, port, INADDR_ANY);
            if(client.socket == -1){
                cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
                return;
            }
            char* res = client.request(&client, tracker.ip, tracker.port, req);
            cout<<"**********["<<res<<"]**********\n";
            string response(res);
            if(response=="<LOGGED IN>") {
                userid = command[1];
            }
        }else{
            cout<<"**********[<ALREADY LOGGED IN AS>: "<<userid<<"]**********\n";
        }
    }
    
    //logout
    else if(command[0] == "logout"){

        if(command.size() != 1){
            cout<<"Invalid number of arguments. Try => logout\n";
            return;
        }

        if(userid.size() == 0){
            cout<<"**********[<NO USER FOUND>]**********\n";
        }else{
            string ui = userid;
            struct Client client = clientConstructor(AF_INET,  SOCK_STREAM, 0, port, INADDR_ANY);
            if(client.socket == -1){
                cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
                return;
            }
            req+=" ";
            req+=userid;
            char* res = client.request(&client, tracker.ip, tracker.port, req);
            
            string response(res);
            if(response == "<LOGGED OUT>"){
                userid = "";
                cout<<"**********[<"<<ui<<" LOGGED OUT>]**********\n";
            }else{
                cout<<"**********["<<response<<"]**********\n";
            }
        }
    }
    
    //create_group <group_id>
    else if(command[0] == "create_group"){
        if(command.size() != 2){
            cout<<"Invalid number of arguments. Try => create_group <group_id>\n";
            return;
        }

        struct Client client = clientConstructor(AF_INET,  SOCK_STREAM, 0, port, INADDR_ANY);
        if(client.socket == -1){
            cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
            return;
        }
        
        if(userid.size()){
            req+=" ";
            req+=userid;
        }

        
        char* res = client.request(&client, tracker.ip, tracker.port, req);
        string response(res);
        if(response == "<CREATED GROUP>"){
            cout<<"**********[<GROUP "<<command[1]<<" CREATED>]**********\n";
        }else{
            cout<<"**********["<<response<<"]**********\n";
        }

    }

    //list_groups
    else if(command[0] == "list_groups"){
        if(command.size() != 1){
            cout<<"Invalid number of arguments. Try => list_groups\n";
            return;
        }

        struct Client client = clientConstructor(AF_INET,  SOCK_STREAM, 0, port, INADDR_ANY);
        if(client.socket == -1){
            cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
            return;
        }
        char* res = client.request(&client, tracker.ip, tracker.port, req);

        string response(res);
        cout<<"**********["<<response<<"]**********\n";

    }

    //join_group <group_id>
    else if(command[0] == "join_group"){
        if(command.size() != 2){
            cout<<"Invalid number of arguments. Try => join_group <group_id>\n";
            return;
        }

        if(userid.size() == 0){
            cout<<"**********[<NO USER FOUND - LOGIN TO CONTINUE>]**********\n";
            return;
        }

        req+=" ";
        req+=userid;

        struct Client client = clientConstructor(AF_INET,  SOCK_STREAM, 0, port, INADDR_ANY);
        if(client.socket == -1){
            cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
            return;
        }
        char* res = client.request(&client, tracker.ip, tracker.port, req);
        string response(res);
        cout<<"**********["<<response<<"]**********\n";
    }

    //list_requests <group_id>
    else if(command[0] == "list_requests"){
        if(command.size() != 2){
            cout<<"Invalid number of arguments. Try => list_requests <group_id>\n";
            return;
        }

        if(userid.size() == 0){
            cout<<"**********[<NO USER FOUND - LOGIN TO CONTINUE>]**********\n";
            return;
        }

        req+=" ";
        req+=userid;

        struct Client client = clientConstructor(AF_INET,  SOCK_STREAM, 0, port, INADDR_ANY);
        if(client.socket == -1){
            cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
            return;
        }
        char* res = client.request(&client, tracker.ip, tracker.port, req);
        string response(res);
        cout<<"**********["<<response<<"]**********\n";

    }

    //accept_request <group_id> <user_id>
    else if(command[0] == "accept_request"){
        if(command.size() != 3){
            cout<<"Invalid number of arguments. Try => accept_request <group_id> <user_id>\n";
            return;
        }

        if(userid.size() == 0){
            cout<<"**********[<NO USER FOUND - LOGIN TO CONTINUE>]**********\n";
            return;
        }

        req+=" ";
        req+=userid;

        struct Client client = clientConstructor(AF_INET,  SOCK_STREAM, 0, port, INADDR_ANY);
        if(client.socket == -1){
            cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
            return;
        }
        char* res = client.request(&client, tracker.ip, tracker.port, req);
        string response(res);
        cout<<"**********["<<response<<"]**********\n";

    }

    //leave_group <group_id>
    else if(command[0] == "leave_group"){
        if(command.size() != 2){
            cout<<"Invalid number of arguments. Try => leave_group <group_id>\n";
            return;
        }

        if(userid.size() == 0){
            cout<<"**********[<NO USER FOUND - LOGIN TO CONTINUE>]**********\n";
            return;
        }

        req+=" ";
        req+=userid;

        struct Client client = clientConstructor(AF_INET,  SOCK_STREAM, 0, port, INADDR_ANY);
        if(client.socket == -1){
            cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
            return;
        }
        char* res = client.request(&client, tracker.ip, tracker.port, req);
        string response(res);
        cout<<"**********["<<response<<"]**********\n";
    }

    //upload_file <file_path> <group_id>
    else if(command[0] == "upload_file"){
        if(command.size() != 3){
            cout<<"Invalid number of arguments. Try => upload_file <file_path> <group_id>\n";
            return;
        }

        if(userid.size() == 0){
            cout<<"**********[<NO USER FOUND - LOGIN TO CONTINUE>]**********\n";
            return;
        }

        req+=" ";
        req+=userid;

        struct Client client = clientConstructor(AF_INET,  SOCK_STREAM, 0, port, INADDR_ANY);
        if(client.socket == -1){
            cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
            return;
        }
        char* res = client.request(&client, tracker.ip, tracker.port, req);
        string response(res);
        cout<<"**********["<<response<<"]**********\n";
    }

    //list_files <group_id>
    else if(command[0] == "list_files"){
        if(command.size() != 2){
            cout<<"Invalid number of arguments. Try => list_files <group_id>\n";
            return;
        }

        struct Client client = clientConstructor(AF_INET,  SOCK_STREAM, 0, port, INADDR_ANY);
        if(client.socket == -1){
            cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
            return;
        }
        char* res = client.request(&client, tracker.ip, tracker.port, req);
        string response(res);
        cout<<"**********["<<response<<"]**********\n";
    }

    else{
        struct Client client = clientConstructor(AF_INET,  SOCK_STREAM, 0, port, INADDR_ANY);
        if(client.socket == -1){
            cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
            return;
        }
        client.request(&client, ip, client.port, req);
    }

}

/* main function */
int main(int n, char* argv[]){

    string ip, trackerInfoDest;
    int port;
    getArgDetails(ip, port, trackerInfoDest, argv);
    tracker = getTrackerDetails(trackerInfoDest);

    //127.0.0.3:2022
    cout<<"[Tracker Used]:    TrackerId=> "<<tracker.id<<" | IP=> "<<tracker.ip<<" | PORT=> "<<tracker.port<<"\n";
    cout<<"[Peer Client]:    IP=> "<<ip<<" | PORT=> "<<port<<"\n";

    // Logger::Info("%d", 3);
    pthread_t serverThread;
    
    int* p = &port;
    if(pthread_create(&serverThread, NULL, server, (void *)p) != 0){
        cout<<"!! ERROR - CANNOT CREATE SERVER THREAD !!\n";
    }

    while(true){
        string req;
        getline(cin, req);
        if(req!="") client(req, ip, port);
    }

    if(pthread_join(serverThread, NULL) != 0){
        cout<<"!! ERROR - PTHREAD_JOIN FAILED !!\n";
    }

    return 0;
}


