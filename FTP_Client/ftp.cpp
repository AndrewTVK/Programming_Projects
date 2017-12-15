// Andrew VanKooten
// ftp.cpp
// 3-17-2016

#include "ftpClient.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <stdlib.h>
using namespace std;

const int FTP_PORT = 21;

// driver for the ftpClient class
//	can be used with or without any command line arguments
int main(int argc, char *argv[])
{
    if(argc > 2)
    {
        cerr << "Incorrect number of arguments" << endl;
        cerr << "Usage: ./ftp serverName" << endl;
        exit(EXIT_FAILURE);
    }
	else if(argc == 1)
	{
		ftpClient Client = ftpClient();
		if(!Client.commandLine())
		{
			exit(EXIT_SUCCESS);
		}
	}
	else
	{
		string server(argv[1]);

		ftpClient Client = ftpClient();
		if(!Client.connectToServer(server, FTP_PORT))
		{
		    exit(EXIT_FAILURE);
		}
		if(!Client.loginToServer())
		{
		    exit(EXIT_FAILURE);
		}
		if(!Client.commandLine())
		{
			exit(EXIT_SUCCESS);
		}
   	}
    
    return 0;
}


