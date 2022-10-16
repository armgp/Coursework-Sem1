/* includes */
#include <iostream>
#include <thread>
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
#include <limits.h> 
#include <unordered_map>
#include <fcntl.h>
#include <semaphore.h>
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

unordered_map<string, pair<vector<bool>, int>> fileToBitMap;
unordered_map<string, string> fileLocMap; // fileName -> filePath

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

string convertBitMapToString(pair<vector<bool>, int> bitMap){
    vector<bool> bits = bitMap.first;
    int lastChunkSize = bitMap.second;
    string res="";
    for(bool b : bits){
        if(b){
            res+="1";
        }else{
            res+="0";
        }
    }
    res+=" ";
    res+=to_string(lastChunkSize);
    return res;
}

pair<vector<bool>, int> convertStringToBitMap(string str){
    vector<bool> v;
    int lastChunkSize;
    vector<string> infos = processCommand(str);
    lastChunkSize = atoi(infos[1].c_str());
    for(char c : infos[0]){
        if(c == '1'){
            v.push_back(true);
        }else if(c == '0'){
            v.push_back(false);
        }
    }
    return make_pair(v, lastChunkSize);
}

string getFileName(string filePath){
    int n = filePath.size();
    for(int i=n-1; i>=0; i--){
        if(filePath[i] == '/'){
            return filePath.substr(i+1);
        }
    }

    return filePath;
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
        std::cout<<"**********[<ERROR WHILE CREATING CLIENT DIRECTORY>]**********\n";
    }else{
        std::cout<<"**********[<SUCCESSFULLY CLEANED CLIENT DIRECTORY>]**********\n";
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

void server(int port){
    std::cout<<"[Peer Server]:    PORT=> "<<port<<"\n";
    struct Server server = serverConstructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, port, 20);
    
    struct sockaddr* address = (struct sockaddr*)&server.address;
    socklen_t addressLen = (socklen_t)sizeof(server.address);
    while(true){
        int newSocketFd = accept(server.socket, address, &addressLen);
        char req[1000];
        std::memset(req, 0, 1000);
        read(newSocketFd, req, 1000);
        string request(req);

        vector<string> commands = processCommand(request);
        if(commands[0] == "getbitmap"){
            string fileName = commands[1];
            string filePath = fileLocMap[fileName];

            pair<vector<bool>, int> bitMap = fileToBitMap[fileName];
            string stringBitmap = convertBitMapToString(bitMap);

            std::cout<<"<BITMAP SEND>\n";
            std::cout<<stringBitmap<<"\n\n";
            send(newSocketFd, stringBitmap.c_str(), stringBitmap.size(), 0);
        }

        if(commands[0] == "download"){
            string fileName = commands[1];
            int positionOfChunk = stoi(commands[2]);
            string fileLoc = fileLocMap[fileName];
            int totalNoOfChunks = fileToBitMap[fileName].first.size();
            int chunkSize = 524288;
            if(positionOfChunk == totalNoOfChunks-1){
                chunkSize = fileToBitMap[fileName].second;
            }

            ifstream file;
            file.open(fileLoc, ios::binary);
            long offset = positionOfChunk*524288;
            file.seekg(offset, file.beg);
            char buffer[chunkSize];
            file.read(buffer, chunkSize);
            file.close();

            send(newSocketFd, buffer, chunkSize, 0);
            std::cout<<"<CHUNK "<<positionOfChunk<<" SEND>\n";
        }

        else{
            printf("%s\n", req);
        }
        close(newSocketFd);
    }
}


/* client code */
struct Client {
    int socket;
    int domain;
    int type;
    int protocol;
    int port;
    u_long interface;

    char* (*request)(struct Client *client, string serverIp, int port, string req, int resSize, int flag);
};

char* request(struct Client *client, string serverIp, int port, string req, int resSize, int flag){

        char* res = (char*)malloc(resSize);

        struct sockaddr_in serverAddress;
        serverAddress.sin_family = client->domain;
        serverAddress.sin_port = htons(port);
        serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

        inet_pton(client->domain, serverIp.c_str(), &serverAddress.sin_addr);
        if(connect(client->socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1){
            std::cout<<"!! ERROR - CANNOT CONNECT SOCKET TO HOST !!\n";
            return res;
        }

        if(send(client->socket, req.c_str(), req.size(), 0) == -1){
            std::cout<<"!! ERROR - SENDING FROM REQ BUFFER TO CLIENT SOCKET FAILED !!\n";
            return res;
        }

        if(flag == 0){
            if(read(client->socket, res, resSize) == -1){
                std::cout<<"!! ERROR - READING FROM CLIENT SOCKET FILE DESCRIPTOR !!\n";
                return res;
            }

            return res;
        }

        //flag == 1 (compulsorily return resSize buffer)
        int totalSize = 0;
        char* res2 = (char*)malloc(resSize);
        int ind = 0;
        while(totalSize != resSize){
            int currReadSize = read(client->socket, res, resSize);
            totalSize+=currReadSize;
            for(int i=0; (ind<ind+currReadSize && i<currReadSize); i++,ind++){
                res2[ind] = res[i];
            }
            std::memset(res, 0, resSize);
        }
        return res2;
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

void getUsersBitMapThread(string peerIp, int peerPort, string fileName, string peerId, unordered_map<string, string>& userToBitMap, sem_t& mutex){
    struct Client client2 = clientConstructor(AF_INET,  SOCK_STREAM, 0, peerPort, INADDR_ANY);
    if(client2.socket == -1){
        std::cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
        return;
    }
    string getFileBitMapReq = "getbitmap ";
    
    getFileBitMapReq+=fileName;
    char* res2 = client2.request(&client2, peerIp, peerPort, getFileBitMapReq, 20000, 0);
    string stringBitMap(res2);

    sem_wait(&mutex);
    userToBitMap[peerId] = stringBitMap;
    sem_post(&mutex);

    std::cout<<"<BITMAP RECEIVED FROM: >"<<peerId<<"\n";
}

void peerThreadCodeForOnePeer(string peer1, string fileName, int port, string destinationPath, string peerIp, int peerPort){

            struct Client client2 = clientConstructor(AF_INET,  SOCK_STREAM, 0, peerPort, INADDR_ANY);
            if(client2.socket == -1){
                std::cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
                return;
            }

            string getFileBitMapReq = "getbitmap ";
            
            getFileBitMapReq+=fileName;
            char* res2 = client2.request(&client2, peerIp, peerPort, getFileBitMapReq, 20000, 0);
            string stringBitMap(res2);
            pair<vector<bool>, int> bitMapInfo = convertStringToBitMap(stringBitMap);
            std::cout<<"<BITMAP RECEIVED>\n";
       
            vector<bool> bitMap = bitMapInfo.first;
            int lastChunkSize = bitMapInfo.second;
            int chunkSize = 1024*512;
            int noOfChunks = bitMap.size();
            destinationPath+="/";
            destinationPath+=fileName;
            ofstream downloadedFile;
            downloadedFile.open(destinationPath, ofstream::binary|std::ofstream::app);

            for(int i=0; i<noOfChunks; i++){
                int currChunkSize = chunkSize;
                if(i == noOfChunks-1){
                    currChunkSize = lastChunkSize;
                }
                string downloadFileReq = "download ";
                downloadFileReq+=fileName;
                downloadFileReq+=" ";
                downloadFileReq+=to_string(i);
                struct Client client3 = clientConstructor(AF_INET,  SOCK_STREAM, 0, peerPort, INADDR_ANY);
                if(client2.socket == -1){
                    std::cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
                    return;
                }

                char* res3 = client3.request(&client3, peerIp, peerPort, downloadFileReq, currChunkSize, 1);
                string response3(res3);
                
                if(response3 == "<CHUNK DOWNLOAD FAILED>"){
                    std::cout<<"<FAILED TO DOWNLOAD> CHUNK - "<<i<<"\n";
                    return;
                }
                downloadedFile.write(res3, currChunkSize);
            }
        
            downloadedFile.close();

            std::cout<<"<DOWNLOAD SUCCESSFULL>\n";
}

// void downloadChunkFromPeer(string peer1, string fileName, int port, string destinationPath, string peerIp, int peerPort, int chunkNo){

            
//             vector<bool> bitMap = bitMapInfo.first;
//             int lastChunkSize = bitMapInfo.second;
//             int chunkSize = 1024*512;
//             int noOfChunks = bitMap.size();
//             destinationPath+="/";
//             destinationPath+=fileName;
//             ofstream downloadedFile;
//             downloadedFile.open(destinationPath, ofstream::binary|std::ofstream::app);

           
//             int currChunkSize = chunkSize;
//             if(chunkNo == noOfChunks-1){
//                 currChunkSize = lastChunkSize;
//             }

//             string downloadFileReq = "download ";
//             downloadFileReq+=fileName;
//             downloadFileReq+=" ";
//             downloadFileReq+=to_string(chunkNo);
//             struct Client client3 = clientConstructor(AF_INET,  SOCK_STREAM, 0, peerPort, INADDR_ANY);
//             if(client2.socket == -1){
//                 std::cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
//                 return;
//             }

//             char* res3 = client3.request(&client3, peerIp, peerPort, downloadFileReq, currChunkSize, 1);
//             string response3(res3);
            
//             if(response3 == "<CHUNK DOWNLOAD FAILED>"){
//                 std::cout<<"<FAILED TO DOWNLOAD> CHUNK - "<<chunkNo<<"\n";
//                 return;
//             }

//             downloadedFile.write(res3, currChunkSize);
            
        
//             downloadedFile.close();

//             std::cout<<"<DOWNLOADED CHUNK>: "<<chunkNo<<"\n";
// }


void client(string req, string ip, int port) {

    vector<string> command = processCommand(req);

    // create_user <user_id> <password>
    if(command[0] == "create_user"){
        if(command.size() != 3){
            std::cout<<"Invalid number of arguments. Try => create_user <user_id> <password>\n";
            return;
        }
        struct Client client = clientConstructor(AF_INET,  SOCK_STREAM, 0, port, INADDR_ANY);
        if(client.socket == -1){
            std::cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
            return;
        }

        char* res = client.request(&client, tracker.ip, tracker.port, req, 20000, 0);
        std::cout<<"**********["<<res<<"]**********\n";
        string response(res);
        if(response == "<CREATED USER>"){
            if(createUserDirectory(command[1]) == 1){
                std::cout<<"**********[<ERROR WHILE CREATING CLIENT DIRECTORY>]**********\n";
            }
        }
    }
    
    // login <user_id> <password>
    else if(command[0] == "login"){
        if(command.size() != 3){
            std::cout<<"Invalid number of arguments. Try => login <user_id> <password>\n";
            return;
        }

        req+=" ";
        req+=to_string(port);
        req+=" ";
        req+=ip;

        if(userid.size()==0){
            struct Client client = clientConstructor(AF_INET,  SOCK_STREAM, 0, port, INADDR_ANY);
            if(client.socket == -1){
                std::cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
                return;
            }
            char* res = client.request(&client, tracker.ip, tracker.port, req, 20000, 0);
            std::cout<<"**********["<<res<<"]**********\n";
            string response(res);
            if(response=="<LOGGED IN>") {
                userid = command[1];
            }
        }else{
            std::cout<<"**********[<ALREADY LOGGED IN AS>: "<<userid<<"]**********\n";
        }
    }
    
    //logout
    else if(command[0] == "logout"){

        if(command.size() != 1){
            std::cout<<"Invalid number of arguments. Try => logout\n";
            return;
        }

        if(userid.size() == 0){
            std::cout<<"**********[<NO USER FOUND>]**********\n";
        }else{
            string ui = userid;
            struct Client client = clientConstructor(AF_INET,  SOCK_STREAM, 0, port, INADDR_ANY);
            if(client.socket == -1){
                std::cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
                return;
            }
            req+=" ";
            req+=userid;
            char* res = client.request(&client, tracker.ip, tracker.port, req, 20000, 0);
            
            string response(res);
            if(response == "<LOGGED OUT>"){
                userid = "";
                std::cout<<"**********[<"<<ui<<" LOGGED OUT>]**********\n";
            }else{
                std::cout<<"**********["<<response<<"]**********\n";
            }
        }
    }
    
    //create_group <group_id>
    else if(command[0] == "create_group"){
        if(command.size() != 2){
            std::cout<<"Invalid number of arguments. Try => create_group <group_id>\n";
            return;
        }

        struct Client client = clientConstructor(AF_INET,  SOCK_STREAM, 0, port, INADDR_ANY);
        if(client.socket == -1){
            std::cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
            return;
        }
        
        if(userid.size()){
            req+=" ";
            req+=userid;
        }

        
        char* res = client.request(&client, tracker.ip, tracker.port, req, 20000, 0);
        string response(res);
        if(response == "<CREATED GROUP>"){
            std::cout<<"**********[<GROUP "<<command[1]<<" CREATED>]**********\n";
        }else{
            std::cout<<"**********["<<response<<"]**********\n";
        }

    }

    //list_groups
    else if(command[0] == "list_groups"){
        if(command.size() != 1){
            std::cout<<"Invalid number of arguments. Try => list_groups\n";
            return;
        }

        struct Client client = clientConstructor(AF_INET,  SOCK_STREAM, 0, port, INADDR_ANY);
        if(client.socket == -1){
            std::cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
            return;
        }
        char* res = client.request(&client, tracker.ip, tracker.port, req, 20000, 0);

        string response(res);
        std::cout<<"**********["<<response<<"]**********\n";

    }

    //join_group <group_id>
    else if(command[0] == "join_group"){
        if(command.size() != 2){
            std::cout<<"Invalid number of arguments. Try => join_group <group_id>\n";
            return;
        }

        if(userid.size() == 0){
            std::cout<<"**********[<NO USER FOUND - LOGIN TO CONTINUE>]**********\n";
            return;
        }

        req+=" ";
        req+=userid;

        struct Client client = clientConstructor(AF_INET,  SOCK_STREAM, 0, port, INADDR_ANY);
        if(client.socket == -1){
            std::cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
            return;
        }
        char* res = client.request(&client, tracker.ip, tracker.port, req, 20000, 0);
        string response(res);
        std::cout<<"**********["<<response<<"]**********\n";
    }

    //list_requests <group_id>
    else if(command[0] == "list_requests"){
        if(command.size() != 2){
            std::cout<<"Invalid number of arguments. Try => list_requests <group_id>\n";
            return;
        }

        if(userid.size() == 0){
            std::cout<<"**********[<NO USER FOUND - LOGIN TO CONTINUE>]**********\n";
            return;
        }

        req+=" ";
        req+=userid;

        struct Client client = clientConstructor(AF_INET,  SOCK_STREAM, 0, port, INADDR_ANY);
        if(client.socket == -1){
            std::cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
            return;
        }
        char* res = client.request(&client, tracker.ip, tracker.port, req, 20000, 0);
        string response(res);
        std::cout<<"**********["<<response<<"]**********\n";

    }

    //accept_request <group_id> <user_id>
    else if(command[0] == "accept_request"){
        if(command.size() != 3){
            std::cout<<"Invalid number of arguments. Try => accept_request <group_id> <user_id>\n";
            return;
        }

        if(userid.size() == 0){
            std::cout<<"**********[<NO USER FOUND - LOGIN TO CONTINUE>]**********\n";
            return;
        }

        req+=" ";
        req+=userid;

        struct Client client = clientConstructor(AF_INET,  SOCK_STREAM, 0, port, INADDR_ANY);
        if(client.socket == -1){
            std::cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
            return;
        }
        char* res = client.request(&client, tracker.ip, tracker.port, req, 20000, 0);
        string response(res);
        std::cout<<"**********["<<response<<"]**********\n";

    }

    //leave_group <group_id>
    else if(command[0] == "leave_group"){
        if(command.size() != 2){
            std::cout<<"Invalid number of arguments. Try => leave_group <group_id>\n";
            return;
        }

        if(userid.size() == 0){
            std::cout<<"**********[<NO USER FOUND - LOGIN TO CONTINUE>]**********\n";
            return;
        }

        req+=" ";
        req+=userid;

        struct Client client = clientConstructor(AF_INET,  SOCK_STREAM, 0, port, INADDR_ANY);
        if(client.socket == -1){
            std::cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
            return;
        }
        char* res = client.request(&client, tracker.ip, tracker.port, req, 20000, 0);
        string response(res);
        std::cout<<"**********["<<response<<"]**********\n";
    }

    //upload_file <file_path> <group_id>
    else if(command[0] == "upload_file"){
        if(command.size() != 3){
            std::cout<<"Invalid number of arguments. Try => upload_file <file_path> <group_id>\n";
            return;
        }

        if(userid.size() == 0){
            std::cout<<"**********[<NO USER FOUND - LOGIN TO CONTINUE>]**********\n";
            return;
        }

        char buf[PATH_MAX]; 
        char *result = realpath(command[1].c_str(), buf);
        if(!result){
            std::cout<<"**********[<FILE NOT FOUND>]**********\n";
            return;
        }
        string buffer(buf);

        req="";
        req+=command[0];
        req+=" ";
        req+=buffer;
        req+=" ";
        req+=command[2];
        req+=" ";
        req+=userid;

        struct Client client = clientConstructor(AF_INET,  SOCK_STREAM, 0, port, INADDR_ANY);
        if(client.socket == -1){
            std::cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
            return;
        }
        char* res = client.request(&client, tracker.ip, tracker.port, req, 20000, 0);
        string response(res);
        std::cout<<"**********["<<response<<"]**********\n";
        if(response == "<UPLOADED FILE>"){
            string fileName = getFileName(buffer);

            FILE * pFile;
            pFile = fopen (buf , "r");

            int sz=0;
            if (pFile == NULL) perror ("Error opening file");
            else{
                fseek(pFile, 0L, SEEK_END);
                sz = ftell(pFile);
            }
            fclose(pFile);
            
            int chunkSize = 512*1024;
            int noOfChunks = sz/chunkSize;
            int bytesLeft = sz - noOfChunks*chunkSize;
            // vector<pair<bool,int>> bitmap(noOfChunks, make_pair(1, chunkSize));
            
            int lastChunkSize;
            if(bytesLeft > 0){
                lastChunkSize = bytesLeft;
                noOfChunks++;
            }
            vector<bool> bitMap(noOfChunks, true);
            fileToBitMap[fileName] = make_pair(bitMap, lastChunkSize);

            // if(bytesLeft > 0){
            //     bitmap.push_back(make_pair(1, bytesLeft));
            // }
            
            // fileMap[fileName] = bitmap;
            fileLocMap[fileName] = buffer;
        }
    }

    //list_files <group_id>
    else if(command[0] == "list_files"){
        if(command.size() != 2){
            std::cout<<"Invalid number of arguments. Try => list_files <group_id>\n";
            return;
        }

        struct Client client = clientConstructor(AF_INET,  SOCK_STREAM, 0, port, INADDR_ANY);
        if(client.socket == -1){
            std::cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
            return;
        }
        char* res = client.request(&client, tracker.ip, tracker.port, req, 20000, 0);
        string response(res);
        std::cout<<"**********["<<response<<"]**********\n";
    }

    //download_file <group_id> <file_name> <destination_path>
    else if(command[0] == "download_file"){
        if(command.size() != 4){
            std::cout<<"Invalid number of arguments. Try => download_file <group_id> <file_name> <destination_path>\n";
            return;
        }

        if(userid.size() == 0){
            std::cout<<"**********[<NO USER FOUND - LOGIN TO CONTINUE>]**********\n";
            return;
        }

        char buf[PATH_MAX]; 
        std::memset(buf, 0, PATH_MAX);
        char *result = realpath(command[3].c_str(), buf);
        
        if(!result){
            std::cout<<"**********[<DESTINATION NOT FOUND>]**********\n";
            return;
        }

        req+=" ";
        req+=userid;

        struct Client client = clientConstructor(AF_INET,  SOCK_STREAM, 0, port, INADDR_ANY);
        if(client.socket == -1){
            std::cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
            return;
        }


        char* res = client.request(&client, tracker.ip, tracker.port, req, 20000, 0);
        string response(res);
        std::memset(res, 0, 20000);

        if(response == "<ERROR1>"){
            std::cout<<"**********[<GROUP DOESN'T EXIST>]**********\n";
        }else if(response == "<ERROR2>"){
            std::cout<<"**********[<FILE DOESN'T EXIST IN THE GROUP>]**********\n";
        }else if(response == "<ERROR3>"){
            std::cout<<"**********[<USER NOT A MEMBER OF THE GROUP>]**********\n";
        }else{

            std::cout<<"**********[<USERS WITH FILE>: "<<response<<"]**********\n";

            //
            //considering response only has one user id
            //
            vector<string> usersInfo = processCommand(response);
            vector<vector<string>> userDetails;
            for(string userInfo : usersInfo){
                vector<string> tokens;
                string delimiter = "-";

                size_t pos = 0;
                string token;
                while ((pos = userInfo.find(delimiter)) != string::npos) {
                    token = userInfo.substr(0, pos);
                    tokens.push_back(token);
                    userInfo.erase(0, pos + delimiter.length());
                }
                tokens.push_back(userInfo);
                string peerUserId = tokens[0];
                string peerIp = tokens[1];
                string peerPort = tokens[2];
                userDetails.push_back({peerUserId, peerIp, peerPort});
            }

            // string destinationPath = command[3];
            // string peerId = userDetails[0][0];
            // string peerIp = userDetails[0][1];
            // int peerPort = atoi(userDetails[0][2].c_str());
            // thread peerThread(peerThreadCodeForOnePeer, response, command[2], port, destinationPath, peerIp, peerPort);
            // peerThread.join();

            //
            //considering multiple users with file
            //
            
            unordered_map<string, string> userToBitMap;
            string fileName = command[2];
            vector<thread> bitMapThreads;

            sem_t mutex;
            sem_init(&mutex, 0, 1);
            for(vector<string> usDet : userDetails){
                string peerId = usDet[0];
                string peerIp = usDet[1];
                int peerPort = atoi(usDet[2].c_str());
                bitMapThreads.push_back(thread(getUsersBitMapThread, peerIp, peerPort, fileName, peerId, ref(userToBitMap), ref(mutex)));
            }

            for (thread &t : bitMapThreads) {
                if(t.joinable()) t.join();
            }
            sem_destroy(&mutex);
            
            bitMapThreads.clear();
            std::cout<<"<ALL BITMAPS RECEIVED>\n";

            for(auto ub : userToBitMap){
                cout<<ub.first<<"--> "<<ub.second<<"\n";
            }


        }
    }

    else{
        struct Client client = clientConstructor(AF_INET,  SOCK_STREAM, 0, port, INADDR_ANY);
        if(client.socket == -1){
            std::cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
            return;
        }
        client.request(&client, ip, client.port, req, 20000, 0);
    }

}


/* main function */
int main(int n, char* argv[]){

    string ip, trackerInfoDest;
    int port;
    getArgDetails(ip, port, trackerInfoDest, argv);
    tracker = getTrackerDetails(trackerInfoDest);

    //127.0.0.3:2022
    std::cout<<"[Tracker Used]:    TrackerId=> "<<tracker.id<<" | IP=> "<<tracker.ip<<" | PORT=> "<<tracker.port<<"\n";
    std::cout<<"[Peer Client]:    IP=> "<<ip<<" | PORT=> "<<port<<"\n";

    thread serverThread(server, port);

    while(true){
        string req;
        getline(cin, req);
        if(req!="") client(req, ip, port);
    }

    serverThread.join();

    return 0;
}

