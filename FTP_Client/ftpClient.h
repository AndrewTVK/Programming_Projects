// Andrew VanKooten
// ftpClient.h
// 3-17-2016

#ifndef _FTPCLIENT_H_
#define _FTPCLIENT_H_

#include "Socket.h"

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
using namespace std;


// This class handles all of the operations of an ftp client.
//	To use it just call the commandLine function and use it like the original ftp client
class ftpClient
{
    public:
		ftpClient();
		ftpClient(string server, int serverPort);
		ftpClient(char server[], int serverPort);
		~ftpClient();
		
		// creates a TCP connection with the inputed server and port number
		bool connectToServer(string server, int serverPort);
		// main user interface
		bool commandLine();
		// log on to FTP server using your username and password
		bool loginToServer();
		// cd
		bool changeDirectory(string subdir);
		//  ls
		bool listFiles();
		// put
		bool sendFile(string filename);
		// get
		bool getFile(string filename);
    private:
        Socket controlLine;
        string serverName;
        int port;
		// sends the username to the server
        bool sendUsername();
		// send the password to the server
		bool sendPassword();
		// gets the system info
		bool sendSYST();
		// returns the port number of a passive port
		int sendPASV();
		// sends a command to the server and compmares the returen code with reply code
		bool sendCommand(string command, int replyCode);
		// gets a reply for a server and compares the reply code with code
		bool getReply(int code);
		// gets the return code out of a string of characters
		int getReturnCode(string message);
		//seperates the words of a string by a delimiter and returns them in a vector
        vector<string> delimit(string message, char delimiter);
};


#endif
