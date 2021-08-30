#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include "crypto.h"

#define TAM_MAX_MENS_INIT       300
#define TAM_MAX_MENS_CLIENT     2000
#define NUM_MAX_CONEX_CLIENT    1
#define PORT_SOCKET_SERVER      8888

void socket_error(char *msg);

char buffer_key[TAM_BUFFER_KEY];
char buffer_iv[TAM_BUFFER_IV];
unsigned char ciphertext[TAM_BUFFER_CRIPTO];
int decryptedtext_len, ciphertext_len;

void socket_error(char *msg)
{
    perror(msg);
    exit(0);
}


int main(int argc, char *argv[]){
    int socket_desc, client_sock, c, read_size;         
                                                    
    struct sockaddr_in server, client;                  
                                                        
    char client_message[TAM_MAX_MENS_CLIENT];       
    char msg_init[TAM_MAX_MENS_INIT];
    char msg_client[TAM_MAX_MENS_CLIENT];           

    unsigned char decryptedtext[TAM_BUFFER_DECRIPTO];
    int i;

    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();

    load_key_iv();

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1){
        perror("Impossivel Criar Socket");
        return 1;
    }
    puts("Socket Criado com Sucesso!");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT_SOCKET_SERVER);

    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0){
        perror("Erro ao Fazer Bind");
        return 1;
    }
    puts("Bind Feito com Sucesso!");

    listen(socket_desc, NUM_MAX_CONEX_CLIENT);

    puts("Aguardando Conexao...");
    c = sizeof(struct sockaddr_in);

    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);

    if (client_sock < 0){
        perror("Falha ao Aceitar Conexao");
        return 1;
    }
    puts("Conexao Aceita!");

    while ((read_size = recv(client_sock, client_message, 2000, 0)) > 0){
        ciphertext_len = read_size;
       
        memcpy(ciphertext, client_message, read_size);
        decryptedtext_len = decrypt(ciphertext, ciphertext_len, buffer_key, buffer_iv, decryptedtext);

        printf("\n\nMensagem Decriptografada: %s\n\n", decryptedtext);
        printf("%s ",ciphertext);

        memset(msg_client, 0, TAM_MAX_MENS_CLIENT);
        memcpy(msg_client, decryptedtext, strlen(decryptedtext));
        printf("\n\nMensagem a ser enviada de volta ao client: %s.", msg_client);

        ciphertext_len = encrypt(msg_client, strlen((char *)msg_client), buffer_key, buffer_iv, ciphertext);

        write(client_sock, ciphertext, ciphertext_len);
        memset(msg_client, 0, TAM_MAX_MENS_CLIENT);
        memset(client_message, 0, TAM_MAX_MENS_CLIENT);
    }

    if (read_size == 0){
        puts("Client Desconectado. A aplicacao Encerrada.");
        fflush(stdout);
        close(client_sock);
    }
    else if (read_size == -1)
        perror("Recv failed");

    return 0;
}
    