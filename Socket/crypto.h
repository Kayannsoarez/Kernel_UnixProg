#ifndef CRYPTO_H
#define CRYPTO_H

#define TAM_BUFFER_CRIPTO 		1024
#define TAM_BUFFER_DECRIPTO 	1024
#define TAM_BUFFER_KEY 			32
#define TAM_BUFFER_IV 			16

#define ARQUIVO_KEY             "key.txt"
#define ARQUIVO_IV              "iv.txt"

void handle_errors(void);
int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext);
int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext);
void load_key_iv(void);

#endif //CRYPTO_H





