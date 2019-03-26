//Equipe Marivaldo Junior, Bruno, Antonio MATA58 Sistemas operacionais 2019.1
//Projeto prático 2 

#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h> //wait
#include <sys/wait.h> //wait
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h> // biblioteca errno
#include <string.h> //strtok
#include <signal.h>
#include <time.h>
#include <unistd.h> //fork


int main(int argc, char* argv[]) {


    // Declare a linha (com buffer e a matriz de palavras)
	char line[4096];
    char* words[100];



    // Iniciar o loop de execução do shell
    while(1) {
        // Definir variáveis de loop
        int counter = 0;

        // Entradas do Shell
    	printf("myshell> ");
        fflush(stdout);
    	char* input = fgets(line, 4096, stdin);

        if(input == NULL) {
            break;
        }
    
        // Define tokenizer
    	char* tok = strtok(line, " \t\n");

        // Sem entradas? Reconfirmar.
        if(tok == NULL) {
            printf("\n");
            continue;
        }

        else {

            // Loop sobre as strings do token
            while(tok != NULL) {
                words[counter] = tok;
                tok = strtok(NULL, " \t\n");
                ++counter;
            }
        }

        // Definir string
        words[counter] = 0;

        // Condicionais para cada comando separado


        // Definir ids de processo fora do loop
        pid_t p_id;
        int status;     
        if(strcmp(words[0], "start") == 0) {
            p_id = fork(); // No começo, bifurque um novo processo

            if(p_id >= 0) {
                if(p_id == 0) {
                    printf("myshell: processo %d iniciado \n", getpid());
                    execvp(words[1], words + 1); // Incrementar palavras por 1, para pular o primeiro comando

                }
                else {
                }
            }

            else {
                perror("forking falhou"); // mostra o erro
                exit(1);
            }
        }
        
        else if(strcmp(words[0], "wait") == 0) {
            pid_t result = wait(&status);

            if(result == -1) {
                printf("myshell: %s\n", strerror(errno));
            }

            else {
                if(status == 0) {
                    printf("myshell: processo %d saiu normalmente com status %d \n", result, WEXITSTATUS(status));
                }
                else {
                    printf("myshell: processo %d saiu anormalmente com sinal %d : %s \n", result, status, strsignal(status) );
                }            
            }
        }
        else if(strcmp(words[0], "run") == 0) {
            // Forking
            p_id = fork();


            // Se for válido, inicie o processo e espere que ele termine
            if(p_id >= 0) {
                if(p_id == 0) {
                    printf("myshell: processo %d iniciado \n", getpid());
                    execvp(words[1], words+1);                    
                }

                else {
                    pid_t exited = waitpid(p_id, &status, 0);

                    if(exited == -1) {
                        printf("myshell: erro de espera: %s", strerror(errno));
                    }

                    if(status == 0) {
                        printf("myshell: processo %d saiu normalmente com status %d \n", exited, WEXITSTATUS(status));
                    } else {
                    printf("myshell: processo %d saiu anormalmente com sinal %d : %s \n",exited, status, strsignal(status) );
                    }
                }

            }

            // Else mostra o erro
            else {
                perror("forking falhou");
                exit(1);
            }

        }
        else if(strcmp(words[0], "kill") == 0) {


            // Mate o processo, armazene o resultado
            pid_t killprocess = atoi(words[1]);
            int result = kill(killprocess, SIGKILL);

            // Se houver um resultado -1, houve um erro
            if(result == -1) {
                printf("myshell: erro de processo: %s \n", strerror(errno));
            }

            // Else, foi morto com sucesso
            else {
                printf("myshell: processo %s morto \n", words[1]);
            }
        }

        else if(strcmp(words[0],"continue") == 0) {
                        // Verifique o status do processo, verifique se ele existe


            // Continuar o processo
            pid_t killprocess = atoi(words[1]);
            int result = kill(killprocess, SIGCONT);
            
            if(result == -1) {
                printf("myshell: erro de processo: %s \n", strerror(errno));
            }

            else {
                printf("myshell: processo %s continuado. \n", words[1]);
            }
        }
        else if(strcmp(words[0], "stop") == 0 ) {
            // Pare o processo
            pid_t killprocess = atoi(words[1]);
            int result = kill(killprocess, SIGSTOP);
            
            if(result == -1) {
                printf("myshell: erro de processo: %s \n", strerror(errno));
            }

            else {
                printf("myshell: processo %s parado. \n", words[1]);
            }

        }
        else if(strcmp(words[0], "exit") == 0) {
            exit(0);
        }
        else if (strcmp(words[0], "quit") == 0) {
            exit(0);
        }
        else {
            printf("myshell: comando desconhecido: %s \n", words[0]);
        }  
    }

    return 0;
}

