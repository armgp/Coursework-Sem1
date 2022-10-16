/* includes */
#include <iostream>
#include <thread>
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
#include <set>
#include <fstream>
#include "logger.h"

using namespace std;


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

class User {
public:
    string uid;
    string password;
    string ip;
    int port;
    bool live = false;
    unordered_map<string, int> joinedGroups;
    unordered_map<string, int> adminedGroups;
    set<string> files; //filename -> SharedFile

    User(){

    }

    User(string _uid, string _password){
        uid = _uid;
        password = _password;
    }

    int joinGroup(string groupId){
        if(joinedGroups.find(groupId) != joinedGroups.end()) return -1;
        joinedGroups[groupId] = 1;
        return 0;
    }

    int addToAdminedGroups(string groupId){
        if(adminedGroups.find(groupId) != adminedGroups.end()) return -1;
        adminedGroups[groupId] = 1;
        return 0;
    }

};

class Group{
public:
    string adminUserId;
    set<string> userIds;
    set<string> pendingRequests; //userids
    unordered_map<string, vector<string>> shareableFiles; //filename-> <vector>userids
};

unordered_map<string, string> sharedFileDets;
unordered_map <string, User> UsersMap;
unordered_map <string, Group> Groups; //groupId, Group

/* utils */
string getFileName(string filePath){
    int n = filePath.size();
    for(int i=n-1; i>=0; i--){
        if(filePath[i] == '/'){
            return filePath.substr(i+1);
        }
    }

    return filePath;
}

bool doesFileExist(string filePath){
    ifstream test(filePath); 
    return test.good();
}

Tracker getTrackerDetails(string trackerInfoDest, int trackerNo){
    string jsonfile = "traker_info.json";
    ifstream trackerInfoFile;
    trackerInfoFile.open(trackerInfoDest);
    ofstream trackerInfoJson;
    trackerInfoJson.open("traker_info.json");
    string content = "";

    int i = 0;
    for(i=0; !trackerInfoFile.eof(); i++) content += trackerInfoFile.get();
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

struct Client {
    int socket;
    int domain;
    int type;
    int protocol;
    int port;
    u_long interface;

    char* (*request)(struct Client *client, string serverIp, int port, string req);
};

struct ThreadParams {
    int newSocketFd;
    struct Server server;
    bool* status;
};

void processClientRequest(struct ThreadParams params){

        int newSocketFd = params.newSocketFd;
        struct Server server = params.server;
        bool* status = params.status;

        if(!(*status)) return;

        char req[1000];
        memset(req, 0, 1000);
        read(newSocketFd, req, 1000);
        string request(req);

        if(request == "quit"){
            close(server.socket);
            *status = false;
        }

        vector<string> command = processCommand(request);

        // create_user <user_id> <password> 
        if(command[0] == "create_user"){

            if(command.size() != 3){
                cout<<"<INVALID NO OF ARGS>\n";
                send(newSocketFd, "<INVALID NO OF ARGS>", 21, 0);
            }

            else{
                string userId = command[1];
                string password = command[2];
     
                if(UsersMap.find(userId) == UsersMap.end()){
                    User user(userId, password);
                    UsersMap[userId] = user;
                    send(newSocketFd, "<CREATED USER>", 15, 0);
                    cout<<"<CREATED> User:"<<userId<<"\n";
                }
                else{
                    send(newSocketFd, "<CREATE USER FAILED - USERID ALREADY EXISTS>", 45, 0);
                    cout<<"<CREATE USER FAILED>: User-Id already exists!\n";
                }
            }
        }

        // login <user_id> <password> <peer_port> <peer_ip>
        else if(command[0] == "login"){
            string userId = command[1];
            string password = command[2];
            string peerPort = command[3];
            string peerIp = command[4];

            if(UsersMap.find(userId) != UsersMap.end() && 
               UsersMap[userId].live == false && 
               UsersMap[userId].password == password){

                UsersMap[userId].live = true;
                UsersMap[userId].ip = peerIp;
                UsersMap[userId].port = stoi(peerPort);
                cout<<"<LOGGED IN AS>: "<<userId<<"\n";
                send(newSocketFd, "<LOGGED IN>", 12, 0);
            }
            
            else if(UsersMap.find(userId) != UsersMap.end() && UsersMap[userId].live == true){
                cout<<"<USER ALREADY LOGGED ON ANOTHER SYSTEM>\n";
                send(newSocketFd, "<ALREADY LOGGED IN ANOTHER SYSTEM>", 35, 0);
            }

            else{
                cout<<"<INVALID CREDENTIALS>\n";
                send(newSocketFd, "<INVALID CREDENTIALS>", 22, 0);
            }
        }
        
        //logout <user_id>
        else if(command[0] == "logout"){
            string userid = command[1];
            if(UsersMap.find(userid) == UsersMap.end()){
                cout<<"<USER NOT LOGGED IN>\n";
                send(newSocketFd, "<USER NOT LOGGED IN>", 21, 0);
            }
            
            else{
                UsersMap[userid].live = false;
                UsersMap[userid].ip = "";
                UsersMap[userid].port = 0;
                cout<<"<"<<userid<<" LOGGED OUT>\n";
                send(newSocketFd, "<LOGGED OUT>", 21, 0);
            }
        }

        //create_group <group_id> <user_id>
        else if(command[0] == "create_group"){
            if(command.size() != 3){
                cout<<"<LOGIN TO CREATE A GROUP>\n";
                send(newSocketFd, "<LOGIN TO CREATE A GROUP>", 26, 0);
            }else{
                string groupid = command[1];
                string userid = command[2];
                if(Groups.find(groupid) == Groups.end()){
                    Group newGroup;
                    newGroup.userIds.insert(userid);
                    newGroup.adminUserId = userid;
                    Groups[groupid] = newGroup;
                    send(newSocketFd, "<CREATED GROUP>", 16, 0);
                    cout<<"<CREATED> Group: "<<groupid<<" - ADMIN: "<<userid<<"\n";
                    UsersMap[userid].addToAdminedGroups(groupid);
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
                response+="\n";
                send(newSocketFd, response.c_str(), response.size(), 0);
            }
        }

        //join_group <group_id> <user_id>
        else if(command[0] == "join_group"){
            if(command.size() != 3){
                cout<<"<LOGIN TO CREATE A GROUP>\n";
                send(newSocketFd, "<LOGIN TO CREATE A GROUP>", 26, 0);
            }else{
                string groupId = command[1];
                string userId = command[2];
                if(Groups.find(groupId) == Groups.end()){
                    send(newSocketFd, "<GROUP DOESNT EXIST>", 21, 0);
                    cout<<"<ERROR> Group: "<<groupId<<" - DOESN'T EXIST\n";
                }

                else if(UsersMap.find(userId) == UsersMap.end()){
                    send(newSocketFd, "<USER DOESNT EXIST>", 20, 0);
                    cout<<"<ERROR> USER: "<<userId<<" - DOESN'T EXIST\n";
                }

                else if(Groups[groupId].userIds.find(userId) != Groups[groupId].userIds.end()){
                    send(newSocketFd, "<USER ALREADY A PART OF THE GROUP>", 35, 0);
                    cout<<"<USER ALREADY A PART OF THE GROUP>\n";
                }
                
                else{
                    auto pos = Groups[groupId].pendingRequests.find(userId);

                    if(pos != Groups[groupId].pendingRequests.end()){
                        send(newSocketFd, "<REQUEST ALREADY PENDING>", 26, 0);
                        cout<<"<REQUEST ALREADY PENDING>\n";
                    }

                    else {
                        Groups[groupId].pendingRequests.insert(userId);
                        send(newSocketFd, "<REQUEST SEND>", 15, 0);
                        cout<<"<REQUEST SENT>\n";
                    }
                }
            }
        }

        //leave_group <group_id> <user_id>
        else if(command[0] == "leave_group"){
            if(command.size() != 3){
                cout<<"<YOU MUST BE LOGGED IN FOR EXECUTING THIS COMMAND>\n";
                send(newSocketFd, "<YOU MUST BE LOGGED IN FOR EXECUTING THIS COMMAND>", 51, 0);
            }else{
                string groupId = command[1];
                string userId = command[2];
                if(Groups.find(groupId) == Groups.end()){
                    send(newSocketFd, "<GROUP DOESNT EXIST>", 21, 0);
                    cout<<"<ERROR> Group: "<<groupId<<" - DOESN'T EXIST\n";
                }

                else if(UsersMap.find(userId) == UsersMap.end()){
                    send(newSocketFd, "<USER DOESNT EXIST>", 20, 0);
                    cout<<"<ERROR> USER: "<<userId<<" - DOESN'T EXIST\n";
                }
                
                else{
                    auto pos = Groups[groupId].userIds.find(userId);
                    if(pos == Groups[groupId].userIds.end()){
                        send(newSocketFd, "<USER IS NOT A PART OF THIS GROUP>", 35, 0);
                        cout<<"<ERROR> USER: "<<userId<<" - NOT IN THE GROUP\n";
                    }else{
                        if(Groups[groupId].adminUserId == userId){
                            send(newSocketFd, "<ADMIN CANNOT LEAVE>", 21, 0);
                            cout<<"<ERROR> ADMIN: "<<userId<<" - CANNOT LEAVE\n";
                        }
                        else{
                            Groups[groupId].userIds.erase(pos);
                            send(newSocketFd, "<LEFT GROUP>", 35, 0);
                            cout<<"<LEFT> USER: "<<userId<<" - LEFT THE GROUP\n";
                        }
                        
                    }
                }
            }
        }

        //list_requests <group_id> <user_id>
        else if(command[0] == "list_requests"){
            if(command.size() != 3){
                cout<<"<LOGIN TO CREATE A GROUP>\n";
                send(newSocketFd, "<LOGIN TO CREATE A GROUP>", 26, 0);
            }else{
                string groupId = command[1];
                string userId = command[2];
                User user = UsersMap[userId];

                if(Groups.find(groupId) == Groups.end()){
                    cout<<"<ERROR>: GROUP DOESN'T EXIST\n";
                    send(newSocketFd, "<GROUP DOESN'T EXIST>", 22, 0);
                }
                else if( Groups[groupId].adminUserId != userId){
                    cout<<"<ERROR>: ONLY ADMIN OF THE GROUP CAN LIST REQUESTS\n";
                    send(newSocketFd, "<ONLY ADMIN OF THIS GROUP CAN SEE PENDING REQUESTS>", 52, 0);
                }else{
                    if(Groups[groupId].pendingRequests.size() == 0){
                        cout<<"<NO PENDING REQUESTS>\n";
                        send(newSocketFd, "<NONE>", 7, 0);
                    }else{
                        string response = "\n\n";
                        int i = 1;
                        for(string uid : Groups[groupId].pendingRequests){
                            response+=to_string(i++);
                            response+=". ";
                            response+=uid;
                            response+="\n";
                        }
                        response+="\n";
                        send(newSocketFd, response.c_str(), response.size(), 0);
                    }
                }
            }
        }

        //accept_request <group_id> <user_id> <admin_user_id>
        else if(command[0] == "accept_request"){
            if(command.size() != 4){
                cout<<"<LOGIN TO SENT ACCEPT REQUEST>\n";
                send(newSocketFd, "<LOGIN TO SENT ACCEPT REQUEST>", 31, 0);
            }

            else{
                string groupId = command[1];
                string userId = command[2];
                string admin = command[3];

                if(Groups.find(groupId) == Groups.end()){
                    cout<<"<ERROR>: GROUP DOESN'T EXIST\n";
                    send(newSocketFd, "<GROUP DOESN'T EXIST>", 22, 0);
                }

                else if(UsersMap.find(userId) == UsersMap.end()){
                    cout<<"<ERROR>: USER "<<userId<<" DOESN'T EXIST\n";
                    send(newSocketFd, "<USER DOESN'T EXIST>", 21, 0);
                }

                else {
                    User adminUser = UsersMap[admin];
                    if(Groups[groupId].adminUserId != admin){
                        cout<<"<ERROR>: "<<admin<<" IS NOT THE ADMIN\n";
                        send(newSocketFd, "<CURRENT SESSION DOESN'T HAVE AUTHORIZATION OVER THIS GROUP>", 61, 0);
                    }else{
                       
                        auto pos = Groups[groupId].pendingRequests.find(userId);
                    
                        if(pos != Groups[groupId].pendingRequests.end()){
                            Groups[groupId].userIds.insert(userId);
                            Groups[groupId].pendingRequests.erase(pos);
                            cout<<"<ACCEPTED>: "<<userId<<"\n";
                            send(newSocketFd, "<USER IS ACCEPTED TO THE GROUP>", 32, 0);
                        }

                        else{
                            cout<<"<ERROR>: NO PENDING REQUESTS OF "<<userId<<" FOUND\n";
                            send(newSocketFd, "<NO PENDING REQUESTS FOUND>", 28, 0);
                        }
                    }
                }
            }
        }

        //upload_file <file_path> <group_id> <user_id>
        else if(command[0] == "upload_file"){
            if(command.size() != 4){
                cout<<"<LOGIN TO UPLOAD FILES>\n";
                send(newSocketFd, "<LOGIN TO UPLOAD FILES>", 24, 0);
            }

            else{
                string filePath = command[1];
                string groupId = command[2];
                string userId = command[3];

                if(Groups.find(groupId) == Groups.end()){
                    cout<<"<ERROR>: GROUP DOESN'T EXIST\n";
                    send(newSocketFd, "<GROUP DOESN'T EXIST>", 22, 0);
                }

                else if(UsersMap.find(userId) == UsersMap.end()){
                    cout<<"<ERROR>: USER "<<userId<<" DOESN'T EXIST\n";
                    send(newSocketFd, "<USER DOESN'T EXIST>", 21, 0);
                }

                else{
                    if(Groups[groupId].userIds.find(userId) == Groups[groupId].userIds.end()){
                        cout<<"<ERROR>: USER "<<userId<<" IS NOT A PART OF THE GROUP "<<groupId<<"\n";
                        send(newSocketFd, "<USER NOT PART OF THE GROUP>", 29, 0);
                    }else{
                        string fileName = getFileName(filePath);
                        //filename-> <vector>userids
                        Groups[groupId].shareableFiles[fileName].push_back(userId);
                        UsersMap[userId].files.insert(fileName);

                        cout<<"<UPLOADED>\n";
                        send(newSocketFd, "<UPLOADED FILE>", 16, 0);
                    }
                }
            }

        }

        //list_files <group_id>
        else if(command[0] == "list_files"){
            string groupId = command[1];
            if(Groups.find(groupId) == Groups.end()){
                cout<<"<ERROR>: GROUP DOESN'T EXIST\n";
                send(newSocketFd, "<GROUP DOESN'T EXIST>", 22, 0);
            }else{
                if(Groups[groupId].shareableFiles.size() == 0){
                    cout<<"<NO SHARABLE FILES>\n";
                    send(newSocketFd, "<NONE>", 7, 0);
                }
                
                else{
                    string response = "\n\n";
                    int i = 1;
                    for(auto file : Groups[groupId].shareableFiles){
                        response+=to_string(i++);
                        response+=". ";
                        response+=file.first;
                        response+="\n";
                    }
                    response+="\n";
                    send(newSocketFd, response.c_str(), response.size(), 0);
                }
            }
        }

        //download_file <group_id> <file_name> <destination_path> <user_id>
        else if(command[0] == "download_file"){
            if(command.size() != 5){
                cout<<"<LOGIN TO UPLOAD FILES>\n";
                send(newSocketFd, "<LOGIN TO UPLOAD FILES>", 24, 0);
            }
            else{

                string groupId = command[1];
                string fileName = command[2];
                string destinationPath = command[3];
                string userId = command[4];

                if(Groups.find(groupId) == Groups.end()){
                    cout<<"<ERROR>: GROUP DOESN'T EXIST\n";
                    send(newSocketFd, "<ERROR1>", 9, 0);
                }

                else if(Groups[groupId].shareableFiles.find(fileName) == Groups[groupId].shareableFiles.end()){
                    cout<<"<ERROR>: FILE DOESN'T EXIST IN THE GROUP\n";
                    send(newSocketFd, "<ERROR2>", 9, 0);
                }

                else if(Groups[groupId].userIds.find(userId) == Groups[groupId].userIds.end()){
                    cout<<"<ERROR>: USER NOT A MEMBER OF THE GROUP\n";
                    send(newSocketFd, "<ERROR3>", 9, 0);
                }

                else{
                    vector<string> userIds = (Groups[groupId].shareableFiles)[fileName];
                    string res = "";
                    for(string uid : userIds){
                        //uid-ip-port-
                        if(UsersMap[uid].live){
                            res+=uid;
                            res+="-";
                            res+=UsersMap[uid].ip;
                            res+="-";
                            res+=to_string(UsersMap[uid].port);
                            res+=" ";
                        }
                    }
                    res.pop_back();
                    send(newSocketFd, res.c_str(), res.size(), 0);
                    cout<<"<SEND>: META DATA SUCCESFULLY FORWARDED\n";
                }

            }
        }

        //getuserdetails uid
        else if(command[0] == "getuserdetails"){
            string uid = command[1];
            string res = "";

            if(UsersMap.find(uid) == UsersMap.end()){
                cout<<"<ERROR>: USER NOT FOUND -> "<<uid<<"\n";
                send(newSocketFd, "<USER NOT FOUND>", 17, 0);
            }
            else{
                res+=UsersMap[uid].ip;
                res+=" ";
                res+=to_string(UsersMap[uid].port);
                cout<<"<SEND>: USER DATA SUCCESFULLY FORWARDED\n";
                send(newSocketFd, res.c_str(), res.size(), 0);
            }
        }

        else{
            if(request == "quit"){
                cout<<"SHUTTING DOWN.. ";
            }
            else cout<<"INVALID COMMAND!!\n";
        }

        close(newSocketFd);
}

/* server code */
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

void server(int port, string ip){
    cout<<"[Tracker Server]:    PORT=> "<<port<<"\n";
    struct Server server = serverConstructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, port, 20);

    struct sockaddr* address;
    socklen_t addressLen;

    bool status = true;

    while(status){
        int newSocketFd = accept(server.socket, address, &addressLen);
        struct ThreadParams params;
        params.newSocketFd = newSocketFd;
        params.server = server;
        params.status = &status;  

        thread clientReqHandleThread(processClientRequest, params);
        clientReqHandleThread.join();
    }
}


/* client code */
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
    thread serverThread(server, tracker.port, tracker.ip);

    while(true){
        string req;
        getline(cin, req);
        if(req!="") client(req, tracker.ip, 2021);
        if(req == "quit") {
            break; 
        }
    }

    serverThread.join();

    return 0;
}