#include <bits/stdc++.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <bits/stdc++.h>
#include <dirent.h>
#include <sys/stat.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <fcntl.h>
#include <fstream>
#include <iostream>

std::string getSourceName(std::string sourcefile){
    int n = sourcefile.size();
    int i=n-1;
    for(; i>=0; i--){
        if(sourcefile[i]=='/') break;
    }
    return sourcefile.substr(i+1);
}

std::string getSourceDirectory(std::string sourcefile){
    int n = sourcefile.size();
    int i=n-1;
    for(; i>=0; i--){
        if(sourcefile[i]=='/') break;
    }
    return sourcefile.substr(0,i+1);
}

int createfile(std::string name, mode_t perms){
    int fd = open(name.c_str(), O_CREAT|O_EXCL, perms);
    if (fd >= 0) {
        std::cout<<"done"<<"\n";
        return fd;
    } else {
        std::cout<<"error perms"<<"\n";
    }
    return -1;
}

int createfile(std::string name){
    mode_t perms = S_IRWXU; 
    int fd = open(name.c_str(), O_CREAT|O_EXCL, perms);
    if (fd >= 0) {
        std::cout<<"done"<<"\n";
        return fd;
    } else {
        std::cout<<"error now"<<"\n";
    }
    return -1;
}

void createDirectory(std::string name){
    const int d = mkdir(name.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (d==-1){
        printf("Error creating directory!n");
        exit(1);
    }
}


//std::string sourceFileName, std::string sourceFile, std::string destinationDirectory
void copyfile(std::string sourceFile, std::string destinationDirectory){
    std::string line;
    struct stat st;
    std::ifstream ifile{
        // "/home/user/Documents/Coursework/AOS/Assg/AOS_Assignment_1.pdf"
        sourceFile
    }; 
    std::ofstream ofile{ 
        // "/home/user/Documents/Coursework/AOS/AOS_Assignment_1.pdf"
        destinationDirectory 
    };
    if (ifile && ofile) {
  
        while (getline(ifile, line)) {
            ofile << line << "\n";
        }
        std::cout << "Copy Finished \n";
    }
    else {
        printf("Cannot read File \n");
    }

    stat(sourceFile.c_str(), &st);    
    mode_t perm = st.st_mode;
    chmod(destinationDirectory.c_str(), perm); 
    ifile.close();
    ofile.close();
}

void renameFile(std::string sourceFile, std::string newName){
    copyfile(sourceFile, getSourceDirectory(sourceFile)+newName);
    remove(sourceFile.c_str());
}


void copyfiles(std::vector<std::string> sourcefiles, std::string destinationDirectory){
    for(std::string sourcefile : sourcefiles){
        std::string sourceName=getSourceName(sourcefile);
        std::cout<<sourcefile<<" -to- "<<destinationDirectory+"/"+sourceName<<"\n";
        copyfile(sourcefile, destinationDirectory+"/"+sourceName);
    }
}

void movefiles(std::vector<std::string> sourcefiles, std::string destinationDirectory){
    for(std::string sourcefile : sourcefiles){
        std::string sourceName=getSourceName(sourcefile);
        std::cout<<sourcefile<<" -to- "<<destinationDirectory+"/"+sourceName<<"\n";
        copyfile(sourcefile, destinationDirectory+"/"+sourceName);
        remove(sourcefile.c_str());
    }
}

int main(){
    // createfile("gpfile");
    // createfile("gpfile", S_IRWXU);
    // createDirectory("/home/user/Documents/Coursework/AOS/Assg/newDir");
    
    // std::vector<std::string> sourcefiles={"/home/user/Documents/Coursework/AOS/Assg/fromFolder/AOS_Assignment_1.pdf", "/home/user/Documents/Coursework/AOS/Assg/fromFolder/test1", "/home/user/Documents/Coursework/AOS/Assg/fromFolder/test2"};
    // std::string destinationDirectory="/home/user/Documents/Coursework/AOS/Assg/toFolder";
    // movefiles(sourcefiles, destinationDirectory);

    // std::vector<std::string> sourcefiles={"/home/user/Documents/Coursework/AOS/Assg/toFolder/AOS_Assignment_1.pdf", "/home/user/Documents/Coursework/AOS/Assg/toFolder/test1", "/home/user/Documents/Coursework/AOS/Assg/toFolder/test2"};
    // std::string destinationDirectory="/home/user/Documents/Coursework/AOS/Assg/fromFolder";
    // copyfiles(sourcefiles, destinationDirectory);

    // renameFile("/home/user/Documents/Coursework/AOS/Assg/fromFolder/test1", "ironman");

    copyfile("/home/user/Documents/Coursework/AOS/Assg/fromFolder/newmain.cpp", "/home/user/Documents/Coursework/AOS/Assg/toFolder/newmain.cpp");

    return 0;
}

