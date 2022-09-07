/*
Commento di Matteo
32 bit i686-w64-mingw32-gcc -mwindows -lws2_32
64 bit x86_64-w64-mingw32-gcc client_eg.c unisock.c -lws2_32 -o Cliente.exe
*/
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#define HIDDEN static

//https://stackify.dev/577359-redirect-io-of-process-to-windows-socket
//https://stackoverflow.com/questions/20316332/using-windows-sockets-for-a-new-process-i-o
//https://stackoverflow.com/questions/40854092/equivalent-of-dup2-with-windows-socket-in-c
//https://tangentsoft.net/wskfaq/articles/bsd-compatibility.html
//https://handsonnetworkprogramming.com/articles/differences-windows-winsock-linux-unix-bsd-sockets-compatibility/
#if defined __MINGW64__

#include <winsock2.h>
#include <io.h>
#include <stdint.h>

//Whether winsock has been initialized or not. 1 if it has been initialized; 0 else
HIDDEN int __inited_winsock = 0;

/*
Tutte le applicazioni winsock devono
essere inizializzate per essere sicuri che i
socket windows siano supportati dal sistema
*/
//inizializzazione dell’applicazione
HIDDEN int __init_winsock(){

	/*
	La struttura WSADATA contiene informazioni sull’implementazione dei socket windows
	typedef struct WSAData {
		WORD wVersion; //versione per la specifica di socket utilizzato
		WORD wHighVersion; //versione massima supportabile per la specifica di socket windows
		...
	} WSADATA;
	*/
	WSADATA wsaData; //The WSADATA returned by WSAStartup() indicates the actual version of winsock returned.

	/*
	Specifica la versione di socket windows richiesta e
	recupera i dettagli dell’implementazione del socket
	windows specifico
	*/
	//                    v versione di socket windows che il chiamante può usare
	//int WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData);
	//                                                 ^ puntatore alla struttura WSDATA che contiene informazioni per l’implementazione della socket
	//             v specifica il numero di versione di winsock richiesto sul sistema e lo costruisce correttamente
	if (WSAStartup(MAKEWORD(2 ,2), &wsaData) != 0) return -1;
	//The WinSock DLL is acceptable. Proceed
	__inited_winsock = 1;

	return 0;
}

#else

#include <sys/socket.h> //per socket(), bind(), connect()
#include <sys/types.h>
#include <arpa/inet.h> //per sockaddrin         

#endif


//checks whether winsock has been initialized or not. If I haven't compiled the program with MinGW, does nothing
int __check_winsock_init(){

	#if defined __MINGW64__
	//test if we fell in error
	if (__inited_winsock != 1) __init_winsock();
	#endif
}

int unidup2(int oldfd, int newfd){

	#if defined __MINGW64__
	//La chiamata _open_osfhandle trasferisce la proprietà dell'handle al descrittore di file
	int sockfd = _open_osfhandle((intptr_t)oldfd, _O_APPEND);
	if (sockfd < 0){
		printf("errore con _open_osfhandle\n");
		getchar();
		return -1;
	}
	printf("open_os ok\n");
	getchar();
	int result = _dup2(sockfd, newfd);
	if (result < 0){
		printf("errore con _dup2\n");
		getchar();
		return -1;
	}
	printf("dup ok created!\n");
	getchar();
	//_close(sock);
	#else
	#endif
}

//Win-version would return otherwise an int
ssize_t unisend(int sockfd, const void *buf, size_t len, int flags){
	#if defined __MINGW64__
	return (ssize_t)(send((SOCKET)sockfd, (const char *)buf, (int)len, flags)); //SOCKET type is unsigned int
	#else
	return send(sockfd, buf, len, flags);
	#endif
}

//short
ssize_t unirecv(int sockfd, void *buf, size_t len, int flags){

	#if defined __MINGW64__
	return (ssize_t)(recv((SOCKET)sockfd, (char *)buf, (int)len, flags));
	#else
	return recv(sockfd, buf, len, flags);
	#endif

}

int uniclose(int sockfd){

	//test - proceed if the function above return proper value
	#if defined __MINGW64__
	int result = closesocket((SOCKET)sockfd);
	if (result >= 0){
		return WSACleanup(); //call the function when your Windows program is finished using winsock.
	}
	else return result;
	#else
	return close(sockfd);
	#endif

}

//initiate a connection on a socket as original one on Linux
int uniconnect(int sockfd, const struct sockaddr *addr, size_t addrlen){
	
	#if defined __MINGW64__
	return connect((SOCKET)sockfd, addr, (int)addrlen);
	#else
	return connect(sockfd, addr, (socklen_t)addrlen);
	#endif
}

//returns socket descriptor as original one on Linux
//On Windows SHOULD return an unsigned int, while on Linux and macOS returns int
int unisocket(int domain, int type, int protocol){
	//decide if throw an error if conn failed
	__check_winsock_init();

	#if defined __MINGW64__
	//return (int)(socket(domain, type, protocol));
	return (int)(WSASocket(domain, type, protocol, 0, 0, 0));
	#else
	return socket(domain, type, protocol);
	#endif
}

int unibind(int sockfd, const struct sockaddr *addr, size_t addrlen){
	
	#if defined __MINGW64__
	return bind((SOCKET)sockfd, addr, (int)addrlen);
	#else
	return bind(sockfd, addr, (socklen_t)addrlen);
	#endif
}

int unilisten(int sockfd, int backlog){
	#if defined __MINGW64__
	return listen((SOCKET)sockfd, backlog);
	#else
	return listen(sockfd, backlog);
	#endif
}

int uniaccept(int sockfd, struct sockaddr *addr, size_t *addrlen){
	#if defined __MINGW64__
	return (int)(accept((SOCKET)sockfd, addr, (int *)addrlen));
	#else
	return accept(sockfd, addr, (socklen_t *)addrlen);
	#endif
}