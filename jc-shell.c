/*
*****************************************************************************
	OPERATING SYSTEMS PROJECT (COMPUTER SCIENCE - 2nd YEAR)
*****************************************************************************
		PROJECT:				JCShell
		NAME:               	JCShell
       	DEVELOPED BY:			GROUP Nº02 (DEI-CC/FC/UAN 2020)
       	WRITTEN IN:         	C Language
       	FOR:               		UNIX
       	STABLE VERSION:     	V.1.5.7
       	TEACHER:            	Msc. JOÃO COSTA
       	YEAR:                	20/04/2021 - 06/07/2021
*****************************************************************************
       AGOSTINHO NETO UNIVERSITY
       FACULTY OF SCIENCES
       DEPARTMENT OF COMPUTER SCIENCES

		GROUP Nº02:
				- ELIÚDE PATRÍCIO DE CARVALHO VEMBA - (ID Github) HelioPC
				- LUDMILO HUEBA CAMBAMBI - (ID Github) Ludmilo-cambambi
				- PEDRO MANUEL DOMINGOS - (ID Github) pedro7-7-7
				- LUCÍLIO TÉRCIO GOMES - (ID Github) luciliogomez
*****************************************************************************
*/

#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>
#include "headers/commandlinereader.h"
#define __CONST__
#define __FILE_PIPE__
#define __MNT_THREAD__
#define __FILE_FUNCS__
#include "headers/ourheadfile.h"
#include <fcntl.h>
#define __USE_POSIX 1
#define __USE_XOPEN_EXTENDED
#include <signal.h>
#include "headers/list.h"
#define __REG_THREAD__
#include "headers/listterm.h"

#define MAXPAR 4

extern int errno;

void sighandler(int);

int exit1; /* Termination order for the monitor thread. */
int numChildren;
int startread; /* Reading order on named pipe. */
int exit2; /* Termination order for the terminal thread. */
FILE *regfile; /* log.txt */

/* Ensures mutual exclusion between the monitoring thread and the main one. */
pthread_mutex_t mutex;

/* Ensures mutual exclusion between the terminals thread and the main one. */
pthread_mutex_t mutex2;

/* Used to fall asleep/wake up the monitor thread. */
pthread_cond_t cond_var;

/* Used to limit the creation of child processes. */
pthread_cond_t cond_var_2;

/* Used to wait for at least one jcshell-terminal to open. */
pthread_cond_t cond_var_3;

/* Linked list of processes. */
LIST_PROC *list;

/* TDA representing the necessary information from the "log.txt" file. */
REGARQ arq_info;

/* Linked list of jcshell-terminal(s). */
LISTTERMS *lsterms;

struct sigaction sact, old;

int main(int argc, char **argv){
	int numargs, numlines, fdin, fdproc, fdstats;
	char buffer[BUFFERSIZE];
	char *argvector[VECTORSIZE];
	char filename[FILENAMESIZE];
	void *result, *result2;
	pid_t pid;
	pthread_t monitorThread; /* Monitor thread ID. */
	pthread_t terminalThread; /* Terminal thread ID. */

	errno = 0;

	sact.sa_handler = sighandler; /* Function that handles the signal. */
	sigemptyset(&sact.sa_mask);
	sact.sa_flags = 0;
	
	sigaction(SIGINT, &sact, NULL);

	/* Create a directory to store temporary files.
	First check for existence.*/
	if(access(TMP_DIR, F_OK))
		if(mkdir(TMP_DIR, S_IRWXU | S_IRWXG | S_IRWXO)){
			perror("\033[31mCould not create default temporary directory.");
			exit(errno);
		}

	regfile = fopen(OUTPUT_TXT, READ_AND_APPEND);

	/* Initializations of mutex and condition variables. */
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&mutex2, NULL);
	pthread_cond_init(&cond_var, NULL);
	pthread_cond_init(&cond_var_2, NULL);
	pthread_cond_init(&cond_var_3, NULL);

	/* If the named pipe exists, ends with error. */
	if(!access(NAMED_PIPE, F_OK)){
		errno = EEXIST;
		perror("\033[31mNamed pipe \"jcshell-in\" not erased\033[m");
		exit(errno);
	}

	/* Creates the named pipe. */
	if(mkfifo(NAMED_PIPE, S_IRUSR | S_IWUSR) < 0){
		perror("\033[31mError creating FIFO.");
		exit(errno);
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

	/* Loading "log.txt" file information. */
	if(numlines != 0 && numlines % 3 == 0 && fileload());

	else{
		fclose(regfile);
		regfile = fopen(OUTPUT_TXT, READ_AND_WRITE);
		arq_info.iternum = -1;
		arq_info.totaltime = 0;
	}

	/* Creates the monitor thread */
	if(pthread_create(&monitorThread, NULL, monitor_Thread, NULL) < 0){
			puts("\033[31mError creating monitor thread\033[m");
			exit(errno);
	}

	/* Creates the terminal thread */
	if(pthread_create(&terminalThread, NULL, regTerminalThread, NULL) < 0){
			puts("\033[31mError creating terminals handling thread\033[m");
			exit(errno);
	}

	/* Opens the named pipe. */
	if((fdin = open(NAMED_PIPE, O_RDONLY | O_NONBLOCK, S_IRUSR)) < 0){
		perror("\033[31mCouldn\'t open named pipe \"jcshell-in\"\033[m");
		exit(errno);
	}

	/* Redirects standard input (keyboard) to the named pipe. */
	dup2(fdin, STDIN_FILENO);

	while(1){
		/*Read the command*/
		fflush(stdin);

		A:
		pthread_mutex_lock(&mutex2);
		while(lsterms->first == NULL){
			pthread_cond_wait(&cond_var_3, &mutex2);
		} /* Wait for the signal from the terminal thread. */
		pthread_mutex_unlock(&mutex2);

		numargs = readLineArguments(argvector, VECTORSIZE, buffer, BUFFERSIZE);

		if(startread){
			startread = 0;
			goto A;
		}

		if(numargs < 1) continue;

		/* Examines the command */
		switch(command(argvector[0])){
			case LS:
				CLEAR();
				printf("\t\t%sJC-SHELL %s%s\n", PURPLE, VERSION, NORM);
				ls();
				break;

			case STATS:
				if((fdstats = open(buffer, O_WRONLY, S_IRUSR | S_IWUSR)) < 0){
					perror("\033[31mCouldn\'t open stats pipe\033[m");
					exit(errno);
				}

				pthread_mutex_lock(&mutex);
				stats(fdstats, arq_info.totaltime, unfinished(list));
				pthread_mutex_unlock(&mutex);

				close(fdstats);

				break;

			case EXIT_GLOBAL:
				/* General termination. */

				/* Closes all jcshell-terminal. */
				if(exitall(SIGTERM)){
					printf("\n%sSuccess in terminating all active jcshell-ter"
					"minals.%s\n", YELLOW, NORM);
				}
				
				pthread_mutex_lock(&mutex);
				exit1 = 1; /* Critical section */
				pthread_mutex_unlock(&mutex);

				pthread_cond_signal(&cond_var);

				pthread_mutex_lock(&mutex2);
				exit2 = 1; /* Critical section */
				pthread_mutex_unlock(&mutex2);

				/* Wait for all threads to finish, before exit. */
				pthread_join(monitorThread, &result);
				pthread_join(terminalThread, &result2);

				/* Prints the process list. */
				process_print(list);

				/* Frees all allocated resources, closes all open files and de
				letes all created files. */
				freelsterm(lsterms);
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

				/* Successfully terminates jcshell. */
				exit(EXIT_SUCCESS);
				break;

			case EXIT:
				/* Sends a termination signal to jcshell-terminal. */
				if(exitterminal(buffer)){
					puts("\n\033[32mSuccess sending SIGTERM to jcshell-termina"
					"l.\033[m");
				}else{
					puts("\n\033[31mFailure to send SIGTERM to jcshell-termina"
					"l.\033[m");
				}

				break;

			case INT_CLEAR:
				/* Clean the screen. */
				CLEAR();
				printf("\t\t%sJC-SHELL %s%s\n", PURPLE, VERSION, NORM);
				break;

			case PATHNAME:
				/* Creates a child process and execute the program */
				pthread_mutex_lock(&mutex);
				while(numChildren == MAXPAR) pthread_cond_wait(&cond_var_2, &mutex);
				pthread_mutex_unlock(&mutex);

				pid = fork();

				if((int) pid == -1){
					/* Failed to create a child process. */
					fprintf(stderr, "%sChild process creation failed.%s\n",
					RED, NORM);
					break;
				}

				else if((int) pid == 0) {
					/* Child process field. */
					sprintf(filename, "jcshell-out-%d.txt", (int) getpid());

					if((fdproc = open(filename, O_CREAT | O_WRONLY | O_TRUNC,
					S_IRUSR | S_IWUSR)) == -1) exit(errno);

					dup2(fdproc, STDERR_FILENO);
					dup2(fdproc, STDOUT_FILENO);

					execv(argvector[0], argvector);

					close(fdproc);
				}

				else{
					/* Parent process field. */
					pthread_mutex_lock(&mutex);
					numChildren++; /* Critical section */
					insert_new_process(list, pid, time(NULL));
					/*End of critical section*/
					pthread_mutex_unlock(&mutex);
					pthread_cond_signal(&cond_var);
				}

				break;

			default:
				/* None of the above commands. */
				puts("\033[0;31mInexisting file\033[m");
		}
	}
	
	return 0; /* The program never gets here. */
}

void sighandler(int sig){
	startread = 1;
	if(exitall(sig)){
		puts("\n\033[33mSuccess in terminating all active jcshell-terminal ab"
		"ruptly\033[m");
	}
}
