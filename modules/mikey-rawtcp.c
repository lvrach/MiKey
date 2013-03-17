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


#define DEFUALT_HOST_PORT "13050"
#define DEFAULT_DNS 1

#define STATE_INIT 0
#define STATE_ADDR 1
#define STATE_CONN 2

struct sockaddr_in *serv_addr;
int state;
int tcpsocket;

void help() {
	
	printf(" --host ADDRESS\t specify  listener address \n");
	printf(" --port PORT \t specify  listener port \n");
	printf(" --no-dns \t do not use DNS lookup \n");

}

struct sockaddr_in *tcpaddr(struct in_addr *ip, int port ) {

	struct sockaddr_in *addr;

	addr = malloc( sizeof(addr) );
	memset(addr, '\0', sizeof(addr)); 

	//address protocol 
	addr->sin_family = AF_INET;
	
	//set port
	addr->sin_port = htons(port); 
	
	//set ip address
	addr->sin_addr = *ip;


	return addr;

}



int tcplog(const char * );

int conn() {

	//socket with:
	//
	//AF_INET - IPv4 Internet protocols
	//
	//SOCK_STREAM - (aka TCP) 
	//Provides sequenced, reliable, two-way, connection-based byte  streams.
	tcpsocket = socket(AF_INET, SOCK_STREAM, 0);

	//connect to server
	if (connect(tcpsocket, (struct sockaddr *) serv_addr, sizeof(*serv_addr)) < 0) {
		fprintf(stderr, "Connection failed , errno:%i \n", errno);
		return 0;
	}
	

	//send initial message 
	tcplog("Mikey!!!\n");

	return 1;

}


int tcplog(const char *buffer) {

	long int i, size, re;

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
int getFeed(char *buffer) { 

	if (state == STATE_ADDR ) {
		if (!conn() ) {
			return 0;
		}
		state = STATE_CONN;
	}
	if (state == STATE_CONN ) {

		tcplog(buffer);
	}

	return 0;
}

int handleArgs(int argc, char* argv[]) {

	int i;
	int dns;
	char *host;
	struct in_addr *ip;
	int port;

	char *server_port;
	struct hostent *host_info;
	
	//set INIT state
	state = STATE_INIT;

	ip = NULL;
	host = NULL;
	
	//Host default port
	server_port = DEFUALT_HOST_PORT;

	//dns look up is on by default
	dns = 1;

	for (i = 1; i <argc; i++) {

		if (strcmp(argv[i], "--no-dns") == 0 ) {

			dns = 0;
		}
		else if (strcmp(argv[i], "--port") == 0 ) {
			
			if (i+1 < argc && argv[i+1][0] != '-') {
				server_port = argv[i+1];	
				
			}

		}
		else if (strcmp(argv[i], "--host") == 0 ) {
			
			if (i+1 < argc && argv[i+1][0] != '-') {
				host = argv[i+1];	
				
			}

		}
	}


	
	//Host is required
	if (host == NULL ) {
		fprintf(stderr, "Host address not set\n");
		help();
		return 1;
	}
	
	//Port default
	if (server_port == NULL ) {
		server_port = DEFUALT_HOST_PORT;
	}



	if (dns == 0 ) {
		
		printf("DNS lookup disabled \n");
		//convert string to numeric ip
		ip = (struct in_addr *) malloc(sizeof(struct in_addr));
		if (inet_pton(AF_INET, host, ip ) <= 0) {
			fprintf(stderr, "Invalid ip address");			
			return 1;
		}

	}
	else {
		
		printf("resolving %s \n", host);
		host_info = gethostbyname(host);			
		
		if (host_info == NULL ) {
			fprintf(stderr, "Unable to look up %s\n", host);
			return 1;
		}
		ip = (struct in_addr *) host_info->h_addr;

	}


	port = atoi(server_port);
	if (port <= 0) {
		fprintf(stderr, "Invalid port \n");
		return 1;
	}
	
	if (ip == NULL ) {

		fprintf(stderr, "Invalid ip\n");
	}

	
	printf("Connecting to %s:%i\n", host, port);
	serv_addr = tcpaddr(ip, port);
	state = STATE_ADDR;

	if (conn() ) {
		state = STATE_CONN;
		return 0;
	}
	else {
		return 1;
	}
}
