// Andrew VanKooten
// Socket.h
// 3-17-2016

#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <sys/poll.h>
#include <iostream>
#include <stdio.h>
using namespace std;

const int NULL_SD = -1;
const int BUFSIZE = 1024;       

// This class contains the functions and members needed to open, close, and send data over a TCP socket
class Socket 
{
    public:     
		Socket();
        Socket(char ipName[], int destPort);       
        ~Socket();
		// create a TCP connection with the provided server name and port
		bool createConnection(string ipName, int destPort); 
		// send a message over the connection 
		int sendMsg(string msg);
		// wait for a message back and return it in a string
		//	ends when there are five reads that return 0 
		string recvMsg();
		// wait for a message back and return it in a string
		//	ends when it sees the <CRNL> delimiter
		string blockRecv();
		// returns if sd != NULL_SD
        bool checkConnection();
		// closes the conection and sets sd to NULL_SD
		bool closeConnection();
    private:
        int port;        
        int sd;                       
        sockaddr_in myAddr;     // my socket address for internet
        sockaddr_in destAddr;   // a destination socket address for internet
        sockaddr srcAddr;       // a source socket address for internet
};

#endif
