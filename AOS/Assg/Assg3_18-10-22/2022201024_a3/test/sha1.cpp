#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include <iostream>
#include <string>
#include <unistd.h> 
#include <stdlib.h>
#include <fcntl.h>

using namespace std;

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

int main() {

    char buffer[524288];
    int readByte;
    string res = "";
    string path;
    cin>>path;

    int fd = open(path.c_str(), O_RDONLY);
    if (fd < 0)
        return -1;
    else{
        while ((readByte = read(fd, buffer, 524288)) > 0){
            string buf(buffer);
            res+=getShaOfString(buf);
            memset(buffer, 0, 524288);
        }
    }

    cout<<res<<"\n";
    return 0;
}
