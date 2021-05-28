/*
*Exercise nº2 of JC-SHELL
*Developed by group nº10

*ELIÚDE PATRÍCIO DE CARVALHO VEMBA - (ID Github) HelioPC
*LUDMILO HUEBA CAMBAMBI - (ID Github) Ludmilo-cambambi
*PEDRO MANUEL DOMINGOS - (ID Github) pedro7-7-7
*LUCÍLIO TÉRCIO GOMES - (ID Github) luciliogomez
*/

#include <stdio_ext.h>
#include <sys/wait.h>
#include <time.h>
#include "commandlinereader.h"
#include "ourheadfile.h"
#include <semaphore.h>

#define MAXPAR 3
#define BUFFERSIZE 100
#define VECTORSIZE 7


pthread_mutex_t mutex;
int Exit;
int numChildren;
LIST_PROC *list;
sem_t sem;
sem_t sem_sleep;

int main(int argc, char **argv){
	int pid, numargs;
	char buffer[BUFFERSIZE];
	char *argvector[VECTORSIZE];
	void *result;
	pthread_t monitorThread;

	pthread_mutex_init(&mutex,NULL);
	sem_init(&sem, 0, MAXPAR);
	sem_init(&sem_sleep, 0, 0);
	
	CLEAR();

	if(argc > 1 || argv[1] != NULL)
		puts("\033[31m\nThis program doesn't need any args\033[m\n");

	numChildren=0;
	list = process_new();

	puts("\t\t\033[0;35mJC-SHELL V.1.2\033[0m\n");
	puts("Insert your commands:");

	/*Creates the monitor thread*/
	if(pthread_create(&monitorThread, NULL, monitor_Thread, NULL) == -1){
			puts("\033[31mError creating monitor thread\033[m");
			exit(THREAD_CREATE_FAILED);
	}

	while(1){
		/*Read the command*/
		numargs = readLineArguments(argvector, VECTORSIZE, buffer, BUFFERSIZE);

		if(feof(stdin) != 0) strcpy(argvector[0], "exit");

		else if(numargs < 1) continue;

		/*Examines the command*/
		switch(command(argvector[0])){
			case EXIT:
				/*Waits for the task monitors to finish before exit.*/

				/*Critical section*/
				pthread_mutex_lock(&mutex);
				Exit = 1;
				pthread_mutex_unlock(&mutex);
				pthread_join(monitorThread, &result);

				process_print(list);
				process_destroy(list);
				pthread_mutex_destroy(&mutex);
				fclose(stdin);
				exit(EXIT_SUCCESS);
				break;

			case INT_CLEAR:
				CLEAR();
				puts("\t\t\033[0;35mJC-SHELL V.1.2\033[0m\n");
				break;

			/*Creates a child process and execute the program*/
			case PATHNAME:
				sem_wait(&sem);

				pid=fork();

				if(pid == -1){
					fprintf(stderr, "%sChild process creation failed.%s\n",
					RED, NORM);
					break;
				}

				else if(pid == 0) execv(argvector[0], argvector);

				else{
					/*Critical section*/
					pthread_mutex_lock(&mutex);
					numChildren++;
					insert_new_process(list, pid, time(NULL));
					pthread_mutex_unlock(&mutex);
					sem_post(&sem_sleep);
				}

				break;

			default:
				puts("\033[0;31mInexisting file\033[m");
		}
	}
	return 0;
}
