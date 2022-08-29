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
#include <sys/types.h>
#include <sys/wait.h>

/**functions**/
void refreshExplorerScreen();
void initWindowSize();

/** define **/
#define CTRL_KEY(k) ((k) & 0x1f)
#define LOWTOUP(k) ((k) & 0xdf)

enum editorKey {
  HOME,
  DELETE,
  BACKSPACE=127
};

/** data **/
struct explorerConfig{
    int cx, cy;
    int explorerRows;
    int explorerColumns;
    struct termios origTermios;
}exCfg;

struct currDirectoryDetails{
    std::string currDirectory=getpwuid(getuid())->pw_name;
    std::stack<std::string> dirHistory;
}currDirDet;

struct onScreenDirectories{
    std::vector<std::vector<std::string>> directories;
    int currPos=0;
    int startInd=0;
    std::string d=".";
}osd;


/** append buffer **/
std::string appendBuffer="";

/** terminal **/
void handleWindowSizeChange(){
    int prevCx=exCfg.explorerColumns;
    int prevCy=exCfg.explorerRows;
    initWindowSize();
    int currCx=exCfg.explorerColumns;
    int currCy=exCfg.explorerRows;
    if(currCx!=prevCx || currCy!=prevCy) refreshExplorerScreen();
}
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
        handleWindowSizeChange();
        if(n==-1 && errno != EAGAIN) die("read"); 
    }

    if (c == '\x1b') {
        char seq[3];
        if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
        if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';
        if (seq[0] == '[') {
            if (seq[1] >= '0' && seq[1] <= '9') {
                if (read(STDIN_FILENO, &seq[2], 1) != 1) return '\x1b';
                if (seq[2] == '~') {
                    switch (seq[1]) {
                        case '1': return HOME;
                        case '3': return DELETE;
                        case '7': return HOME;
                    }
                }
            }
            else{
                switch (seq[1]) {
                    case 'A': return 'w';
                    case 'B': return 's';
                    case 'C': return 'd';
                    case 'D': return 'a';
                    case 'H': return HOME;
                }
            }
        }else if(seq[0]=='O'){
            switch (seq[1]) {
                case 'H': return HOME;
            }
        }



        return '\x1b';
    }

    return c;
}

//reposition cursor to x and y
void repositionCursor(int x, int y, std::string& currBuffer){
    if(x>=exCfg.explorerColumns){
        x=exCfg.explorerColumns;
    }
    if(y>=exCfg.explorerRows){
        y=exCfg.explorerRows;
    }
    std::string X = std::to_string(x+1);
    std::string Y = std::to_string(y+1);
    currBuffer+="\x1b["+Y+";"+X+"H";
}

//move cursor by u,d,f b distances;
void moveCursor(int u, int d, int f, int b, std::string& currBuffer){
    std::string U = std::to_string(u);
    std::string D = std::to_string(d);
    std::string F = std::to_string(f);
    std::string B = std::to_string(b);

    if(u!=0) currBuffer+="\x1b["+U+"A";
    if(d!=0) currBuffer+="\x1b["+D+"B";
    if(f!=0) currBuffer+="\x1b["+F+"C";
    if(b!=0) currBuffer+="\x1b["+B+"D";
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

/** init **/
void initExplorer(){
    exCfg.cx=0;
    exCfg.cy=0;
    if(getWindowSize(exCfg.explorerRows, exCfg.explorerColumns)==-1) die("getWindowSize");
}

void initWindowSize(){
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

int populateCurrDirectory(){
    osd.directories.clear();
    DIR* dir = opendir(osd.d.c_str());
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
        // std::cout<<lastMod<<"\n";
        // processKeyPress();
        dirDetails.push_back(lastMod);
        std::string permData = getPermissions(statBuff.st_mode);
        dirDetails.push_back(permData);

        if(S_ISDIR(statBuff.st_mode)) dirDetails.push_back("1");
        else dirDetails.push_back("0");

        osd.directories.push_back(dirDetails);
        
        entity = readdir(dir);
    }
    std::sort(osd.directories.begin(), osd.directories.end());
    return 0;
}

/** input **/
void processKeyPress(){
    char c = readKey();

    //IF CTRL+q exit
    //here since the 5th bit is also discarded 
    //it doesnt matter if its lowercase/uppercase q.
    //5th bit set to 1 is lowercase
    if(c=='w' && exCfg.cy>=1){
        if(exCfg.cy==1 && osd.startInd>0){
            if(osd.currPos>0){
                osd.startInd--;
                osd.currPos--;
            }
        }
        else {
            exCfg.cy--;
            osd.currPos--;
        }
    }

    else if(c=='s' && exCfg.cy<osd.directories.size()-1 && exCfg.cy<=exCfg.explorerRows-4){
        if(exCfg.cy==exCfg.explorerRows-4){
            if(osd.currPos < osd.directories.size()-1){
                osd.startInd++;
                osd.currPos++;
            }
        }
        else {
            exCfg.cy++;
            osd.currPos++;
        }
    }

    else if(c=='d'){
        //to change the colors back to default 
        write(STDOUT_FILENO, "\x1b[0m", 4);
        //to clear screen when exiting
        write(STDOUT_FILENO, "\x1b[2J", 4);
        exit(0);   
    }

    else if(c=='a'){
        //to change the colors back to default 
        write(STDOUT_FILENO, "\x1b[0m", 4);
        //to clear screen when exiting
        write(STDOUT_FILENO, "\x1b[2J", 4);
        exit(0);
    }

    else if(c==DELETE){
        //to change the colors back to default 
        write(STDOUT_FILENO, "\x1b[0m", 4);
        //to clear screen when exiting
        write(STDOUT_FILENO, "\x1b[2J", 4);
        exit(0);
    }

    else if(c==HOME || c== 'h' || c== 'H'){
        //to change the colors back to default 
        write(STDOUT_FILENO, "\x1b[0m", 4);
        //to clear screen when exiting
        write(STDOUT_FILENO, "\x1b[2J", 4);
        exit(0);
    }

    else if(c==BACKSPACE){
        //to change the colors back to default 
        write(STDOUT_FILENO, "\x1b[0m", 4);
        //to clear screen when exiting
        write(STDOUT_FILENO, "\x1b[2J", 4);
        exit(0);
    }

    else if(c=='\r'){
        if(osd.directories[osd.currPos][6]=="1"){
            osd.d+="/"+osd.directories[osd.currPos][0];
            populateCurrDirectory();
        }else{
            std::string temp=osd.d;
            osd.d+="/"+osd.directories[osd.currPos][0];
            //only child process will have fork()=0
            int pid=fork();
            if(pid==0){
                execl("/usr/bin/vi", "vi", osd.d.c_str(), (char *)0);
                exit(1);
            }else{
                int wval, status=0;
                while ((wval=waitpid(pid, &status, 0)) != pid){
                    if (wval == -1) exit(1); 
                }
            }
            osd.d=temp;
        }
        
    }

    c=LOWTOUP(c);
    if(c=='Q'){
        //to change the colors back to default 
        write(STDOUT_FILENO, "\x1b[0m", 4);
        //to clear screen when exiting
        write(STDOUT_FILENO, "\x1b[2J", 4);
        exit(0);
    } 
}

/** output **/
void drawExplorerRows(){
    int n = exCfg.explorerRows;
    int m = osd.directories.size();
    int eCols=exCfg.explorerColumns;
    int dCols=6;
    int maxOccupancy = eCols/dCols;
    std::string draw;
    for(int i=0; i<=n; i++){
        if(i==n-1){
            draw+="\x1b[K\x1b[H";
            break;
        }else if(i==0){
            draw+="\x1b[K\x1b[1G";
            moveCursor(0,0,5,0,draw);
            draw+="FILE_NAME";

            draw+="\x1b[1G";
            moveCursor(0,0,maxOccupancy+5,0,draw);
            draw+="FILE_SIZE";

            draw+="\x1b[1G";
            moveCursor(0,0,2*maxOccupancy+5,0,draw);
            draw+="USER";

            draw+="\x1b[1G";
            moveCursor(0,0,3*maxOccupancy+5,0,draw);
            draw+="GROUP";

            draw+="\x1b[1G";
            moveCursor(0,0,4*maxOccupancy+5,0,draw);
            draw+="LAST_MOD";

            draw+="\x1b[1G";
            moveCursor(0,0,5*maxOccupancy+5,0,draw);
            draw+="PERMISSIONS";
        }else if(i==n-2) {
            draw+="\x1b[K Normal Mode: ";
            draw+=currDirDet.currDirectory;
        }else draw+="\x1b[K";

        
        if(i>0 && (i-1+osd.startInd)<m && i<n-3){
            std::vector<std::string> dirDetails = osd.directories[i-1+osd.startInd];
            
            moveCursor(0,0,5,0,draw);
            if(dirDetails[0].size()<maxOccupancy) draw+=dirDetails[0];
            else draw+=dirDetails[0].substr(0, maxOccupancy-3)+"..";

            draw+="\x1b[1G";
            moveCursor(0,0,maxOccupancy+5,0,draw);
            if(dirDetails[1].size()<maxOccupancy) draw+=dirDetails[1];
            else draw+=dirDetails[1].substr(0, maxOccupancy-3)+"..";

            draw+="\x1b[1G";
            moveCursor(0,0,2*maxOccupancy+5,0,draw);
            if(dirDetails[2].size()<maxOccupancy) draw+=dirDetails[2];
            else draw+=dirDetails[2].substr(0, maxOccupancy-3)+"..";

            draw+="\x1b[1G";
            moveCursor(0,0,3*maxOccupancy+5,0,draw);
            if(dirDetails[3].size()<maxOccupancy) draw+=dirDetails[3];
            else draw+=dirDetails[3].substr(0, maxOccupancy-3)+"..";

            draw+="\x1b[1G";
            moveCursor(0,0,4*maxOccupancy+5,0,draw);
            if(dirDetails[4].size()<maxOccupancy) draw+=dirDetails[4];
            else draw+=dirDetails[4].substr(0, maxOccupancy-3)+"..";

            draw+="\x1b[1G";
            moveCursor(0,0,5*maxOccupancy+5,0,draw);
            if(dirDetails[5].size()<maxOccupancy) draw+=dirDetails[5];
            else draw+=dirDetails[5].substr(0, maxOccupancy-3)+"..";
        }

        draw+="\r\n";
    }
    appendBuffer+=draw;

}

void refreshExplorerScreen(){
    //hide cursor while drawing
    appendBuffer+="\x1b[?25l";

    /* DELETED-(We dont want to clear the entire screen each time we refresh)
    // \x1b is the escape charachter(byte)=27 which is always in front of escape sequences
    // escape seq always starts with esc char and [
    // after [ comes the argument which is 2(clear the entire screen) and command which is J(Erase In Display)
    //4bytes
    appendBuffer+="\x1b[2J";
    */

    //reposition cursor. The default is \x1b[1;1H == \x1b[H, so only need to write 3 bytes
    //row and column starts from 1 and not 0.
    appendBuffer+="\x1b[H\x1b[1m";
    // write(STDOUT_FILENO, "\x1b[H", 3);

    drawExplorerRows();

    //+1 because indexing starts from 1 in the screen
    std::string cy=std::to_string(exCfg.cy+1);
    std::string cx=std::to_string(exCfg.cx+1);

    // appendBuffer+=cx

    // add "\x1b[cy;cxH" to appendBuffer
    std::string positionCursor ="\x1b[";
    positionCursor+=cy;
    positionCursor+=";";
    positionCursor+=cx;
    positionCursor+="H\r\n";
    appendBuffer+=positionCursor;

    //show cursor after drawing
    appendBuffer+="\x1b[?25h";
    // repositionCursor(40, 20, appendBuffer);
    write(STDOUT_FILENO, appendBuffer.c_str(), appendBuffer.size());
    appendBuffer="";

    int x, y;
    getCursorPosition(y, x);
    int index = y-2;
    appendBuffer+="\x1b[0;0;60b";
    write(STDOUT_FILENO, appendBuffer.c_str(), appendBuffer.size());
    appendBuffer="";
    // std::cout<<x<<" "<<y<<"\n";
}

int main(){

    if(populateCurrDirectory()!=0) die("populateCurrDirectory");
    enableRawMode();
    initExplorer();

    while (1){
        refreshExplorerScreen();
        processKeyPress();
    }

    return 0;
}