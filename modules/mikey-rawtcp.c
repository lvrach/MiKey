/*
 * Mikey Module:
 * Raw tcp client 
 * 
 */
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <stdarg.h>


#define DEFUALT_SERVER_PORT "13050";

char *server_ip;
char *server_port;
int tcpsocket;

void help() {
	
	printf(" --server-ip SERVER_IP\t specify server ip add \n");
	printf(" --server--port SERVER_PORT \t");

}

int tcplog(const char * );

int init() {

	struct sockaddr_in serv_addr; 
	int port;

	
	memset(&serv_addr, '0', sizeof(serv_addr)); 

	printf("Connecting: %s:%s", server_ip, server_port);
	
	//socket with:
	//
	//AF_INET - IPv4 Internet protocols
	//
	//SOCK_STREAM - (aka TCP) 
	//Provides sequenced, reliable, two-way, connection-based byte  streams.
	tcpsocket = socket(AF_INET, SOCK_STREAM, 0);


	//address protocol 
	serv_addr.sin_family = AF_INET;
	
	//set port
	port = atoi(server_port);
	if(port <= 0) {
		printf("Invalid port");
		return 0;
	}
	serv_addr.sin_port = htons(atoi(server_port)); 
	
	//set ip
	if(inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
		printf("Invalid ip address \n");
		return 0;
	} 
	
	//connect to server
	if(connect(tcpsocket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("Connection failed , errno:%i \n", errno);
		return 0;
	} 

	//send initial message 
	tcplog("Mikey!!!\n");

	return 1;

}


int tcplog(const char *buffer) {

	long int i, size, re;

	printf("send\n");
	size = (strlen(buffer)+1) * sizeof(buffer[0]);

	for (i = 0; i < size; i+=re) {
		
		re = write(tcpsocket, &buffer[i], size);
		if(re < 0) {
			printf("Error \n");
			break;
		}
	}

	return size;

}
void getFeed(char *buffer) { 
	printf("b: %s",buffer);	
	tcplog(buffer);

	
}

void handleArgs(int argc, char* argv[]) {

	int i;
	
	server_ip = NULL;
	server_port = NULL;

	for (i = 1; i <argc; i++) {

		if (strcmp(argv[i], "--server-ip") == 0 ) {

			if (i+1 < argc && argv[i+1][0] != '-') {
				server_ip = argv[i+1];
			}

		}
		else if (strcmp(argv[i], "--server-port") == 0 ) {
			
			if (i+1 < argc && argv[i+1][0] != '-') {
				server_port = argv[i+1];	
				
			}

		}


	}

	//set Defaults 
	if (server_port == NULL ) {
		server_port = DEFUALT_SERVER_PORT;
	}
	if (server_ip == NULL ) {
		server_ip = "127.0.0.1";
	}

	//required check
	if (server_ip == NULL ) {

		fprintf(stderr, "You must define Server Address\n");
		help();
		return 1;
	}
	if (server_port == NULL ) {

		return 1;
	}

	if(init() ) {
		return 0;
	}	
}
