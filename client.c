/*********************************************

Name: Jake Seawell
Date: 11/02/19
Assignment: Project1 - Server-Chat
Description: This program, called client.c,
interacts with server.py on a certain port,
and creates a back-and-forth chat over sockets.
See README.txt for compile/run directions.

Resources:
https://beej.us/guide/bgnet/html/
https://realpython.com/python-sockets/


*********************************************/

// LIBRARIES //
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAXDATASIZE 501 // max number of chars we can get at once 


//---------------------------------------------------------

// GET SOCKET ADDRESS FUNCTION
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


// MAIN FUNCTION
int main(int argc, char *argv[])
{
	//integers
	int clientQuit = 0;
	int serverQuit = 0;
	int sockfd, numbytes, nameChars, bufChars, rv;

	//sizes
	size_t buf_size = MAXDATASIZE;
	size_t username_size = 11;

	//strings (char ptrs)
	char* PORT = argv[2];
	char* userName;
	char* messageReceive;
	char* messageSend;
    	char* buf;

	//struct for sockets
	struct addrinfo hints, *servinfo, *p;

	//INET String
    	char s[INET6_ADDRSTRLEN];


	//Error if 3 arguments not given
    	if (argc != 3) {
        	fprintf(stderr,"usage: client hostname\n");
        	exit(1);
    	}

/***********************************************************************************/
	
// MODIFIED FROM RESOURCES

	//Socket initializtion
    	memset(&hints, 0, sizeof hints);
    	hints.ai_family = AF_UNSPEC;
    	hints.ai_socktype = SOCK_STREAM;

    	if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        	fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        	return 1;
    	}

    	// loop through all the results and connect to the first we can
    	for(p = servinfo; p != NULL; p = p->ai_next) {
        	if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            		perror("client: socket error");
            		continue;
        	}

        	if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            		close(sockfd);
            		//perror("client: connect error");
            		continue;
        	}

        	break;
    	}

    	if (p == NULL) {
        	fprintf(stderr, "client: failed to connect\n");
        	return 2;
    	}

    	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
    	printf("client: connecting to %s\n", s);

    	freeaddrinfo(servinfo);
/**********************************************************************************/

// MY CODE:

	//get username
	printf("What is your name? Enter it below and press enter:\n");
	userName = (char *)malloc(username_size * sizeof(char));
	nameChars = getline(&userName, &username_size, stdin);
	fflush(stdin);
	userName[strcspn(userName, "\n")] = 0;//remove newline at end
	nameChars--;

	//send username to server
	send(sockfd, userName, nameChars, 0);

	//Loop until client quits
	while (clientQuit == 0){

		//Message prompt
		printf("%s > ", userName);

		//get user message
		buf = (char *)malloc(buf_size * sizeof(char));
		bufChars = getline(&buf, &buf_size, stdin);
		fflush(stdin);
		buf[strcspn(buf, "\n")] = 0; 
		bufChars--;
		
		//If client quits, exit
		if (strcmp(buf, "\\quit") == 0){
			printf("Client ended connection.\n");
			clientQuit = 1;
		}

		//allocate and clear send buffer
		messageSend = (char *)malloc((nameChars+bufChars+4) * sizeof(char));
		memset(messageSend, '\0', sizeof(messageSend));

		//put buf in message
		sprintf(messageSend, "%s", buf);	
	
		//send message
		numbytes = 0;
		numbytes = send(sockfd, messageSend, bufChars, 0);
		if (numbytes < 0){	
			perror("send failure");
			exit(1);
		}	

		//allocate and clear receive buffer
		messageReceive = (char *)malloc(100 * sizeof(char));
		memset(messageReceive, '\0', sizeof(messageReceive));
		
		//receive message
		numbytes = 0;
		numbytes = recv(sockfd, messageReceive, 100, 0);
		if (numbytes < 0){
			perror("receive failure");
			exit(1);
		}
		
		//if server quits, exit
		if (strcmp(messageReceive, "\\quit") == 0){
			printf("Server ended connection.\n");
			serverQuit = 1;
		}

		//exit upon quit from client or server
		if (clientQuit == 1 || serverQuit == 1)
			exit(2);
		
		//print reply to screen
		printf("Server > %s\n", messageReceive);
		
		//reset and free receive buffer
		memset(messageReceive, '\0', 100);
		free(messageReceive);

		//reset and free buffer
		memset(buf, '\0', sizeof(buf));
		free(buf);
		
		//reset and free send buffer
		memset(messageSend, '\0', sizeof(messageSend));
		free(messageSend);
	}

	//close socket/connection
    	close(sockfd);

    return 0;
}
