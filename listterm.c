#include "headers/listterm.h"
#define __FILE_PIPE__
#include "headers/ourheadfile.h"

extern int errno;

extern int exit2;
extern LISTTERMS *lsterms;
extern pthread_mutex_t mutex2;
extern pthread_cond_t cond_var_3;

LISTTERMS *listterm_init(){
    LISTTERMS *ls = (LISTTERMS *) malloc(sizeof(LISTTERMS));

    if(ls != NULL) ls->first = NULL;

    return ls;
}

void *regTerminalThread(){
	int fd, pid, tam;
	char term_pid[20];
	
	if(access(NAMED_PIPE_REG, F_OK))
        if(mkfifo(NAMED_PIPE_REG, S_IRUSR | S_IWUSR) < 0){
            perror("\033[31mError opening the fifo");
            exit(EXIT_FAILURE);
        }
	
	if((fd = open(NAMED_PIPE_REG, O_RDONLY)) < 0){
		perror("\033[31mCouldn\'t open named pipe \"jchell-in\"\033[m");
		exit(errno);
	}

	while(1){
		if(read(fd, term_pid, 20) > 0){
            if((pid = atoi(term_pid)) < 2){
                errno = EINVAL;
                perror("\033[31mUnable to register terminal pid.");
                exit(EINVAL);
            }

            pthread_mutex_lock(&mutex2);
            if(!insert_new_terminal_id(lsterms, pid)){
                pthread_mutex_unlock(&mutex2);
                errno = EINVAL;
                perror("\033[31mUnable to register terminal pid.");
                exit(EINVAL);
            }

            tam = listtermtam(lsterms);
            printf("\n%s1 registered terminal. %d active terminal%s.%s\n",
            YELLOW, tam, tam > 1 ? "s" : "", NORM);

            pthread_mutex_unlock(&mutex2);

            pthread_cond_signal(&cond_var_3);
		}

        pthread_mutex_lock(&mutex2);
        if(exit2){
            pthread_mutex_unlock(&mutex2);
            pthread_exit(NULL);
        }
        pthread_mutex_unlock(&mutex2);
	}
}

int exitterminal(char *args){
	char *cmd = args;
	pid_t pid;

	pid = atoi((cmd + 5));

    if(!isinlistterm(lsterms, pid)) return 0;
	
	if(kill(pid, SIGTERM) < 0) return 0;

    if(!remove_terminal_id(lsterms, pid)) return 0;

	return 1;
}

int exitall(int sig){
    REGTER *ant, *aux = lsterms->first;

    while(aux != NULL){
        ant = aux;
		if(kill(aux->id_terminal, sig) < 0) return 0;
		aux = aux->next;
        remove_terminal_id(lsterms, ant->id_terminal);
	}

    return 1;
}

int insert_new_terminal_id(LISTTERMS *ls, unsigned int id){
    REGTER *termid;

    if(ls == NULL) return 0;

    termid = (REGTER *) malloc(sizeof(REGTER));

    if(termid == NULL) return 0;

    termid->id_terminal = id;
    termid->next = ls->first;
    ls->first = termid;

    return 1;
}

int remove_terminal_id(LISTTERMS *ls, unsigned int id){
    REGTER *aux, *ant;

    if(ls == NULL || ls->first == NULL) return 0;

    aux = ls->first;

    while(aux != NULL && aux->id_terminal != id){
        ant = aux;
        aux = aux->next;
    }

    if(aux == NULL) return 0;

    if(aux == ls->first) ls->first = aux->next;

    else ant->next = aux->next;

    free(aux);

    return 1;
}

void freelsterm(LISTTERMS *ls){
    REGTER *aux;

    if(ls != NULL){
        while(ls->first != NULL){
            aux = ls->first;
            ls->first = ls->first->next;
            free(aux);
        }
        free(ls);
    }
}

int isinlistterm(LISTTERMS *ls, unsigned int pid){
    REGTER *aux;

    if(ls == NULL || ls->first == NULL) return 0;

    aux = ls->first;

    while(aux != NULL){
        if(aux->id_terminal == pid) return 1;

        aux = aux->next;
    }

    return 0;
}

int listtermtam(LISTTERMS *ls){
    int count = 0;
    REGTER *aux;

    if(ls == NULL || ls->first == NULL) return count;

    aux = ls->first;
    
    while(aux != NULL){
        count++;
        aux = aux->next;
    }

    return count;
}
