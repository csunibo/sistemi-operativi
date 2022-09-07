#include <netinet/in.h>
#include <errno.h>
#include <strings.h>
#include <sys/epoll.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#define PORT 8888
#define HOST "127.0.0.1"
#define TRUE 1
#define FALSE 0
#define MAX_EPOLL_EVENTS 2
//client => quello che vuole scrivere lo manda al server
//quello che riceve lo printa
enum _PIPE {READ, WRITE};
int main() {

    /* int fd_pipe[2]; //per farsì che l'input non venga mostrato */
    /* if (pipe(fd_pipe) == -1){ */
    /*     perror("\npipe()"); */
    /*     exit(EXIT_FAILURE); */
    /* } */

    /* if (dup2(fd_pipe[WRITE], STDIN_FILENO) == -1){ */
    /*     perror("\ndup2()"); */
    /*     exit(EXIT_FAILURE); */
    /* } */

    /* close(fd_pipe[WRITE]); */
    fclose(stdout); //NON RIUSCITO: GLI ALTRI SONO IN GRADO DI SCRIVERE SEMBRA...
    struct epoll_event events[MAX_EPOLL_EVENTS]; memset(events, 0, sizeof(events));
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1){
        perror("\nepoll_create1()");
        exit(EXIT_FAILURE);
    }

    struct epoll_event e; bzero(&e, sizeof(struct epoll_event));
    e.events = EPOLLIN;
    e.data.fd = STDIN_FILENO;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &e) == -1) {
        perror("epoll_ctl: STDIN_FILENO");
        exit(EXIT_FAILURE);
    }


    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1){
        perror("\nsocket()");
        exit(EXIT_FAILURE);
    }


    struct sockaddr_in server_sock_addr;
    memset(&server_sock_addr, 0, sizeof(struct sockaddr_in) );
    size_t sockaddr_in_size = sizeof(struct sockaddr_in);
    server_sock_addr.sin_family = AF_INET;
    server_sock_addr.sin_port = htons(PORT); //htons takes local endian-ness and converts it to network byte order (big endian)
    server_sock_addr.sin_addr.s_addr = inet_addr(HOST);

    if (connect(sock, (const struct sockaddr *)&server_sock_addr, (socklen_t) sizeof(server_sock_addr)) == -1){
        perror("\nconnect()");
        exit(EXIT_FAILURE);
    }

    struct epoll_event ev; bzero(&ev, sizeof(struct epoll_event));
    ev.events = EPOLLIN;
    ev.data.fd = sock;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock, &ev) == -1) {
        perror("epoll_ctl: sock");
        exit(EXIT_FAILURE);
    }
    for (;;) {
        char buffer[512]; bzero(buffer, sizeof(buffer));
        int nfds = epoll_wait(epoll_fd, events, MAX_EPOLL_EVENTS, -1);
        if (nfds == -1){
            perror("\nepoll_wait()");
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < nfds; i++){
            if (events[i].data.fd == STDIN_FILENO){
                /* if (read(fd_pipe[READ], buffer, sizeof(buffer)) == -1){ */
                if (fgets(buffer, sizeof(buffer), stdin) == NULL){
                    perror("\nfgets()");
                    exit(EXIT_FAILURE);
                }
                //ha già printato il contenuto mentre lo digitava
                if (send(sock, buffer, strlen(buffer), 0) == -1){
                    perror("\nsend()");
                    exit(EXIT_FAILURE);
                }
            }
            else if (events[i].data.fd == sock){
                if (recv(sock, buffer, sizeof(buffer), 0) == -1){
                    perror("\nrecv()");
                    exit(EXIT_FAILURE);
                }
                fprintf(stderr, "%s", buffer);
            }
        }
    }

    exit(EXIT_SUCCESS);
}
