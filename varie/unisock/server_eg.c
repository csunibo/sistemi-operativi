#include "unisock.h"
#include <string.h>

void printError(const char *data){
	printf("%s", data);
	getchar();
}

int main(){

	struct sockaddr_in server_sockaddr, client_sockaddr;
	size_t sockaddr_in_size = sizeof(struct sockaddr_in);

	//creating a socket
	int sock = unisocket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0){
		printError("Could not create socket\n");
		return -1;
	}
	printf("socket created!\n");

	//where we want to bind
	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_port = htons(8080); //htons takes local endian-ness and converts it to network byte order (big endian)
	server_sockaddr.sin_addr.s_addr = inet_addr("192.168.1.154");

	//bind
	if (unibind(sock, (struct sockaddr *)&server_sockaddr, sockaddr_in_size) < 0){
		printError("Could not bind\n");
		return -1;
	}
	printf("binded successfully\n");

	//listening to incoming conns...
	unilisten(sock, 3);

	//accept connection from an incoming client
	printf("waiting for incoming connection...\n");
	int client_sock = uniaccept(sock, (struct sockaddr *)&client_sockaddr, (size_t *)&sockaddr_in_size);
	if (client_sock < 0){
		printError("accept failed\n");
		return -1;
	}
	printf("connection accepted\n");/*
	char *message = "Hello Client , I have received your connection. But I have to go now, bye\n";
	unisend(client_sock , message , strlen(message) , 0);
	*/
	printf("messaggio inviato!\n");
	getchar();
	

	return 0;
}