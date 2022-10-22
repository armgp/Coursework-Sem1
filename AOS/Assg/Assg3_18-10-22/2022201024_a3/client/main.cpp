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
#include <openssl/sha.h>
#include <algorithm>
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
unordered_map<string, pair<string, char>> downloadedFiles; //fileName*gid -> D/C

vector<string> createdDirectories;

unordered_map<string, string> dynamicDownloadsShaMap;

/* utils */
vector<string> divideStringByChar(string s, char c){
    int n = s.size();
    int st = 0;
    vector<string> res;
    for(int i=0; i<n; i++){
        if(s[i]==c){
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
    vector<string> infos = divideStringByChar(str, ' ');
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

vector<Tracker> getTrackerDetails(string trackerInfoDest){
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
    
    string id_s1 = fastWriter.write(jsonData["trackers"][0]["id"]);
    string id_s2 = fastWriter.write(jsonData["trackers"][1]["id"]);

    string ip1 = fastWriter.write(jsonData["trackers"][0]["ip"]);
    ip1.pop_back();
    ip1.pop_back();
    ip1 = ip1.substr(1);
    string ip2 = fastWriter.write(jsonData["trackers"][1]["ip"]);
    ip2.pop_back();
    ip2.pop_back();
    ip2 = ip2.substr(1);

    string port_s1 = fastWriter.write(jsonData["trackers"][0]["port"]);
    int id1 = atoi(id_s1.c_str());
    int port1 = atoi(port_s1.c_str());
    string port_s2 = fastWriter.write(jsonData["trackers"][1]["port"]);
    int id2 = atoi(id_s2.c_str());
    int port2 = atoi(port_s2.c_str());

    vector<Tracker> res;
    Tracker tracker1(id1, ip1, port1);
    Tracker tracker2(id2, ip2, port2);
    res.push_back(tracker1);
    res.push_back(tracker2);

    remove(jsonfile.c_str());
    return res;
}

int createUserDirectory(string userId){
    string destinationDirectory = DESTDIR;
    string path=destinationDirectory+"/"+userId;
    mode_t perms = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH;
    int d = mkdir(path.c_str(), perms);
    if (d==-1){
        return 1;
    }
    return 0;
}

string getShaOfString(string value){
    int i = 0;
    unsigned char temp[SHA_DIGEST_LENGTH];
    char buf[SHA_DIGEST_LENGTH*2];

    memset(buf, 0x0, SHA_DIGEST_LENGTH*2);
    memset(temp, 0x0, SHA_DIGEST_LENGTH);

    SHA1((unsigned char *)value.c_str(), value.size(), temp);

    for (i=0; i < SHA_DIGEST_LENGTH; i++) {
        sprintf((char*)&(buf[i*2]), "%02x", temp[i]);
    }

    string res(buf);
    return res;
}

string getChunkWiseSha(string path){
    char buffer[524288];
    int readByte;
    string res = "";

    int fd = open(path.c_str(), O_RDONLY);
    if (fd < 0)
        return "-1";
    else{
        while ((readByte = read(fd, buffer, 524288)) > 0){
            string buf(buffer);
            res+=getShaOfString(buf);
            memset(buffer, 0, 524288);
        }
    }

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

        vector<string> commands = divideStringByChar(request, ' ');
        if(commands[0] == "getbitmap"){
            string fileName = commands[1];
            string filePath = fileLocMap[fileName];

            pair<vector<bool>, int> bitMap = fileToBitMap[fileName];
            string stringBitmap = convertBitMapToString(bitMap);

            std::cout<<"<BITMAP SEND>\n";
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
            std::memset(buffer, 0, chunkSize);
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
    close(client2.socket);
}

void downloadChunkFromPeer(string fileName, string peerIp, int peerPort, int chunkNo, int currChunkSize, int fd){
    
    string downloadFileReq = "download ";
    downloadFileReq+=fileName;
    downloadFileReq+=" ";
    downloadFileReq+=to_string(chunkNo);
    struct Client client3 = clientConstructor(AF_INET,  SOCK_STREAM, 0, peerPort, INADDR_ANY);
    if(client3.socket == -1){
        std::cout<<"!! ERROR - SOCKET CREATION FAILED HERE@!!\n";
        return;
    }

    string status = "incomplete";
    int count = 0;
    while(status == "incomplete"){
        char* res3 = client3.request(&client3, peerIp, peerPort, downloadFileReq, currChunkSize, 1);
        string response3(res3);
        
        if(response3 == "<CHUNK DOWNLOAD FAILED>"){
            std::cout<<"<FAILED TO DOWNLOAD> CHUNK - "<<chunkNo<<"\n";
            return;
        }

        string currChunkSha1 = getShaOfString(response3);
        string shaReceived = dynamicDownloadsShaMap[fileName].substr(chunkNo*40, 40);

        if(currChunkSha1 == shaReceived){
            pwrite(fd, res3, currChunkSize, chunkNo*1024*512);
            status = "completed";
        }else count++;

        if(count >= 7) {
            status = "interrupted";
        }

        memset(res3, 0, currChunkSize);
        
    }
    close(client3.socket);

    if(status != "completed") std::cout<<"<DOWNLOADED CHUNK NO: "<<chunkNo<<" INCOMPLETE>\n";
    fileToBitMap[fileName].first[chunkNo] = true;
}

void client(string req, string ip, int port) {

    vector<string> command = divideStringByChar(req, ' ');

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
        close(client.socket);
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
            close(client.socket);
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
            close(client.socket);
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
        close(client.socket);
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
        close(client.socket);
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
        close(client.socket);
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
        close(client.socket);

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
        close(client.socket);

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
        close(client.socket);
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
        string fileName = getFileName(buffer);
        string sha1 = getChunkWiseSha(buffer);
        int noOfChunks = (int)(sha1.size())/40;

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
        if(response == "<SHA1 ALREADY PRESENT>" || response == "<WAITING FOR SHA1 UPLOADS>"){
            std::cout<<"**********[<UPLOADED>]**********\n";
        }

        if(response == "<WAITING FOR SHA1 UPLOADS>"){
            for(int i=0; i<noOfChunks; i++){
                string chunkSha = sha1.substr(i*40, 40);
                string req1 = "uploadChunkSha ";
                req1+=fileName;
                req1+=" ";
                req1+=to_string(i);
                req1+=" ";
                req1+=to_string(noOfChunks);
                req1+=" ";
                req1+=chunkSha;
                struct Client client1 = clientConstructor(AF_INET,  SOCK_STREAM, 0, port, INADDR_ANY);
                if(client1.socket == -1){
                    std::cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
                    return;
                }
                char* res1 = client1.request(&client1, tracker.ip, tracker.port, req1, 20000, 0);
                string response1(res1);
                response = response1;
                close(client1.socket);
            }
        }

        if(response == "<SHA1 UPLOAD COMPLETED>" || response == "<SHA1 ALREADY PRESENT>"){
            cout<<response<<"\n";
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
            
            int lastChunkSize=512*1024;
            if(bytesLeft > 0){
                lastChunkSize = bytesLeft;
                noOfChunks++;
            }
            vector<bool> bitMap(noOfChunks, true);
            fileToBitMap[fileName] = make_pair(bitMap, lastChunkSize);

            fileLocMap[fileName] = buffer;
        }

        close(client.socket);
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
        close(client.socket);
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

            vector<string> usersInfo = divideStringByChar(response, ' ');
            unordered_map<string, pair<string, int>> userDetails;
            for(string userInfo : usersInfo){
                vector<string> tokens = divideStringByChar(userInfo, '-');
                userDetails[tokens[0]] = make_pair(tokens[1], atoi(tokens[2].c_str()));
            }
            
            unordered_map<string, string> userToBitMap;
            string fileName = command[2];
            string destinationPath = command[3];
            vector<thread> bitMapThreads;

            //threads to get all the bitmaps from seeders
            sem_t mutex;
            sem_init(&mutex, 0, 1);
            for(auto us : userDetails){
                string peerId = us.first;
                string peerIp = us.second.first;
                int peerPort = us.second.second;
                bitMapThreads.push_back(thread(getUsersBitMapThread, peerIp, peerPort, fileName, peerId, ref(userToBitMap), ref(mutex)));
            }

            for (thread &t : bitMapThreads) {
                t.join();
            }
            sem_destroy(&mutex);

            bitMapThreads.clear();
            std::cout<<"<ALL BITMAPS RECEIVED>\n";

            unordered_map<string, pair<string, int>> userToStringBitMapAndLchunkSize;
            string bitMap="";
            int lastChunkVal=0;
            for(auto ub : userToBitMap){
                vector<string> vals = divideStringByChar(ub.second, ' ');
                userToStringBitMapAndLchunkSize[ub.first] = make_pair(vals[0], atoi(vals[1].c_str()));
                if(bitMap=="") bitMap = vals[0];
                if(lastChunkVal==0) lastChunkVal=atoi(vals[1].c_str());
            }

            //get whole sha1 of file
            int noOfChunks = bitMap.size();
            string getShaReq = "getSha1 ";
            getShaReq+=fileName;
            int noOfBytes = noOfChunks*40;

            struct Client clientsh = clientConstructor(AF_INET,  SOCK_STREAM, 0, port, INADDR_ANY);
            if(clientsh.socket == -1){
                std::cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
                return;
            }
            char* entireSha1 = clientsh.request(&clientsh, tracker.ip, tracker.port, getShaReq, noOfBytes, 1);
            string fullsha1(entireSha1);
            std::memset(entireSha1, 0, noOfBytes);
            dynamicDownloadsShaMap[fileName] = fullsha1;
            close(clientsh.socket);

            cout<<dynamicDownloadsShaMap[fileName].size()<<"\n";

            //chunkNo, noOfSeeders
            
            vector<pair<int, int>> chunkNoToNoOfSeeders(noOfChunks, make_pair(0, 0));
            unordered_map<int, vector<string>> chunkNoToSeeders;
            int firstChunkNo = -1;
            for(int i=0; i<noOfChunks; i++){
                chunkNoToNoOfSeeders[i].first = i;
                for(auto u : userToStringBitMapAndLchunkSize){
                    chunkNoToSeeders[i].push_back(u.first);
                    string bm = u.second.first;
                    if(bm[i]=='1'){
                        chunkNoToNoOfSeeders[i].second++;
                        if(firstChunkNo == -1) firstChunkNo = i;
                        int udn = userDetails.size();
                        if(chunkNoToNoOfSeeders[i].second == udn) firstChunkNo = i;
                    }
                }
            }
            int m = chunkNoToNoOfSeeders.size();
            pair<int, int> lastEle = chunkNoToNoOfSeeders[m-1];
            pair<int, int> randomFirst = chunkNoToNoOfSeeders[firstChunkNo];
            chunkNoToNoOfSeeders[firstChunkNo] = lastEle;
            chunkNoToNoOfSeeders[m-1] = randomFirst;
            chunkNoToNoOfSeeders.pop_back();

            std::sort(chunkNoToNoOfSeeders.begin(), chunkNoToNoOfSeeders.end(), [](pair<int, int>& a, pair<int, int>& b){
                return a.second > b.second;
            });
            //prioritizing random chunk first
            chunkNoToNoOfSeeders.push_back(randomFirst);

            destinationPath+="/";
            destinationPath+=fileName;
            int fd = open(destinationPath.c_str(), O_CREAT|O_RDWR, 0666);

            string fileGrpKey = "";
            fileGrpKey+=fileName;
            fileGrpKey+="*";
            fileGrpKey+=command[1];

            //random chunk first and then rarest chunk first
            vector<thread> downloadChunkThreads;
            for(int i=m-1; i>=0; i--){
                //setup file details for the first chunk download
                if(downloadedFiles.find(fileGrpKey) == downloadedFiles.end()) {
                    downloadedFiles[fileGrpKey].first = command[1];
                    downloadedFiles[fileGrpKey].second = 'D';
                    char buf[PATH_MAX]; 
                    realpath(destinationPath.c_str(), buf);
                    fileLocMap[fileName] = string(buf);
                    vector<bool> bmap(noOfChunks, false);
                    fileToBitMap[fileName] = make_pair(bmap, lastChunkVal);
                }

                if(downloadedFiles[fileGrpKey].second == 'C') break;

                pair<int, int> p = chunkNoToNoOfSeeders[i];
                int chunkNo = p.first;
                
                vector<string> seeders = chunkNoToSeeders[chunkNo];
                int max = seeders.size()-1;
                int min = 0;
                int randNum = rand()%(max-min + 1) + min;
                string uid = seeders[randNum];
                string seederIp = userDetails[uid].first;
                int seederPort = userDetails[uid].second;
                int chunkSize = 512*1024;

                if(chunkNo == noOfChunks-1) chunkSize = lastChunkVal;

                downloadChunkThreads.emplace_back(downloadChunkFromPeer, fileName, seederIp, seederPort, chunkNo, chunkSize, fd);
                // downloadChunkFromPeer(fileName, seederIp, seederPort, chunkNo, chunkSize, fd);
                if(downloadChunkThreads.size() == 10){
                    for(int i=0; i<10; i++) downloadChunkThreads[i].join();
                    downloadChunkThreads.clear();
                }

                if(i==m-1){
                    //download_file <group_id> <file_name> <destination_path>
                    string req1 = "addSeeder ";
                    req1+=command[1];
                    req1+= " ";
                    req1+=command[2];
                    req1+=" ";
                    req1+=userid;
                    struct Client client1 = clientConstructor(AF_INET,  SOCK_STREAM, 0, port, INADDR_ANY);
                    if(client1.socket == -1){
                        std::cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
                        return;
                    }

                    char* res1 = client1.request(&client1, tracker.ip, tracker.port, req1, 20000, 0);
                    string response1(res1);
                    std::memset(res1, 0, 20000);
                    close(client1.socket);
                }

                // usleep(1000000);

            }

            for (thread &t : downloadChunkThreads) {
                t.join();
            }
            close(fd);
            downloadChunkThreads.clear();

            std::cout<<"FILE COMPLETELY DOWNLOADED SUCCESSFULLY\n";
            downloadedFiles[fileGrpKey].second = 'C';
            dynamicDownloadsShaMap.erase(dynamicDownloadsShaMap.find(fileName));

        }
        close(client.socket);
    }

    //show_downloads
    else if(command[0] == "show_downloads"){
        //[D] [grp_id] filename
        if(downloadedFiles.size() == 0){
            cout<<"<NO DOWNLOADS YET>\n";
        }else{
            for(auto m : downloadedFiles){
                std::cout<<"["<<m.second.second<<"]["<<m.second.first<<"] "<<divideStringByChar(m.first, '*')[0]<<"\n";
            }
        }
    }

    // stop_share <group_id> <file_name>
    else if(command[0] == "stop_share"){
        string req1 = req+" ";
        req1+=userid;
        struct Client client = clientConstructor(AF_INET,  SOCK_STREAM, 0, port, INADDR_ANY);
        if(client.socket == -1){
            std::cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
            return;
        }
        char* res = client.request(&client, tracker.ip, tracker.port, req1, 20000, 0);
        string response(res);
        std::cout<<"**********["<<response<<"]**********\n";
        close(client.socket);
    }

    else{
        struct Client client = clientConstructor(AF_INET,  SOCK_STREAM, 0, port, INADDR_ANY);
        if(client.socket == -1){
            std::cout<<"!! ERROR - SOCKET CREATION FAILED !!\n";
            return;
        }
        client.request(&client, ip, client.port, req, 20000, 0);
        close(client.socket);
    }

}


/* main function */
int main(int n, char* argv[]){

    string ip, trackerInfoDest;
    int port;
    getArgDetails(ip, port, trackerInfoDest, argv);
    vector<Tracker> trackers = getTrackerDetails(trackerInfoDest);
    tracker = trackers[0];

    //127.0.0.3:2022
    std::cout<<"[Tracker Used]:    TrackerId=> "<<tracker.id<<" | IP=> "<<tracker.ip<<" | PORT=> "<<tracker.port<<"\n";
    std::cout<<"[Tracker 2]:    TrackerId=> "<<trackers[1].id<<" | IP=> "<<trackers[1].ip<<" | PORT=> "<<trackers[1].port<<"\n";
    std::cout<<"[Peer Client]:    IP=> "<<ip<<" | PORT=> "<<port<<"\n";

    struct sockaddr_in servaddr;
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(tracker.ip.c_str());
    servaddr.sin_port = htons(tracker.port);

    thread serverThread(server, port);
    vector<thread> clientThreads;
    while(true){
        int sockfd;
        string req;
        getline(cin, req);
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
            printf("socket creation for tracker live check failed...\n");
            exit(0);
        }
        if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
            cout<<"SWITCHING TO SECOND TRACKER\n";
            tracker = trackers[1];
            servaddr.sin_addr.s_addr = inet_addr(tracker.ip.c_str());
            servaddr.sin_port = htons(tracker.port);
        }
        close(sockfd);

        if(req!="" && req!="\n" && req!=" ") clientThreads.emplace_back(client,req, ip, port);
    }
    for (thread &t : clientThreads) {
         t.join();
    }
    serverThread.join();


    return 0;
}

