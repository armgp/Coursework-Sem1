# Advanced OS
## Assignment #3 - Peer-to-Peer Group Based File Sharing System

### Execution
    1. Inside client directory open terminal and run "make" command.
    2. Now run ./client <IP>:<PORT> tracker_info.txt to run a peer.
    3. Inside tracker directory open terminal and run "make" command.
    4. To run both trackers run the below commands on two different terminals
        4.1 ./tracker tracker_info.txt 1
        4.2 ./tracker tracker_info.txt 2
    5. Use make clean to clear the .o files and the executable file
