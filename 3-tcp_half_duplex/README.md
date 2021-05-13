
# TCP Half-Duplex

Uma aplicação TCP, no qual o servidor consiga atender mais de cliente ao mesmo tempo, para promover diálogo bidirecional (half-duplex, ou seja, enquanto um dos lados fala o outro escuta e vice-versa, numa ordem pré-definida), usando a system call fork().

Nos testes de validação, é importante que vários clientes possam dialogar com o servidor, mas não é necessário que clientes dialoguem entre si.

## Alunas

16/0121817 – Gabriela Medeiros da Silva

16/0122180 – Geovana Ramos Sousa Silva

## Como funciona?

Execute primeiro o servidor e depois os clientes. Ambos vão enviar e receber mensagens, o servidor envia mensagem após receber mensagens dos cliente (simulando uma resposta ou confirmação). As aplicações não encerram, a não ser que você interrompa a execução.

Caso vc encerre o servidor, o cliente também será encerrado.

Caso você encerre o cliente, o servidor permance ativo, e aguarda a conexão de um novo cliente.