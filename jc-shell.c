/*
*Exercise nº2 of JC-SHELL
*Developed by group nº10

*ELIÚDE PATRÍCIO DE CARVALHO VEMBA - (ID Github) HelioPC
*LUDMILO HUEBA CAMBAMBI - (ID Github) Ludmilo-cambambi
*PEDRO MANUEL DOMINGOS - (ID Github) pedro7-7-7
*LUCÍLIO TÉRCIO GOMES - (ID Github) luciliogomez
*/

#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>
#include "commandlinereader.h"
#include "ourheadfile.h"
#include <fcntl.h>
#define __USE_POSIX 1
#define __USE_XOPEN_EXTENDED
#include <signal.h>
#include "list.h"
#include "listterm.h"

#define MAXPAR 2

void sighandler(int);

int exit1;
int numChildren;
int startread;
int exit2;
FILE *regfile;
pthread_mutex_t mutex;
pthread_mutex_t mutex2;
pthread_cond_t cond_var;
pthread_cond_t cond_var_2;
pthread_cond_t cond_var_3;
LIST_PROC *list;
REGARQ arq_info;
LISTTERMS *lsterms;
struct sigaction sact, old;

int main(int argc, char **argv){
	int numargs, numlines, fdin, fdproc;
	char buffer[BUFFERSIZE];
	char *argvector[VECTORSIZE];
	char filename[FILENAMESIZE];
	void *result, *result2;
	pid_t pid;
	pthread_t monitorThread;
	pthread_t terminalThread;

	sact.sa_handler = sighandler;
	sigemptyset(&sact.sa_mask);
	sact.sa_flags = 0;
	
	sigaction(SIGINT, &sact, NULL);

	regfile = fopen(OUTPUT_TXT, READ_AND_APPEND);

	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&mutex2, NULL);
	pthread_cond_init(&cond_var, NULL);
	pthread_cond_init(&cond_var_2, NULL);
	pthread_cond_init(&cond_var_3, NULL);

	if(!access(NAMED_PIPE, F_OK)){
        perror("\033[31mNamed pipe \"jcshell-in\" not cleared");
        exit(ENOENT);
    }

	if(mkfifo(NAMED_PIPE, S_IRUSR | S_IWUSR) < 0){
		perror("\033[31mError opening the fifo");
		exit(EXIT_FAILURE);
	}

	CLEAR();

	numChildren = 0;
	startread = 0;
	list = process_new();
	lsterms = listterm_init();

	printf("\t\t%sJC-SHELL %s%s\n", PURPLE, VERSION, NORM);

	if(argc > 1 || argv[1] != NULL)
		puts("\033[31m\nThis program doesn't need any args\033[m\n");

	numlines = filelines(regfile);
	
	if(numlines != 0 && numlines % 3 == 0 && fileload());

	else{
		fclose(regfile);
		regfile = fopen(OUTPUT_TXT, READ_AND_WRITE);
		arq_info.iternum = -1;
		arq_info.totaltime = 0;
	}

	/*Creates the monitor thread*/
	if(pthread_create(&monitorThread, NULL, monitor_Thread, NULL) < 0){
			puts("\033[31mError creating monitor thread\033[m");
			exit(THREAD_CREATE_FAILED);
	}

	if(pthread_create(&terminalThread, NULL, regTerminalThread, NULL) < 0){
			puts("\033[31mError creating terminals handling thread\033[m");
			exit(THREAD_CREATE_FAILED);
	}

	if((fdin = open(NAMED_PIPE, O_RDONLY | O_NONBLOCK, S_IRUSR)) < 0){
		perror("\033[31mCouldn\'t open named pipe \"jcshell-in\"");
		exit(OPEN_FILE_FAILED);
	}

	dup2(fdin, STDIN_FILENO);

	while(1){
		/*Read the command*/
		fflush(stdin);

		A:
		pthread_mutex_lock(&mutex2);
		while(lsterms->first == NULL){
			pthread_cond_wait(&cond_var_3, &mutex2);
		} /* esperar pelo sinal da função regterminal */
		pthread_mutex_unlock(&mutex2);

		numargs = readLineArguments(argvector, VECTORSIZE, buffer, BUFFERSIZE);

		if(startread){
			startread = 0;
			goto A;
		}

		/* if(feof(stdin) != 0) strcpy(argvector[0], "exit"); */

		/* else */ if(numargs < 1) continue;

		/* Examines the command */
		switch(command(argvector[0])){
			case PID:
				break;

			case EXIT_GLOBAL:
				/*Waits for the task monitors to finish before exit.*/

				exitall(SIGINT);
				
				pthread_cond_signal(&cond_var);
				
				pthread_mutex_lock(&mutex);
				exit1 = 1; /* Critical section */
				pthread_mutex_unlock(&mutex);
				pthread_mutex_lock(&mutex);
				exit2 = 1; /* Critical section */
				pthread_mutex_unlock(&mutex);
				pthread_join(monitorThread, &result);
				pthread_join(terminalThread, &result2);

				freelsterm(lsterms);
				process_print(list);
				process_destroy(list);
				pthread_mutex_destroy(&mutex);
				pthread_cond_destroy(&cond_var);
				pthread_cond_destroy(&cond_var_2);
				pthread_cond_destroy(&cond_var_3);
				fclose(regfile);
				close(fdin);
				unlink(NAMED_PIPE);
				unlink(NAMED_PIPE_REG);
				fclose(stdin);
				exit(EXIT_SUCCESS);
				break;

			case EXIT:
				if(exitterminal(buffer)){
					puts("\n\033[32mSuccess sending SIGQUIT to jcshell-termina"
					"l.\033[m");
				}else{
					puts("\n\033[31mFailure to send SIGQUIT to jcshell-termina"
					"l.\033[m");
				}

				break;

			case INT_CLEAR:
				CLEAR();
				printf("\t\t%sJC-SHELL %s%s\n", PURPLE, VERSION, NORM);
				break;

			/* Creates a child process and execute the program */
			case PATHNAME:
				pthread_mutex_lock(&mutex);
				while(numChildren == MAXPAR) pthread_cond_wait(&cond_var_2, &mutex);
				pthread_mutex_unlock(&mutex);

				pid = fork();

				if((int) pid == -1){
					fprintf(stderr, "%sChild process creation failed.%s\n",
					RED, NORM);
					break;
				}

				else if((int) pid == 0) {
					sprintf(filename, "jcshell-out-%d.txt", (int) getpid());

					if((fdproc = open(filename, O_CREAT | O_WRONLY | O_TRUNC,
					S_IRUSR | S_IWUSR)) == -1) exit(FILE_CREATE_FAILED);

					dup2(fdproc, STDOUT_FILENO);

					execv(argvector[0], argvector);

					close(fdproc);
				}

				else{
					pthread_mutex_lock(&mutex);
					numChildren++; /* Critical section */
					insert_new_process(list, pid, time(NULL));
					/*End of critical section*/
					pthread_mutex_unlock(&mutex);
					pthread_cond_signal(&cond_var);
				}

				break;

			default:
				puts("\033[0;31mInexisting file\033[m");
		}
	}
	return 0;
}

void sighandler(int sig){
	startread = 1;
	exitall(sig);
}
