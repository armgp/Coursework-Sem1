// 1. Use of system() library function for command mode functions is not allowed.
// 2. Use of system commands like ls, cp, mv, mkdir etc are not allowed. You have to 
//    implement your own versions of these commands.
// 3. Use of ncurses.h is strictly prohibited. 

/** includes **/
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

/** define **/
#define CTRL_KEY(k) ((k) & 0x1f)
#define LOWTOUP(k) ((k) & 0xdf)

/** data **/
struct explorerConfig{
    int cx, cy;
    int explorerRows;
    int explorerColumns;
    struct termios origTermios;
}exCfg;

struct currDirectoryDetails{
    std::string currDirectory="current dir";
    std::vector<int> minLengthsPerColumn;
}currDirDet;

std::vector<std::vector<std::string>> directories;

/** terminal **/
void die(const char* s){
    //screen cleared and cursor repositioned
    //we can also see the error after clearing of the screen
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
    write(STDOUT_FILENO, "\x1b[48;2;0;0;0ml", 13);

    perror(s);
    exit(1);
}

void disableRawMode(){
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &exCfg.origTermios)==-1) die("tcsetattr");
}

void enableRawMode(){
    atexit(disableRawMode);
    if(tcgetattr(STDIN_FILENO, &exCfg.origTermios)==-1) die("tcsetattr");

    struct termios raw = exCfg.origTermios;

    //--flip off ECHO bit to disable printing the key we type on terminal
    //--flip off canonical mode(ICANON) when this bit is made 0, input is taken
    //byte by byte instead of line by line.
    //--(IXTEN flip off) On some systems, when you type Ctrl-V, the terminal waits for you 
    //to type another character and then sends that character literally. 
    //--turning off ISIG will turn off sending SIGINT(Ctrl+C - terminate) and SIGTSTP(Ctrl+Z - suspend)
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    //--Disable Ctrl+S -> stops data from being transmitted to the terminal until you press Ctrl+Q
    //XON comes from the names of the two control characters that Ctrl-S and Ctrl-Q produce:
    // XOFF to pause transmission and XON to resume transmission.
    //--Ctrl-M is weird: it’s being read as 10, when we expect it to be read as 13,
    // since it is the 13th letter of the alphabet, and Ctrl-J already produces a 10.
    //What else produces 10? The Enter key does.
    //It turns out that the terminal is helpfully translating any carriage returns (13, '\r') 
    //inputted by the user into newlines (10, '\n'). Let’s turn off this feature.
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    //We will turn off all output processing features by turning off the OPOST flag. 
    //In practice, the "\n" to "\r\n" translation is likely the only output processing
    //feature turned on by default.
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);

    raw.c_cc[VMIN]=0;
    raw.c_cc[VTIME]=1;

    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw)==-1) die("tcsetattr");
}

char readKey(){
    int n;
    char c;
    //read one byte from the standard input to the variable c
    while((n=read(STDIN_FILENO, &c, 1)) != 1){
        if(n==-1 && errno != EAGAIN) die("read"); 
    }
    return c;
}

//reposition cursor to x and y
void repositionCursor(int x, int y){
    if(x>=exCfg.explorerColumns){
        x=exCfg.explorerColumns;
    }
    if(y>=exCfg.explorerRows){
        y=exCfg.explorerRows;
    }
    std::string X = std::to_string(x+1);
    std::string Y = std::to_string(y+1);
    std::cout<<"\x1b["<<Y<<";"<<X<<"H";
}

//move cursor by u,d,f b distances;
void moveCursor(int u, int d, int f, int b){
    std::string U = std::to_string(u);
    std::string D = std::to_string(d);
    std::string F = std::to_string(f);
    std::string B = std::to_string(b);

    if(u!=0) std::cout<<"\x1b["+U+"A";
    if(d!=0) std::cout<<"\x1b["+D+"B";
    if(f!=0) std::cout<<"\x1b["+F+"C";
    if(b!=0) std::cout<<"\x1b["+B+"D";
}

//getCursorPosition(y, x);
//fallback method if ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) doesnt work in some systems
//to get the width and height of the explorer
int getCursorPosition(int& rows, int& cols){
    const int BUFFER_SIZE =32;
    char buffer[BUFFER_SIZE];
    unsigned int i = 0 ;

    if(write(STDOUT_FILENO, "\x1b[6n", 4) != 4) return -1;

    for(int i=0; i<BUFFER_SIZE; i++){
        if(read(STDIN_FILENO, &buffer[i], 1) != 1) break;
        if(buffer[i]=='R'){
            buffer[i]='\0';
            break;
        } 
    }

    if(buffer[0] != '\x1b' || buffer[1] != '[') return -1;
    if (sscanf(&buffer[1], "[%d;%d", &rows, &cols) != 2) return -1;

    return 0;
}

int getWindowSize(int& rows, int& cols){
    struct winsize ws;
    if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws)==-1 || ws.ws_col==0){
        if(write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) return -1;
        return getCursorPosition(rows, cols);
    }
    else{
        rows=ws.ws_row;
        cols=ws.ws_col;
    }
    return 0;
}

/** input **/
void processKeyPress(){
    char c = readKey();
    //IF CTRL+q exit
    //here since the 5th bit is also discarded 
    //it doesnt matter if its lowercase/uppercase q.
    //5th bit set to 1 is lowercase
    c=LOWTOUP(c);
    if(c=='Q'){
        //to change the colors back to default 
        std::cout<<"\x1b[0m";
        //to clear screen when exiting
        std::cout<<"\x1b[2J";
        exit(0);
    } 
}

/** output **/
void drawExplorerRows(){
    int x, y;
    int eCols=exCfg.explorerColumns;
    int dCols=6;
    int maxOccupancy = eCols/dCols;
    for(std::vector<std::string> dir : directories){
        getCursorPosition(y, x);
        repositionCursor(0, y);
        std::cout<<"\x1b[K";
        if(dir[0].size()<maxOccupancy) std::cout<<dir[0];
        else std::cout<<dir[0].substr(0, maxOccupancy-3)<<"..";

        repositionCursor(maxOccupancy, y);
        if(dir[1].size()<maxOccupancy) std::cout<<dir[1];
        else std::cout<<dir[1].substr(0, maxOccupancy-3)<<"..";

        repositionCursor(2*(maxOccupancy), y);
        if(dir[2].size()<maxOccupancy) std::cout<<dir[2];
        else std::cout<<dir[2].substr(0, maxOccupancy-3)<<"..";

        repositionCursor(3*(maxOccupancy), y);
        if(dir[3].size()<maxOccupancy) std::cout<<dir[3];
        else std::cout<<dir[3].substr(0, maxOccupancy-3)<<"..";

        repositionCursor(4*(maxOccupancy), y);
        if(dir[4].size()<maxOccupancy) std::cout<<dir[4];
        else std::cout<<dir[4].substr(0, maxOccupancy-3);

        // repositionCursor(4*(maxOccupancy), y);
        // if(dir[5].size()<maxOccupancy) std::cout<<dir[5];
        // else std::cout<<dir[5].substr(0, maxOccupancy-3);

        std::cout<<"\n\r";
    }
    std::cout<<"\x1b[H \r\n";
}
void drawMode(){
    int x, y;
    getWindowSize(y, x);
    repositionCursor(1, y);
    std::cout<<"\x1b[KNormal Mode: "<<currDirDet.currDirectory;
    std::cout<<"\x1b[H \n\r";
}
void refreshExplorerScreen(){ 
    std::cout<<"\x1b[?25l\x1b[H\r\n";  
    drawMode();
    drawExplorerRows();
    std::cout<<"\x1b[?25h\x1b[H\r\n";  
}

/** init **/
void initExplorer(){
    exCfg.cx=0;
    exCfg.cy=0;
    std::cout<<"\x1b[J\n\r";
    if(getWindowSize(exCfg.explorerRows, exCfg.explorerColumns)==-1) die("getWindowSize");
}

/**util**/
std::string formatBytes(double val){
    double kb = 1024;
    double mb = 1024*1024;
    double gb = 1024*1024*1024;
    double formattedVal;
    
    std::string res;
    if(val<kb){
        return std::to_string(val);
    } 
    else if(val>kb && val<mb){
        formattedVal = val/kb;
        res+=" KB";
    }else if(val>mb && val<gb){
        formattedVal = val/mb;
        res+=" MB";
    }else{
        formattedVal = val/gb;
        res+=" GB";
    }
    
    res = std::to_string(formattedVal)+res;
    return res;
}

std::string getPermissions(mode_t perm){
   
    std::string modVal="---------";
    
    modVal[0] = (perm & S_IRUSR) ? 'r' : '-';
    modVal[1] = (perm & S_IWUSR) ? 'w' : '-';
    modVal[2] = (perm & S_IXUSR) ? 'x' : '-';
    modVal[3] = (perm & S_IRGRP) ? 'r' : '-';
    modVal[4] = (perm & S_IWGRP) ? 'w' : '-';
    modVal[5] = (perm & S_IXGRP) ? 'x' : '-';
    modVal[6] = (perm & S_IROTH) ? 'r' : '-';
    modVal[7] = (perm & S_IWOTH) ? 'w' : '-';
    modVal[8] = (perm & S_IXOTH) ? 'x' : '-';
    return modVal;     
}

int main(){

    DIR* dir = opendir(".");
    if(dir==NULL) return 1;
    struct dirent* entity;
    struct stat statBuff;
    entity = readdir(dir);
    
    while (entity!=NULL){
        std::vector<std::string> dirDetails;
        stat(entity->d_name, &statBuff);
        
        dirDetails.push_back(entity->d_name);

        double sz = statBuff.st_size;
        std::string fileSize = formatBytes(sz);
        dirDetails.push_back(fileSize);
        dirDetails.push_back(getgrgid(statBuff.st_uid)->gr_name);
        dirDetails.push_back(getpwuid(statBuff.st_gid)->pw_name);
        
        
        char time_buf[80];
        // std::string timeBuff;
        struct tm ts;
        ts = *localtime(&statBuff.st_mtim.tv_sec);
        strftime(time_buf, sizeof(time_buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
        std::string lastMod(asctime(&ts));
        lastMod=lastMod.substr(4,12);
        dirDetails.push_back(lastMod);

        std::string permData = getPermissions(statBuff.st_mode);
        dirDetails.push_back(permData);

        directories.push_back(dirDetails);
        entity = readdir(dir);
    }
    std::sort(directories.begin(), directories.end());

    enableRawMode();
    initExplorer();

    while (1){
        refreshExplorerScreen();
        processKeyPress();
    }

    return 0;
}
