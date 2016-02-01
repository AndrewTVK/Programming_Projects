// Andrew VanKooten 
// Jan 17, 2016
// server.cpp

// This program creates a socket and then waits for a client to connect
//	with it to recieve its data

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <sys/uio.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <fcntl.h>
#include <iostream>
using namespace std;

const int BUFFSIZE = 1500;
const int NUM_CONNECTIONS = 5;
const int USEC_IN_SEC = 1000000;
int repetitions;
int serverSD, newSD;

// Writes all the data recieved from the socket to dataBuf and writes back 
//	the number of read calls
void SIGIOhandler( int sig ) 
{
	//cout << "Signal Handler" << endl;
	char dataBuf[BUFFSIZE];	

	timeval tim;
	int startCheck = gettimeofday( &tim, NULL );
    if( startCheck < 0 )
    {
        cerr << "Get Time Failed" << endl;
        close( serverSD );
        close( newSD );
        exit( EXIT_FAILURE );
    }
	int startTime = tim.tv_sec * USEC_IN_SEC + tim.tv_usec;
	
	//cout << "Reading dataBuf" << endl;
	int count = 0;
	for( int i = 0; i < repetitions; i++ )
	{
		int inRead = 0;
        int bytesRead = 0;
		while( inRead < BUFFSIZE )
		{
			bytesRead = read( newSD, dataBuf, BUFFSIZE - inRead );
			inRead += bytesRead;
			count++;
		}
	}

	int endCheck = gettimeofday ( &tim, NULL );
	int endTime = tim.tv_sec * USEC_IN_SEC + tim.tv_usec ;
	int elapsedTime = endTime - startTime; // in micro seconds

	//cout << "Writing bytesRead" << endl;
    int bytesWritten = write( newSD, &count, sizeof( int ) );
	if( bytesWritten < 0 )
	{
		cerr << "Write Failed" << endl;
		close( newSD );
		close( serverSD );
		exit( EXIT_FAILURE );
	}
    
	cout << "Data Receiving Time = " << elapsedTime << " usec" << endl;
	close( newSD );
	close( serverSD );
	exit( EXIT_SUCCESS );
}

// Creates a socket and waits for a connection from the client
int main(int argc, char *argv[])
{
	if( argc != 3 ) 
	{
		cerr << "Incorrect Number of Arguments" << endl;
        cerr << "Usage: ./server serverPort repetitions" << endl;
		exit( EXIT_FAILURE );
	}
	
    int serverPort = atoi( argv[1] );
    repetitions = atoi( argv[2] );
    
    if( serverPort < 0 )
    {
        cerr << "Port Number Must Be Positive" << endl;
        exit( EXIT_FAILURE );
    }
    
    if( repetitions < 0 )
    {
        cerr << "Number of Repetitions Must Be Positive" << endl;
        exit( EXIT_FAILURE );
    }
	
	// Build Address
	//cout << "Building address" << endl;
	sockaddr_in acceptSocketAddr;
    bzero( ( char * )&acceptSocketAddr, sizeof( acceptSocketAddr ) );
	acceptSocketAddr.sin_family = AF_INET;
	acceptSocketAddr.sin_addr.s_addr = htonl( INADDR_ANY );
	acceptSocketAddr.sin_port = htons( serverPort );
	
    // Open Socket
	//cout << "Opening socket" << endl;
    serverSD = socket( AF_INET, SOCK_STREAM, 0 );
    if( serverSD < 0 )
    {
        cerr << "Socket Creation Failed" << endl;
        exit( EXIT_FAILURE );
    }
    const int on = 1;
    int sockCheck = setsockopt( serverSD, SOL_SOCKET, SO_REUSEADDR, ( char * )&on, sizeof( int ) );
    if( sockCheck < 0 )
    {
        cerr << "Socket Manipulation Failed" << endl;
        close( serverSD );
        exit( EXIT_FAILURE );
    }
    
    // Bind Socket
	//cout << "Binding" << endl;
    int bindCheck = bind( serverSD, ( sockaddr * )&acceptSocketAddr, sizeof( acceptSocketAddr ) );
    if( bindCheck < 0 )
    {
        cerr << "Bind Failed" << endl;
        close( serverSD );
		exit( EXIT_FAILURE );
    }

	// Listen
	//cout << "Listening" << endl;
	int listenCheck = listen( serverSD, NUM_CONNECTIONS );
    if( listenCheck < 0 )
    {
        cerr << "Listen Failed" << endl;
        close( serverSD );
        exit( EXIT_FAILURE );
    }
	sockaddr_in newSockAddr;
	socklen_t newSockAddrSize = sizeof( newSockAddr );
    
    // Accept
	//cout << "Accepting" << endl;
	newSD = accept( serverSD, ( sockaddr * )&newSockAddr, &newSockAddrSize );
    if( newSD < 0 )
    {
        cerr << "Accept Failed" << endl;
        close( serverSD );
        exit( EXIT_FAILURE );
    }
	
    // Create Signal Handler
	signal( SIGIO, SIGIOhandler );

	// Change this socket into an asynchronous connection
	//cout << "Changing to asynchronous connection" << endl;
	int fcntl1Check = fcntl( newSD, F_SETOWN, getpid( ) );
	int fcntl2Check = fcntl( newSD, F_SETFL, FASYNC );
    if( fcntl1Check < 0 || fcntl2Check < 0 )
    {
        cerr << "fcntl Failed" << endl;
        close( serverSD );
        exit( EXIT_FAILURE );
    }
	
	// Sleep Forever
	//cout << "Sleeping Forever!!" << endl;
	while( true )
	{
		sleep( 1000 );
	}

	exit( EXIT_SUCCESS );
}
