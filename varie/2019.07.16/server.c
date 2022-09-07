/* Il file /public/socktest.c è il sorgente di un programma che crea una connessione TCP con la macchina */
/* locale a porta 2222. E' possibile testarlo con netcat. Se aprite due shell, nella prima digitate: */
/* nc -l -p 2222 */
/* nella seconda */
/* socktest */
/* potete scrivere ora una riga di testo nella seconda finestra e comparira' nella prima, poi scrivendo una */
/* riga nella prima compare nella seconda ed enrambi i programmi terminano. */
/* Scopo di questo esercizio è di trasformare/completare il sorgente in modo che diventi una chat fra i */
/* due terminali... ogni riga scritta nella prima finestra deve comparire nella seconda e viceversa. */
/* Terminando uno dei due programmi, termina anche l'altro. */
/* Occorre consegnare due versioni del programma: la prima deve fare uso della system call select, la */
/* seconda versione deve usare la system call poll. */
/* Scrivere una terza versione del programma dell'esercizio 1 facendo uso delle system call */
/* epoll_create/epoll_ctl/epoll_wait */
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
//TCP server => quello che riceve, lo manda a tutti gli altri che assieme a lui printeranno il contenuto
//quello che vuole scrivere => lo printa e lo manda a tutti gli altri...
//https://www.youtube.com/watch?v=u3COf_kJ8sk

enum _PIPE {READ, WRITE};
int main(int argc, char **argv){


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

    fclose(stdout);
    struct epoll_event events[MAX_EPOLL_EVENTS]; memset(events, 0, sizeof(events));
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1){
        perror("\nepoll_create1()");
        exit(EXIT_FAILURE);
    }

    //init
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1){
        perror("\nsocket()");
        exit(EXIT_FAILURE);
    }

    //where we want to bind
    struct sockaddr_in server_sock_addr;
    memset(&server_sock_addr, 0, sizeof(struct sockaddr_in) );
    size_t sockaddr_in_size = sizeof(struct sockaddr_in);
    server_sock_addr.sin_family = AF_INET;
    server_sock_addr.sin_port = htons(PORT); //htons takes local endian-ness and converts it to network byte order (big endian)
    server_sock_addr.sin_addr.s_addr = inet_addr(HOST);

    //binding
    if (bind(server_sock, (const struct sockaddr *)&server_sock_addr, sizeof(struct sockaddr_in)) == -1){
        perror("\nbind()");
        exit(EXIT_FAILURE);
    }

    if (listen(server_sock, 1) == -1){
        perror("\nlisten()");
        exit(EXIT_FAILURE);
    }

    int conversation_inited = FALSE;
    int client_sock = -1;
    struct sockaddr_in client_sock_addr; memset(&client_sock_addr, 0, sizeof(struct sockaddr_in));
    client_sock = accept(server_sock, (struct sockaddr *)&client_sock_addr, (socklen_t *)&sockaddr_in_size);
    if (client_sock == -1){
        perror("\naccept()");
        exit(EXIT_FAILURE);
    }

    struct epoll_event e; bzero(&e, sizeof(struct epoll_event));
    e.events = EPOLLIN;
    e.data.fd =STDIN_FILENO;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &e) == -1) {
        perror("epoll_ctl: client_sock OUT");
        exit(EXIT_FAILURE);
    }

    while (!conversation_inited){
        memset(&client_sock_addr, 0, sizeof(struct sockaddr_in));
        char buf[512]; memset(buf, 0, sizeof(buf));
        if (recv(client_sock, buf, sizeof(buf), 0) == -1){
            perror("\nrecv()");
            exit(EXIT_FAILURE);
        }
        if (strncmp(buf, "socktest", (unsigned long)strlen("socktest")) == 0){
            conversation_inited = TRUE;
            struct epoll_event ev; bzero(&ev, sizeof(struct epoll_event));
            ev.events = EPOLLIN;
            ev.data.fd = client_sock;
            if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_sock, &ev) == -1) {
                perror("epoll_ctl: client_sock");
                exit(EXIT_FAILURE);
            }
        }
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
                /* fprintf(stderr, "\n%s", buffer); //è già printato mentre lo digito! */
                if (send(client_sock, buffer, strlen(buffer), 0) == -1){
                    perror("\nsend()");
                    exit(EXIT_FAILURE);
                }
            }
            else if (events[i].data.fd == client_sock){
                if (recv(client_sock, buffer, sizeof(buffer), 0) == -1){
                    perror("\nrecv()");
                    exit(EXIT_FAILURE);
                }
                fprintf(stderr, "%s", buffer);
                //se contenuto scritto una sola volta => righe sotto vanno commentate
                /* if (send(client_sock, buffer, strlen(buffer), 0) == -1){ */
                /*     perror("\nsend()"); */
                /*     exit(EXIT_FAILURE); */
                /* } */
            }
        }
    }
    exit(EXIT_SUCCESS);
}
