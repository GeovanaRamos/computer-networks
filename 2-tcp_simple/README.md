
# TCP Simples

Uma  aplicação TCP, para  diálogo  cliente/servidor,  para  diálogo  bidirecional, cujo lado servidor permanece  com  a  conexão  ativa  até  que  o  cliente  informe  ao  servidor  que  não  há  mais  nada  a  enviar; quando isso ocorre, apenas o cliente encerra a conexão, mas o servidor continua ativo, aguardando novas conexões.

## Alunas

16/0121817 – Gabriela Medeiros da Silva
16/0122180 – Geovana Ramos Sousa Silva

## Como funciona? 

Execute primeiro o servidor e depois o cliente. Ambos vão enviar uma mensagem e receber uma mensagem a cada 1 segundo. As aplicações não encerram, a não ser que você interrompa a execução.

Caso vc encerre o servidor, o cliente também será encerrado.

Caso você encerre o cliente, o servidor permance ativo, e aguarda a conexão de um novo cliente. 

