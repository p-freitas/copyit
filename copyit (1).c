#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

void display_message(int s){ //Mensagem a ser exibida caso ultrapasse 1 segundo
	printf("copyit: ainda copiando...\n");
	alarm(1);
}

int main (int argc, char * argv[])
{
	// declarando variaveis

     
	int fsource, fdest; // fsource = arquivo fonte. fdest = arquivo destino (essas variaveis receberão retornos de open() e write())
	mode_t mode = S_IRUSR; // permissoes de leitura
	char dados[2000]; // string a ser copiada de um arquivo para outro
	unsigned int n, bytes; // n = número de bytes em cada linha. bytes = número de bytes no total

	// declare signal alarm
	signal(SIGALRM, display_message);

	// Verificação quando o programa é executado
	// Caso tenha nenhum ou apenas 1 argumento
	if (argc<3){
		printf("copyit: Poucos argumentos!\n");
		printf("use: copyit arquivoFonte arquivoDestino\n");
		exit(1);
	// Caso tenha mais de dois argumentos
	} else if (argc>3){
		printf("copyit: Muitos argumentos!\n");
                printf("use: copyit arquivoFonte arquivoDestino\n");
                exit(1);
	}

	// Abrir o arquivo fonte
	if ((fsource=open(argv[1],O_RDONLY,0))<0){ //verifica se foi possivel fazer a chamada da função open(arquivoDestino, permisão de leitura)
		printf("copyit: Nao foi possivel abrir o arquivo %s: %s\n",argv[1],strerror(errno));
		exit(1);
	}

	// Criar o arquivo destino
	if ((fdest=creat(argv[2],mode))<0){ //caso o arquivo não exista, verifica se é possível criá-lo
		printf("copyit: Nao foi possivel criar o arquivo %s: %s\n",argv[2],strerror(errno));
		exit(1);
	}

	// Começa o alarme
	alarm(1);

	// Transferência dos dados
	while (1) { //loop infinito
		//alarm(1);
		// limpa os dados
		memset(dados,'\0',sizeof(dados));

		// Leitura dos dados do arquivo fonte
		while ((n=read(fsource,dados,sizeof(dados)))<0){
			// Caso ocorra uma interrupção, tentar ler a linha novamente
			if (errno==EINTR){
				continue;
			}
			// Caso contrário, exibir erro e encerrar
			printf("copyit: Nao foi possivel ler do arquivo %s: %s\n",argv[1],strerror(errno));
			exit(1);
			
		}

		// Incrementar número de bytes
		bytes += n;

		// Se todos os dados foram transferidos, encerrar 
		if (n==0) break;
		
		// Escrever dados no arquivo destino
		while (write(fdest,dados,n)<0){
			// Caso ocorra interrupção, tentar escrever novamente
			if (errno==EINTR){
				continue;
			}

			// Caso contrário, exibir mensgem de erro e encerrar
			printf("copyit: Nao foi possivel escrever no arquivo %s: %s\n",argv[2],strerror(errno));
			exit(1);
		}
	}

	// Fechar os arquivos
	close(fsource);
	close(fdest);
	
	// Caso não ocorra nenhum erro, exibe mensagem de sucesso
	printf("copyit: Foram copiados %u bytes do arquivo %s para o arquivo %s.\n",bytes,argv[1],argv[2]);

	exit(0);
}