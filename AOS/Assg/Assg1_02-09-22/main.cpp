// 1. Use of system() library function for command mode functions is not allowed.
// 2. Use of system commands like ls, cp, mv, mkdir etc are not allowed. You have to 
//    implement your own versions of these commands.
// 3. Use of ncurses.h is strictly prohibited. 

/** includes **/
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <bits/stdc++.h>

/** define **/
#define CTRL_KEY(k) ((k) & 0x1f)
#define LOWTOUP(k) ((k) & 0xdf)
#define EXPLORER_VERSION "0.0.1"

/** data **/
struct explorerConfig{
    int cx, cy;
    int explorerRows;
    int explorerColumns;
    struct termios origTermios;
}exCfg;


/** terminal **/
void die(const char* s){
    //screen cleared and cursor repositioned
    //we can also see the error after clearing of the screen
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);

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
    //byte by byte instead of line by line(now as soon as q is pressed the program exits)
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
    printf("\r\n&buffer[1]: '%s'\r\n", &buffer[1]);

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

/** append buffer **/
std::string appendBuffer="";

/** input **/
void processKeyPress(){
    char c = readKey();

    if (iscntrl(c)) {
      printf("%d\r\n", c);
    } else {
      printf("%d ('%c')\r\n", c, c);
    }

    //IF CTRL+q exit
    //here since the 5th bit is also discarded 
    //it doesnt matter if its lowercase/uppercase q.
    //5th bit set to 1 is lowercase
    c=LOWTOUP(c);
    if(c=='Q'){
        //to clear screen when exiting
        write(STDOUT_FILENO, "\x1b[2J", 4);
        exit(0);
    } 
}

/** output **/
void drawExplorerRows(){
    int n = exCfg.explorerRows;
    for(int i=0; i<n; i++){
        //\x1b[K is for clearing each line
        //2 erases the whole line, 1 erases the part of the line to the left of the cursor, 
        //and 0 erases the part of the line to the right of the cursor. 
        //0 is the default argument, and that’s what we want, 
        //so we leave out the argument and just use <esc>[K.
        if(i==n-1) appendBuffer+="|\x1b[K";
        else if(i==2){
            std::string title = "GP File Explorer --- version ";
            title+=EXPLORER_VERSION;
            int titleLength=title.size();
            int padding = (exCfg.explorerColumns-titleLength)/2;
            for(int i=0; i<padding; i++){
                title+="-";
                title="-"+title;
            }
            if(titleLength<=exCfg.explorerColumns) appendBuffer+=title;
            else appendBuffer+=title.substr(0, exCfg.explorerColumns);
            appendBuffer+="\x1b[K\r\n";
        }
        else appendBuffer+="|\x1b[K\n\r";
    }
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
    //row and colum starts from 1 and not 0.
    appendBuffer+="\x1b[H";
    // write(STDOUT_FILENO, "\x1b[H", 3);

    drawExplorerRows();

    appendBuffer+="\x1b[H";
    // write(STDOUT_FILENO, "\x1b[H", 3);

    //+1 because indexing starts from 1 in the screen
    std::string cy=std::to_string(exCfg.cy+1);
    std::string cx=std::to_string(exCfg.cx+1);

    // appendBuffer+=cx

    // add "\x1b[cy;cxH" to appendBuffer
    std::string positionCursor ="\x1b[";
    positionCursor+=cy;
    positionCursor+=";";
    positionCursor+=cx;
    positionCursor+="H";
    appendBuffer+=positionCursor;

    //show cursor after drawing
    appendBuffer+="\x1b[?25h";
    
    write(STDOUT_FILENO, appendBuffer.c_str(), appendBuffer.size());
    appendBuffer="";
}

/** init **/
void initExplorer(){
    exCfg.cx=0;
    exCfg.cy=0;
    if(getWindowSize(exCfg.explorerRows, exCfg.explorerColumns)==-1) die("getWindowSize");
}

int main(){

    enableRawMode();
    initExplorer();

    while (1){
        refreshExplorerScreen();
        processKeyPress();
    }

    return 0;
}