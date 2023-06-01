#include <dirent.h>
#define __CONST__
#include "headers/ourheadfile.h"

int command(char* cmd){
	FILE *fp;

	/* return EXIT_GLOBAL command */
	if(!strcmp(cmd, CMD_EXIT_GLOBAL)) return EXIT_GLOBAL;

	/* return LS command */
	if(!strcmp(cmd, CMD_LS)) return LS;

	/* return INT_CLEAR command */
	else if(!strcmp(cmd, STR_CLEAR)) return INT_CLEAR;

	/* return stats command */
	else if(strstr(cmd, CMD_STATS) != NULL)
		return strlen(cmd) > 10 ? STATS : -1;

	/* returns PATHNAME if command is an executable file */
	else if((fp=fopen(cmd, "rb")) != NULL){
		fclose(fp);
		return PATHNAME;
	}
	
	/* return EXIT command */
	else if(strstr(cmd, CMD_EXIT) != NULL) return strlen(cmd) > 6 ? EXIT : -1;

	/* MORE COMMANDS SHOULD BE ADDED HERE. */

	else return -1; /* invalid command */
}

int isnum(char *str){
	int i;
	
	if(str == NULL) return 0;

	for(i = 0; (*(str+i) != '\0' && *(str+i) != '\n'); i++){
		if(!isdigit(*(str+i))) return 0;
	}

	return 1;
}

int strdel(char *str){
	int i;
	
	if(str == NULL) return 0;

	for(i = 0; *(str+i) != '\0'; i++){
		str[i] = '\0';
	}

	return 1;
}

int stats(int fd, int totaltm, int totalprocs){
	char str[BUFFERSIZE];

	sprintf(str, "\n%sTotal execution time%s: %ds%s\nNumber of active process"
	"es%s: %d\n\n", PURPLE, NORM, totaltm, PURPLE, NORM, totalprocs);
	
	return (int) write(fd, str, strlen(str));
}

void ls(){
	int cont = 0, i, j;
	char *argve[FILENAME_MAX];
	DIR *dir;
    struct dirent *lsdir;

    dir = opendir(".");

    if(dir == NULL) exit(EXIT_FAILURE);

    while((lsdir = readdir(dir)) != NULL && cont != FILENAME_MAX){
		argve[cont++] = lsdir->d_name;
	}

    putchar('\n');
	for(i=0; i < cont; i+=2){
		j = strlen(argve[i]) / 8;
        printf("%s%s", YELLOW_A, argve[i]);
        for(; j < 5; j++) putchar('\t');
        printf("%s%s%s\n", GREEN_A, (i+1) < cont ? argve[i+1] : "", NORM);
    }

    closedir(dir);
}
