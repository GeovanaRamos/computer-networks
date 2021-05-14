
# TCP Full-Duplex

Produzir novo código TCP com a system call select(), no qual os clientes se conectem ao servidor TCP e consigam dialogar entre si, num diálogo full-duplex, ou seja, os dois lados podem iniciar o diálogo, sem uma ordem pré-definida. Nesse caso, tudo o que um usuário digitar deve chegar aos pares conectados ao servidor TCP naquele momento.

## Alunas

16/0121817 – Gabriela Medeiros da Silva

16/0122180 – Geovana Ramos Sousa Silva

## Como funciona?

Execute primeiro o servidor e depois os clientes. Ambos vão enviar e receber mensagens, o servidor propaga a mensagem para todos os outros clientes conectados após receber uma mensagem de um dos clientes. As aplicações não encerram, a não ser que você interrompa a execução.

Caso você encerre o servidor, o cliente também será encerrado.

Caso você encerre o cliente, o servidor permance ativo. Ele está sempre aceitando conexão de outros clientes, até atingir 30 clientes.