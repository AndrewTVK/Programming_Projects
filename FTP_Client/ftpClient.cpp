// Andrew VanKooten
// ftpClient.cpp
// 3-17-2016

#include "ftpClient.h"

ftpClient::ftpClient()
{

}

ftpClient::ftpClient(string server, int serverPort)
{
    connectToServer(server, serverPort);
}

ftpClient::ftpClient(char server[], int serverPort)
{
    string tmp(server);
    connectToServer(tmp, serverPort);
}

ftpClient::~ftpClient()
{
    controlLine.closeConnection();
}

bool ftpClient::connectToServer(string server, int serverPort)
{
    controlLine = Socket();
    serverName = server;
    port = serverPort;
    
    if(!controlLine.createConnection(serverName, port))
    {
        cerr << "Couldn't Connected" << endl;
        return false;
    }
	if(!getReply(220)) return false;
    return true;
}

bool ftpClient::loginToServer()
{
    if(!sendUsername()) return false;
    if(!sendPassword()) return false;
    if(!sendSYST()) return false;
    return true;
}

bool ftpClient::sendUsername()
{
    string username;
    cout << "Name (" << serverName << ":" << getlogin() << "): ";
    getline(cin, username);
	return sendCommand("USER " + username, 331);
}

bool ftpClient::sendPassword()
{
    cout << "Password: ";
    string password;
    getline(cin, password);
	return sendCommand("PASS " + password, 230);
}

bool ftpClient::changeDirectory(string subdir)
{
	if(!controlLine.checkConnection()) return false;	
	return sendCommand("CWD " + subdir, 250);
}

bool ftpClient::sendSYST()
{
	return sendCommand("SYST ", 215);
}

bool ftpClient::listFiles()
{
	if(!controlLine.checkConnection()) return false;    
	int newPort = sendPASV();
	if(newPort < 0) return false;
	Socket dataLine = Socket();
	if(!dataLine.createConnection(serverName, newPort))
    {
        cerr << "Couldn't Connected" << endl;
        return false;
    }
	int pid = fork();
	if(pid < 0)
	{
		cerr << "Failed To Create Child Process" << endl;
		return false;
	}
	//child process
	else if(pid == 0)
	{
		if(controlLine.sendMsg("LIST ") < 0)
    	{
        	cerr << "Couldn't Send Message" << endl;
        	exit(EXIT_FAILURE);
    	}
		string message = controlLine.blockRecv();
		if(message.length() == 0)
    	{
        	cerr << "No Message To Read" << endl;
			exit(EXIT_FAILURE);
    	}
		cout << message;
		exit(EXIT_SUCCESS);
	}
	//parent process
	else
	{
		string message = dataLine.blockRecv();
		if(message.length() == 0)
    	{
        	cerr << "No Message To Read" << endl;
        	return false;
    	}
		cout << message;
		dataLine.closeConnection();	
		waitpid(pid, NULL, 0);
	}

	string message = controlLine.recvMsg();
	if(message.length() == 0)
   	{
       	cerr << "No Message To Read" << endl;
       	return false;
   	}
	cout << message;
	if(getReturnCode(message) != 226)
    {
        cerr << "Wrong Return Code" << endl;
        return false;
    }
	return true;
}

bool ftpClient::sendFile(string filename)
{
	if(!controlLine.checkConnection()) return false;	
	ifstream input(filename.c_str());
	if(!input)
	{
		cerr << "File Failed To Open)" << endl;
		return false;
	}
	stringstream buffer;
	buffer << input.rdbuf();
	string fileContents = buffer.str();
	input.close();
	
	int newPort = sendPASV();
	if(newPort < 0) return false;
	Socket dataLine = Socket();
	if(!dataLine.createConnection(serverName, newPort))
    {
        cerr << "Couldn't Connected" << endl;
        return false;
    }
	
	if(!sendCommand("TYPE I ", 200)) return false;
	if(!sendCommand("STOR " + filename, 150)) return false;

	//send file contents
	if(dataLine.sendMsg(fileContents) < 0)
    {
        cerr << "Couldn't Send Message" << endl;
        return false;
    }
	dataLine.closeConnection();
	
	//get transfer complete message
	if(!getReply(226)) return false;
	return true;
}

bool ftpClient::getFile(string filename)
{
	if(!controlLine.checkConnection()) return false;	
	int newPort = sendPASV();
	if(newPort < 0)
	{
		cerr << "Sending PASV Failed" << endl;		
		return false;
	}
	Socket dataLine = Socket();
	if(!dataLine.createConnection(serverName, newPort))
    {
        cerr << "Couldn't Connected" << endl;
        return false;
    }

	if(!sendCommand("TYPE I ", 200)) return false;

	int pid = fork();
	if(pid < 0)
	{
		cerr << "Failed To Create Child Process" << endl;
		return false;
	}
	else if(pid == 0)
	{
		if(controlLine.sendMsg("RETR " + filename) < 0)
    	{
        	cerr << "Couldn't Send Message" << endl;
			exit(EXIT_FAILURE);
    	}
		string message = controlLine.blockRecv();
 		if(message.length() == 0)
    	{
    	   	cerr << "No Message To Read" << endl;
			exit(EXIT_FAILURE);
    	}
		cout << message;
		if(!getReply(226)) exit(EXIT_FAILURE);;
		dataLine.closeConnection();
		exit(EXIT_SUCCESS);
	}
	else
	{
		string fileContents = dataLine.recvMsg();
		if(fileContents.length() == 0)
    	{
        	cerr << "No Message To Read" << endl;
        	return false;
    	}
		ofstream output(filename.c_str());
		if(!output)
		{
			cerr << "File Failed To Open)" << endl;
			return false;
		}
		output << fileContents;
		output.close();

		waitpid(pid, NULL, 0);
	}
	return true;
}

int ftpClient::sendPASV()
{
	if(controlLine.sendMsg("PASV ") < 0)
    {
        cerr << "Couldn't Send Message" << endl;
        return -1;
    }
    string message = controlLine.recvMsg();
    if(message.length() == 0)
    {
        cerr << "No Message To Read" << endl;
        return -1;
    }
    cout << message;
    if(getReturnCode(message) != 227)
    {
        cerr << "Wrong Return Code" << endl;
        return -1;
    }

	vector<string> messages = delimit(message, ' ');
	string address = messages.back();

    vector<string> tmp = delimit(address, ')');
	vector<string> tmp1 = delimit(tmp[0], ',');
	int newPort = atoi(tmp1.back().c_str());
	tmp1.pop_back();
	newPort += (256 * atoi(tmp1.back().c_str()));
	
	return newPort;
}

int ftpClient::getReturnCode(string message)
{
    if(message.length() < 4)
    {
        return 0;
    }
    char code[4];
    for(int i = 0; i < 3; i++)
    {
        code[i] = message[i];
    }
    code[3] = '\0';
	//cout << code << endl;
    return atoi(code);
}

vector<string> ftpClient::delimit(string message, char delimiter)
{
	vector<string> messages;
    istringstream ss(message);
    string token;
    while(getline(ss, token, delimiter))
    {
        messages.push_back(token);
    }
    return messages;
}

bool ftpClient::sendCommand(string command, int replyCode)
{
	if(controlLine.sendMsg(command) < 0)
    {
        cerr << "Couldn't Send Message" << endl;
        return false;
    }
	string message = controlLine.recvMsg();
    if(message.length() == 0)
    {
        cerr << "No Message To Read" << endl;
        return false;
    }
    cout << message;
    if(getReturnCode(message) != replyCode)
    {
        cerr << "Wrong Return Code" << endl;
        return false;
    }
	return true;
}

bool ftpClient::getReply(int code)
{
	string message = controlLine.recvMsg();
    if(message.length() == 0)
    {
        cerr << "No Message To Read" << endl;
        return false;
    }
    cout << message;
    if(getReturnCode(message) != code)
    {
        cerr << "Wrong Return Code" << endl;
        return false;
    }
	return true;
}

bool ftpClient::commandLine()
{
    vector<string> messages;
    while(true)
    {
		messages.clear();
        cout << "ftp> ";
		string input = "";
		getline(cin, input);
        messages = delimit(input, ' ');
        if(messages.size() < 1) 
        {
            continue;
        }
        string command = messages[0];
		messages.erase(messages.begin());
		if(command == "quit")
		{
			if(!controlLine.checkConnection()) return true;			
			if(!sendCommand("QUIT ", 221)) return false;
			controlLine.closeConnection();			
			return true;
		}
		else if(command == "close")
		{
			if(!controlLine.checkConnection()) continue;
			if(!sendCommand("QUIT ", 221)) return false;			
			controlLine.closeConnection();
		}
		else if(command == "open")
		{
			if(!controlLine.checkConnection()) continue;
			if(messages.size() == 2)
			{
				if(!connectToServer(messages[1], atoi(messages[0].c_str())))
				{
		    		exit(EXIT_FAILURE);
				}
				if(!loginToServer())
				{
					exit(EXIT_FAILURE);
				}
			}
		}
		else if(command == "cd")
		{
			string dir;			
			if(messages.size() == 0)			
			{
				cout << "(remote-directory) ";
                getline(cin, dir);
			}
			else 
			{
				dir = messages[0];
			}			
			if(!changeDirectory(dir))
    		{
    			continue;
    		}	
		}
        else if(command == "ls")
		{
			if(!listFiles()) continue;
		}
		else if(command == "put")
		{
			string filename;
			if(messages.size() == 0)
			{
				cout << "(local-file) ";
				getline(cin, filename); 
			}
			else
			{
				filename = messages[0];
			}
			if(!sendFile(filename))
			{
				continue;
			}
		}
		else if(command == "get")
		{
			string filename;
			if(messages.size() == 0)
			{
				cout << "(remote-file) ";
				getline(cin, filename); 
			}
			else
			{
				filename = messages[0];
			}
			if(!getFile(filename))
			{
				continue;
			}
		}
    }
}
