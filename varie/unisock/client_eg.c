#include "unisock.h"
#include <string.h>
#include <unistd.h>

void printError(const char *data){
	printf("%s", data);
	getchar();
}

int main()
{
	int sock;
	struct sockaddr_in server;
	char message[1000] , server_reply[2000];
	
	//Create socket
	sock = unisocket(AF_INET , SOCK_STREAM , 0);
	if (sock < 0)
	{
		printError("Could not create socket");
		return -1;
	}
	printf("socket created!\n");
	
	server.sin_addr.s_addr = inet_addr("192.168.1.166"); //from str to IP format
	server.sin_family = AF_INET;
	server.sin_port = htons(8080);

	//Connect to remote server
	if (uniconnect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		printError("connect failed. Error");
		return 1;
	}
	
	printf("Connected\n");

	unirecv(sock , server_reply , 2000 , 0);
	printf("Server reply :");
	printf("%s", server_reply);
	getchar();
	uniclose(sock); //Ctrl C => nell'handler della signal aggiungere uniclose
	return 0;
}
