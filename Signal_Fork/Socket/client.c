#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include "crypto.h"

#define TAM_MAX_MENS_INIT       300
#define TAM_MAX_MENS_CLIENT     2000
#define NUM_MAX_CONEX_CLIENT    1
#define PORT_SOCKET_SERVER      8888

#define ARQUIVO_KEY             "key.txt"
#define ARQUIVO_IV              "iv.txt"

char buffer_key[TAM_BUFFER_KEY];
char buffer_iv[TAM_BUFFER_IV];
unsigned char ciphertext[TAM_BUFFER_CRIPTO];
int decryptedtext_len, ciphertext_len;

void socket_error(char *msg);

void socket_error(char *msg){
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]){
    int sockfd, portno, n;
    int i;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    unsigned char decryptedtext[TAM_BUFFER_DECRIPTO];

    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();

    load_key_iv();

    portno = PORT_SOCKET_SERVER;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
        socket_error("\nERRO: Impossivel Abrir Socket Nesta Porta");

    server = gethostbyname("127.0.0.1");

    if (server == NULL){
        fprintf(stderr, "\nERRO: O Host Informado Nao Existe.\n");
        exit(0);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        socket_error("\nERRO: Impossivel Conectar ao Host.");
    else
        printf("Conexao ao Host bem Sucedida!\n\n");

    printf("Mensagem a ser enviada: ");
    memset(buffer, 0x00, sizeof(buffer));
    fgets(buffer, sizeof(buffer), stdin);

    ciphertext_len = encrypt(buffer, strlen((char *)buffer), buffer_key, buffer_iv, ciphertext);
    n = write(sockfd, ciphertext, ciphertext_len);
    
    if (n < 0)
        socket_error("ERRO: Impossivel Enviar Mensagem Criptografada ao Host");

    bzero(buffer, 256);

    n = read(sockfd, ciphertext, 255);
    if (n < 0)
        socket_error("ERRO: Falha ao Receber Dados do Host");

    ciphertext_len = n;
    decryptedtext_len = decrypt(ciphertext, ciphertext_len, buffer_key, buffer_iv, decryptedtext);

    printf("\n[Mensagem Recebida do Servidor]\n");

    for (i = 0; i < decryptedtext_len; i++)
        printf("%c", decryptedtext[i]);

    return 0;
}