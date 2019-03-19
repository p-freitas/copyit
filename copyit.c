#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <signal.h>

#include <time.h>

#include <errno.h>

#include <sys/stat.h>

#include <fcntl.h>

void mensagem(int s){
	printf("Ainda está copiando...\n");
	alarm(1);
}


int main (int argumX, char * argumY[])
{
	// variaveis

	//bytes por linha
	size_t n, b;

	//definindo as as variaveis dos arquivos
	int aOrigem, aDest; 

	//string
	char l[2000];

	//alarme
	int alarme;
	

	
	// permissoes de leitura
	mode_t m = S_IRUSR; 


	// declarando o alarme
	signal(alarme, mensagem);

	//verificando excessões
	// se menos de 3 argumentos forem digitados
	if (argumX > 3){
		printf(" ________________________________________\n");
		printf("|          Use apenas 3 argumentos      |\n");
        printf("|copyit <ArquivoOrigem> <ArquivoDestino>|\n");
        exit(1);
		
		
		
		
	// Verificando se for maior que 3 argumentos
	} 
	
	else if (argumX < 3){
		printf(" ________________________________________\n");
		printf("|          Use apenas 3 argumentos      |\n");
		printf("|copyit <ArquivoOrigem> <ArquivoDestino>|\n");
		exit(1);
		
	}


	// abrindo o arquivo de origem
	if ((aOrigem=open(argumY[1],O_RDONLY,0))<0){
		
		printf(" ________________________________________\n");
		printf("|Não foi possvel abrir %s: %s            |\n",argumY[1],strerror(errno));
		exit(1);
	}

	// criando o arquivo destino
	if ((aDest=creat(argumY[2],m))<0){
		printf(" ________________________________________\n");
		printf("|Não foi possivel criar %s: %s           |\n",argumY[2],strerror(errno));
		exit(1);
	}
	

	// iniciando o alarme
	alarm(1);

	// tranferindo os dados
	while (1) {
		// limpando
		memset(l,'\0',sizeof(l));

		// lendo os dados
		while ((n=read(aOrigem,l,sizeof(l)))<0){
			// reconectando
			if (errno==EINTR){
				continue;
			}
			// se não conseguir ler
			printf(" ________________________________________\n");
			printf("|Não foi possivel ler %s: %s             |\n",argumY[1],strerror(errno));
			exit(1);
			
		}

		// soma os bytes
		b += n;

		// se não houver mais dados
		if (n==0) 
		    break;
	  
		
		// escrevendo no arquivo destino
		while (write(aDest,l,n)<0){
			// reconectar
			if (errno==EINTR){
				continue;
			}

			// se não conseguir
			printf("________________________________________\n");
			printf("|Não foi possivel escrever %s: %s       |\n",argumY[2],strerror(errno));
			exit(1);
		}
	}

	// fechando os arquivos
	close(aOrigem);
	close(aDest);
	
	// se acontecer tudo normalmente
	printf(" _____________________________________________________\n");
	printf("|Copiado: %zu bytes do arquivo: %s para o arquivo: %s.|\n",b,argumY[1],argumY[2]);

	exit(0);
}
