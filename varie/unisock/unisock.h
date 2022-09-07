#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define HIDDEN static

#if defined __MINGW64__

#include <winsock2.h>

HIDDEN int __init_winsock();

#else

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#endif

int __check_winsock_init();
int uniclose(int sockfd);
int uniconnect(int sockfd, const struct sockaddr *addr, size_t addrlen);
int unisocket(int domain, int type, int protocol);
int unibind(int sockfd, const struct sockaddr *addr, size_t addrlen);
int unilisten(int sockfd, int backlog);
int uniaccept(int sockfd, struct sockaddr *addr, size_t *addrlen);
ssize_t unirecv(int sockfd, void *buf, size_t len, int flags);
ssize_t unisend(int sockfd, const void *buf, size_t len, int flags);
int unidup2(int oldfd, int newfd);