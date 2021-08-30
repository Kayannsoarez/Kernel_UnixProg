#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "crypto.h"

char buffer_key[TAM_BUFFER_KEY];
char buffer_iv[TAM_BUFFER_IV];
unsigned char ciphertext[TAM_BUFFER_CRIPTO];
int decryptedtext_len, ciphertext_len;

void handle_errors(void){
    ERR_print_errors_fp(stderr);
    abort();
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext){
    EVP_CIPHER_CTX *ctx;
    int len;             
    int ciphertext_len;

    if (!(ctx = EVP_CIPHER_CTX_new()))
        handle_errors();

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handle_errors();

    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handle_errors();

    ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        handle_errors();
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext)
{
    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len;

    if (!(ctx = EVP_CIPHER_CTX_new()))
        handle_errors();

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handle_errors();

    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handle_errors();

    plaintext_len = len;


    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        handle_errors();
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    return plaintext_len;
}

void load_key_iv(void){
    FILE *arq;
    char *p_Key;
    char *p_IV;
    int ctd_bytes;

    arq = fopen(ARQUIVO_KEY, "r");
    if (arq == NULL)
        printf("Erro: Impossivel Carregar a KEY\n");
    else{
        ctd_bytes = 0;
        p_Key = &buffer_key[0];
        while (ctd_bytes < TAM_BUFFER_KEY){
            *p_Key = fgetc(arq);
            p_Key++;
            ctd_bytes++;
        }
    }
    fclose(arq);
    printf("[KEY] Carregada com Sucesso.");

    arq = fopen(ARQUIVO_IV, "r");
    if (arq == NULL)
        printf("Erro: Impossivel Carregar o IV\n");
    else{
        ctd_bytes = 0;
        p_IV = &buffer_iv[0];
        while (ctd_bytes < TAM_BUFFER_IV){
            *p_IV = fgetc(arq);
            p_IV++;
            ctd_bytes++;
        }
    }
    fclose(arq);
    printf("\n[IV]  Carregado com Sucesso.\n");
}