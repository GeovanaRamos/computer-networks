#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 6000  // PORTA IGUAL PARA CLIENTE E SERVIDOR

int main(int argc, char const *argv[]) {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Cliente1 diz: Olá!";
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {  // CONEXÃO TCP
        printf("\nErro na criação do socket\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nEndereço inválido\n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConexão falhou\n");
        return -1;
    }

    pid_t child_pid = fork();

    if (child_pid == 0) {  // PROCESSO FILHO
        while (1) {
            send(sock, hello, strlen(hello), 0);
            printf("Cliente1 enviou uma mensagem\n");
            sleep(2);
        }

    } else {  // PROCESSO PAI
        while (1) {
            valread = read(sock, buffer, 1024);
            printf("%s\n", buffer);

            if (valread == 0 || valread == -1)  // SERVIDOR CAIU
                break;
        }
        kill(child_pid, SIGTERM);  // MATA PROCESSO FILHO
    }

    return 0;
}
