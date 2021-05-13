#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 6000  // PORTA IGUAL PARA CLIENTE E SERVIDOR

char *hello = "Servidor diz: Olá!";

void  socketThread(int clientSocket){
    // Há apenas um processo para que a conexão half-duplex seja mantida 
    //   e o programa não receba e envie mensagem ao mesmo tempo

    int valRead;

    for(;;){
        char buffer[1024] = {0};

        valRead = read(clientSocket, buffer, 1024);
        printf("%s\n", buffer);

        if (valRead == 0 || valRead == -1)
            break;  // CLIENTE DESCONECTOU
                    
        send(clientSocket, hello, strlen(hello), 0);
        printf("Servidor envia mensagem para o cliente %d\n\n", clientSocket);

    }
    printf("Aguardando novo cliente...\n");
    kill(clientSocket, SIGKILL);
}


int main(int argc, char const *argv[]) {
    int valread;
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    pid_t pid;

    //Cria o socket TCP
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {  // CONEXÃO TCP
        perror("Socket falhou");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt))) {
        perror("Setsockopt falhou");
        exit(EXIT_FAILURE);
    }
// FOR MACOS
//     if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
//                    &opt, sizeof(opt))) {
//         perror("Setsockopt falhou");
//         exit(EXIT_FAILURE);
//     }
    
    // Dados do servidor
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0) {
        perror("Bind falhou");
        exit(EXIT_FAILURE);
    }


    printf("Aguardando novo cliente...\n");
    for(;;){
        if (listen(server_fd, 3) < 0) {
            perror("Listen falhou");
            exit(EXIT_FAILURE);
        }

        //recebe um novo socket com dados do cliente
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        //Processo pai
        if ((pid = fork()) == -1){
            close(new_socket);
            continue;
        // processo filho
        } else if(pid == 0) {
            //inicia loop de ouvir e enviar mensagens do/para o cliente
            printf("Cliente %d conectado\n", new_socket);
            socketThread(new_socket);
        }

    }

    return 0;
}
