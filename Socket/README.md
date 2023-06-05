## Socket TCP Seguro
As mensagens enviadas nesta comunicação são criptografadas com uma criptografia simétrica AES 256 CBC, utilizando a biblioteca OpenSSL.

### Instalação Necessária 
```bash
$ sudo apt-get install libssl-dev
```

* Sever:

> Executar
```
gcc server.c crypto.c crypto.h -o server -lcrypto
```
> Compilar
```
./server
```

* Client:

> Executar
```
gcc client.c crypto.c crypto.h -o client -lcrypto
```
> Compilar
```
./client
```
    
