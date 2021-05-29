Sistema com dois processos A (um daemon) e B (processo em foreground). 

O processo A deve monitorar um diretório esperando a criação de um arquivo "teste.x". 

Quando esse arquivo for criado, o processo A deve enviar um sinal SIGURSR1 para o processo B.

Ao receber esse sinal, o processo B deve imprimir ok na tela e continuar sua execução.
