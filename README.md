# socket_chat
Disponível no repositório: https://github.com/MarcusMedeiros99/socket-chat

Apresentação em https://youtu.be/-P39SlYxJ4A

A branch da última entrega é a mod3

Uma aplicação que simula uma troca de mensagens usando sockets TCP.

# Autor

Marcus Vinícius Medeiros Pará (Nº USP 11031663) 

# Ambiente utilizado para teste
![image](https://user-images.githubusercontent.com/39343824/172079558-127ac5f9-c73d-41c2-a5a8-bfc93935ab35.png)
GCC 12.1.0

# Compilando
Antes de rodar pela primeira vez, use o comando ``make all``.

Para iniciar o servidor em uma porta: ``./server <port>``.

Para iniciar um cliente e conectar ao servidor em uma porta: ``./client <port>``.

# Instruções
O servidor recebe as mensagens do cliente e retorna uma mensagem confirmando o recebimento. O cliente pode enviar mensagens simplesmente digiando e dando enter.
