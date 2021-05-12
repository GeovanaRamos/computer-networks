#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 6000  // PORTA IGUAL PARA CLIENTE E SERVIDOR

int main(int argc, char const *argv[]) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Servidor diz: Olá!";

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {  // CONEXÃO TCP
        perror("Socket falhou");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt))) {
        perror("Setsockopt falhou");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0) {
        perror("Bind falhou");
        exit(EXIT_FAILURE);
    }

    while (1) {  // PARA CONSEGUIR CONECTAR NOVOS CLIENTES

        printf("Aguardando novo cliente...\n");

        if (listen(server_fd, 3) < 0) {
            perror("Listen falhou");
            exit(EXIT_FAILURE);
        }

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                 (socklen_t *)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        printf("Cliente conectado!\n");

        pid_t child_pid = fork();

        if (child_pid == 0) {  // PROCESSO FILHO
            while (1) {
                send(new_socket, hello, strlen(hello), 0);
                printf("Servidor enviou uma mensagem\n");
                sleep(1);
            }

        } else {  // PROCESSO PAI

            while (1) {
                valread = read(new_socket, buffer, 1024);
                printf("%s\n", buffer);
                if (valread == 0 || valread == -1)
                    break;  // CLIENTE DESCONECTOU
            }
            kill(child_pid, SIGKILL);  // MATA PROCESSO FILHO
        }
    }

    return 0;
}
