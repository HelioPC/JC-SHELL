#include <sys/wait.h>
#include <time.h>
#include "list.h"
#include "ourheadfile.h"
#include <semaphore.h>


extern int Exit;
extern int numChildren;
extern FILE *regfile;
extern pthread_mutex_t mutex;
extern sem_t sem;
extern pthread_cond_t cond_var;
extern pthread_cond_t cond_var_2;
extern LIST_PROC *list;
extern REGARQ arq_info;

void* monitor_Thread(void *args){
	int status, await;

	if(args != NULL){
		printf("%sThis thread doesn't need any args. ", RED);
		printf("Unexpected behavior is expected%s", NORM);
	}

	/* this is the main loop */
	while(1){

		/*waits the process in execution to terminate*/
		pthread_mutex_lock(&mutex);
		if(numChildren > 0){ /* Critical section */
			pthread_mutex_unlock(&mutex);
			await = wait(&status);
			
			pthread_mutex_lock(&mutex); /* Critical section */
			update_terminated_process(list, await, WEXITSTATUS(status),
			time(NULL));
			numChildren--; /* End of critical section */
			pthread_mutex_unlock(&mutex);
			
			filewrite(await, gettime(list, await));
			pthread_cond_signal(&cond_var_2);
			continue;
		}
		pthread_mutex_unlock(&mutex);

		/*sleep for one second then continuos the cicle */
		pthread_mutex_lock(&mutex);
		if(Exit != 1){ /* Critical section */
			pthread_cond_wait(&cond_var, &mutex);
			pthread_mutex_unlock(&mutex);
			continue;
		}
		pthread_mutex_unlock(&mutex);
		
		/* terminates the thread when there is no more task to wait for */
		pthread_mutex_lock(&mutex);
		if(Exit == 1 && (numChildren) == 0){ /* Critical section*/
			pthread_mutex_unlock(&mutex);
			pthread_exit(NULL);
		}
		pthread_mutex_unlock(&mutex);
	}
}

void filewrite(pid_t pid, time_t tm){
	fseek(regfile, 0, SEEK_END);

	arq_info.totaltime += tm;

	fprintf(regfile, "iteration %d\npid: %d execution time: %ld s\ntotal ex"
	"ecution time: %ld s\n", ++arq_info.iternum, pid, tm, arq_info.totaltime);

	return;
}

int fileload(){
	int numlines = filelines(regfile);
	int i=0, j;
	char str[81];
	char *elem;

	while(i++ < numlines){
		fgets(str, 80, regfile);
		
		if(i == (numlines - 2)){
			elem = strtok(str, " ");
			elem = strtok(NULL, " ");

			if(!isnum(elem)) return 0;
			
			arq_info.iternum = atoi(elem);
			continue;
		}

		if(i == numlines){
			elem = strtok(str, " ");

			for(j = 0; j < 3; j++){
				elem = strtok(NULL, " ");
			}

			if(!isnum(elem)) return 0;

			arq_info.totaltime = (time_t) atoi(elem);
			break;
		}
	}

	return 1;
}

int command(char* cmd){ 
	FILE *fp;

	/* return EXIT command */
	if(strcmp(cmd, "exit") == 0) return EXIT;

	/* return INT_CLEAR command */
	else if(!strcmp(cmd, STR_CLEAR)) return INT_CLEAR;

	/* returns PATHNAME if command is an executable file */
	else if((fp=fopen(cmd, "rb")) != NULL){
		fclose(fp);
		return PATHNAME;
	}

	else return -1; /* invalid command */
}

int filelines(FILE *fp){
	char info[80];
	int cont = 0;

	rewind(fp);

	while((fgets(info, 79, fp) != NULL)) cont++;

	rewind(fp);

	return cont;
}

int isnum(char *str){
	int i;
	
	if(str == NULL) return 0;

	for(i = 0; (*(str+i) != '\0' && *(str+i) != '\n'); i++){
		if(!isdigit(*(str+i))) return 0;
	}

	return 1;
}
