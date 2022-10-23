# Peer-to-Peer Group Based File Sharing System

### Execution
    1. Inside client directory open terminal and run "make" command.
    2. Now run ./client <IP>:<PORT> tracker_info.txt to run a peer.
    3. Inside tracker directory open terminal and run "make" command.
    4. To run both trackers run the below commands on two different terminals
        4.1 ./tracker tracker_info.txt 1
        4.2 ./tracker tracker_info.txt 2
    5. Use make clean to clear the .o files and the executable file.
    6. Inside the folder client infile client_functions.h set #define DESTDIR = any value for the user folder to get created at
    
### Technologies Used
    C++, Linux, Socket Programming, SHA1 hash(For verifying each chunk of a file), Multi-threading(For parallel downloads from multiple peers)

### Goal:
	A group based file sharing system where users can share, download files from the group they belong to. Authentication
	is done for the users. Download is parallel with multiple pieces from multiple peers. SHA1 hash is used to verify that 
	each chunk is not corrupted. Two trackers are implemented for fault tolerence and load balancing.
	Random first and rarest pieces first piece selection algorithms are implemented for downloading from peers.
	
### Commands:
	1. Login: login <user_id> <password>
	2. Create Group: create_group <group_id>
	3. Join Group: join_group <group_id>
	4. Leave Group: leave_group <group_id>
	5. List Pending Join: list_requests<group_id>
	6. Accept Group Joining Request: accept_request <group_id> <user_id>
	7. List All Group In Network: list_groups
	8. List All sharable Files In Group: list_files <group_id>
	9. Upload File: upload_file <file_path> <group_id>
	10. Download File: download_file <group_id> <file_name> <destination_path>
	11. Logout: logout
	12. Show_downloads: show_downloads
		Output format:
		[D] [grp_id] filename
		[C] [grp_id] filename D(Downloading), C(Complete)
	13. Stop sharing: stop_share <group_id> <file_name>
		
