#include <sys/wait.h>
#include <time.h>
#include "headers/list.h"
#define __CONST__
#define __FILE_FUNCS__
#include "headers/ourheadfile.h"
#include <semaphore.h>

extern FILE *regfile;
extern REGARQ arq_info;

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
	char str[BUFFERSIZE];
	char *elem;

	while(i++ < numlines){
		fgets(str, BUFFERSIZE, regfile);
		
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

int filelines(FILE *fp){
	char info[BUFFERSIZE];
	int cont = 0;

	rewind(fp);

	while((fgets(info, BUFFERSIZE, fp) != NULL)) cont++;

	rewind(fp);

	return cont;
}
