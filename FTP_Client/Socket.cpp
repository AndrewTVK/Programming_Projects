// Andrew VanKooten
// Socket.cpp
// 3-17-2016

#include "Socket.h"

Socket::Socket()
{
    sd = NULL_SD;
}

Socket::Socket(char ipName[], int destPort)
{
    // Get the host entry corresponding to this destination ipName
    struct hostent* host = gethostbyname(ipName);
    if(host == NULL) 
    {
        cerr << "Cannot find hostname." << endl;
    }

    bzero((char*)&destAddr, sizeof(destAddr)); 
    destAddr.sin_family = AF_INET;            
    destAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(in_addr*)*host->h_addr_list));
    destAddr.sin_port = htons(destPort);  // set the destination port

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0)
    {
        cerr << "Socket creation failed" << endl;
    }

    if( connect(sd, (sockaddr *)&destAddr, sizeof(destAddr)) < 0)
    {
        cerr << "Connect failed" << endl;
        close(sd);
    }
}

Socket::~Socket() 
{
    if (sd != NULL_SD)
    {
        close(sd);
    }
}

bool Socket::createConnection(string ipName, int destPort)
{
    // Get the host entry corresponding to this destination ipName
    hostent* host = gethostbyname(ipName.c_str());
    if(host == NULL) 
    {
        cerr << "Cannot find hostname." << endl;
		return false;
    }

    bzero((char*)&destAddr, sizeof(destAddr)); 
    destAddr.sin_family = AF_INET;            
    destAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(in_addr*)*host->h_addr_list));
    destAddr.sin_port = htons(destPort);

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0)
    {
        cerr << "Socket creation failed" << endl;
		return false;
    }

    if(connect(sd, (sockaddr *)&destAddr, sizeof(destAddr)) < 0)
    {
        cerr << "Connect failed" << endl;
        close(sd);
		return false;
    }
    return true;
}

bool Socket::checkConnection()
{
    return sd != NULL_SD;
}

bool Socket::closeConnection()
{
    if(sd != NULL_SD)
    {
        close(sd);
		sd = NULL_SD;
        return true;
    }
    return false;
}

int Socket::sendMsg(string msg)
{
	msg = msg + "\r\n";
	return write(sd, msg.c_str(), msg.length());
}

string Socket::recvMsg()
{
	pollfd ufds;
	ufds.fd = sd;
	ufds.events = POLLIN;
	ufds.revents = 0;
	
	char buffer[BUFSIZE];
	string message = "";
	bzero(buffer, BUFSIZE);
	
	int msgSize = 0;
	int count = 0;
	while(poll(&ufds, 1, 1000) > 0)
	{
	    msgSize = read(sd, buffer, BUFSIZE);
		message.append(buffer);
		bzero(buffer, BUFSIZE);
		if(msgSize == 0) 
		{
			count++;
			if(count == 5) break;
		}
	}	
	return message;
}

string Socket::blockRecv()
{
	char buffer[BUFSIZE];
	string message = "";
	bzero(buffer, BUFSIZE);
	
	int msgSize = 0;
	int counter = 0;
	int tmp = 0;
	while(true)
	{
		msgSize += read(sd, buffer, BUFSIZE);
		message.append(buffer);
		bzero(buffer, BUFSIZE);
		if (message[message.length()-1] == '\n')
		{
			break;
		}
	}
	return message;
}
