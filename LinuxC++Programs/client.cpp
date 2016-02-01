// Andrew VanKooten 
// Jan 17, 2016
// client.cpp

// This program creates a socket and connects to the server write data over to it
//	and recieve back the number of read calls

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/uio.h>
#include <string>
#include <netdb.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include <iostream>
using namespace std;


const int BUFFSIZE = 1500;
const int USEC_IN_SEC = 1000000;

void inputCheck(int serverPort, int repetitions, int nBufs, int bufSize, int type);

// creates a socket and connects to the server and writes the dataBuf to it
int main(int argc, char *argv[])
{
	if( argc != 7 ) 
	{
		cerr << "Incorrect Number of Arguments" << endl;
		exit( EXIT_FAILURE );
	}

	int serverPort = atoi( argv[1] );
	int repetitions = atoi( argv[2] );
	int nBufs = atoi( argv[3] );
	int bufSize = atoi( argv[4] );
	char *serverName = argv[5];
	int type = atoi( argv[6] );

    inputCheck( serverPort, repetitions, nBufs, bufSize, type );

	char dataBuf[nBufs][bufSize];
	
	// Get Hostent structure to communicate to server
    struct hostent *host = gethostbyname( serverName );
	
	// Build Address
	//	cout << "Building address" << endl;
	sockaddr_in sendSocketAddr;
    bzero( ( char * )&sendSocketAddr, sizeof( sendSocketAddr ) );
	sendSocketAddr.sin_family = AF_INET;
	sendSocketAddr.sin_addr.s_addr = inet_addr( inet_ntoa( *(struct in_addr*) ( *host->h_addr_list ) ) );
	sendSocketAddr.sin_port = htons( serverPort );

	// Open socket and connect
	//cout << "Opening socket" << endl;
    int clientSD = socket(AF_INET, SOCK_STREAM, 0);	
	if( clientSD < 0 )
	{
		cerr << "Socket Creation Failed" << endl;
		exit( EXIT_FAILURE );
	}
	//cout << "Connecting socket" << endl;
    int connectCheck = connect( clientSD, ( sockaddr * )&sendSocketAddr, sizeof( sendSocketAddr ) );
    if( connectCheck < 0 )
    {
        cerr << "Connection Failed" << endl;
        close( clientSD );
        exit( EXIT_FAILURE );
    }
	
	// Write the dataBuf to the server
	timeval tim;
    int startCheck = gettimeofday( &tim, NULL );
	if( startCheck < 0 )
	{
		cerr << "Get Time Failed" << endl;
		exit( EXIT_FAILURE );
	}
	int startTime = tim.tv_sec * USEC_IN_SEC + tim.tv_usec;

	//cout << "Writing dataBuf" << endl;
    int bytesWritten;
    for( int j = 0; j < repetitions; j++ )
	{
        //cout << "Starting Repetition " << j << endl;
        if( type == 1 )
        {
            //cout << "Entering Type 1" << endl;
            for( int i = 0; i < nBufs; i++ )
            {
                //cout << "Iteration: " << i << endl;
                bytesWritten = write( clientSD, dataBuf[i], bufSize );
                //cout << "Bytes Written: " << bytesWritten << endl;
                if( bytesWritten < 0 )
                {
                	cerr << "Write Failed" << endl;
                	close( clientSD );
                	exit( EXIT_FAILURE );
                }
            }
        }
        else if( type == 2 )
        {
            //cout << "Entering Type 2" << endl;
            struct iovec vector[nBufs];
            for( int i = 0; i < nBufs; i++ )
            {
//                cout << "Building structure. Iteration: " << i << endl;
                vector[i].iov_base = dataBuf[i];
                vector[i].iov_len = bufSize;
            }
            //cout << "Attempting to write" << endl;
            bytesWritten = writev( clientSD, vector, nBufs );
            //cout << "Bytes written: " << bytesWritten << endl;
            if( bytesWritten < 0 )
            {
            	cerr << "Write Failed" << endl;
            	close( clientSD );
            	exit( EXIT_FAILURE );
            }
        }
        else
        {
            //cout << "Entering Type 3" << endl;
            bytesWritten = write( clientSD, dataBuf, BUFFSIZE );
            //cout << "Bytes written: " << bytesWritten << endl;
            if( bytesWritten < 0 )
            {
            	cerr << "Write Failed" << endl;
            	close( clientSD );
            	exit( EXIT_FAILURE );
            }
		}
//        cout << "Ending Repetition " << j << endl;
	}
    
    //cout << "Writes complete" << endl;
	
    int lapCheck = gettimeofday( &tim, NULL );
	if( lapCheck < 0 )
	{
		cerr << "Get Time Failed" << endl;
		exit( EXIT_FAILURE );
	}
	int lapTime = tim.tv_sec * USEC_IN_SEC + tim.tv_usec;
	int dataSendTime = lapTime - startTime;
    
	// Read the number of reads from the server
	//cout << "Getting number of reads" << endl;
	int numReads;
    int bytesRead = read(clientSD, &numReads, sizeof( int ) );
    if( bytesRead < 0 ) {
        cerr << "Read Failed" << endl;
        exit( EXIT_FAILURE );
    }
    int endCheck = gettimeofday( &tim, NULL );
    if( endCheck < 0 )
	{
		cerr << "Get Time Failed" << endl;
		exit( EXIT_FAILURE );
	}
	int endTime = tim.tv_sec * USEC_IN_SEC + tim.tv_usec;
	int totalTime = endTime - startTime;
	
	cout << "Data Sending Time = " << dataSendTime << " usec, Round Trip Time = " << totalTime << " usec, #Reads = " << numReads << endl;
    close( clientSD );
    exit( EXIT_SUCCESS );
}

// Input validation
void inputCheck(int serverPort, int repetitions, int nBufs, int bufSize, int type)
{
    if( type < 1 || type > 3)
    {
        cerr << "Type Can Only Be 1, 2, or 3" << endl;
        exit( EXIT_FAILURE );
    }
    
    if( nBufs <= 0 || bufSize <= 0)
    {
        cerr << "The Number Of Buffers Or The Buffer Size Must Be Larger Than Zero" << endl;
        exit( EXIT_FAILURE );
    }
    
    if( nBufs * bufSize != BUFFSIZE )
    {
        cerr << "Buffer Size Does Not Equal 1500" << endl;
        exit( EXIT_FAILURE );
    }
    
    if( serverPort < 0 )
    {
        cerr << "The Server Port Must Be Larger Than Zero" << endl;
        exit( EXIT_FAILURE );
    }
    
    if( repetitions < 0 )
    {
        cerr << "The Number Of Repetitions Must Be Larger Than Zero" << endl;
        exit( EXIT_FAILURE );
    }
}
