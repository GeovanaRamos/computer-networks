#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0
#define PORT 6000  // PORTA IGUAL PARA CLIENTE E SERVIDOR

int main(int argc, char *argv[]) {
    int opt = TRUE;
    int master_socket, addrlen, new_socket, client_socket[30],
        max_clients = 30, activity, i, valread, sd;
    int max_sd;
    struct sockaddr_in address;

    char *msg_conection = "Conexão feita!";
    char buffer[1025];

    fd_set readfds;

    // INICIA LISTA DE CLIENTES
    for (i = 0; i < max_clients; i++) {
        client_socket[i] = 0;
    }

    if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket falhou");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
                   sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind falhou");
        exit(EXIT_FAILURE);
    }

    if (listen(master_socket, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    addrlen = sizeof(address);
    printf("Aguardando novo cliente...\n");

    while (TRUE) {
        FD_ZERO(&readfds);
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        for (i = 0; i < max_clients; i++) {
            sd = client_socket[i];

            if (sd > 0)
                FD_SET(sd, &readfds);

            if (sd > max_sd)
                max_sd = sd;
        }

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR)) {
            printf("Select falhou");
        }

        // NOVO CLIENTE
        if (FD_ISSET(master_socket, &readfds)) {
            if ((new_socket = accept(master_socket,
                                     (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
                perror("Accept falhou");
                exit(EXIT_FAILURE);
            }

            printf("Nova conexão, socket fd=%d, ip=%s, porta=%d\n",
                   new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));

            if (send(new_socket, msg_conection, strlen(msg_conection), 0) != strlen(msg_conection)) {
                perror("Send falhou");
            }

            // SALVA NOVO CLIENTE
            for (i = 0; i < max_clients; i++) {
                if (client_socket[i] == 0) {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n", i);

                    break;
                }
            }
        }

        // AÇÃO DE UM CLIENTE ANTIGO
        for (i = 0; i < max_clients; i++) {
            sd = client_socket[i];

            if (FD_ISSET(sd, &readfds)) {
                if ((valread = read(sd, buffer, 1024)) == 0) {
                    // DESCONECTOU
                    getpeername(sd, (struct sockaddr *)&address,
                                (socklen_t *)&addrlen);
                    printf("Desconectou, ip=%s, porta=%d\n",
                           inet_ntoa(address.sin_addr), ntohs(address.sin_port));

                    close(sd);
                    client_socket[i] = 0;
                }

                // PROPAGAR RESPOSTA
                else {
                    printf("Mensagem: %s\n", buffer);
                    buffer[valread] = '\0';
                    for (int j = 0; j < max_clients; j++) {
                        if (i != j) {
                            int inner_sd = client_socket[j];
                            send(inner_sd, buffer, strlen(buffer), 0);
                        }
                    }
                    printf("Propagou mensagem!\n");
                }
            }
        }
    }

    return 0;
}
