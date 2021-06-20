#include "ourheadfile.h"

int command(char* cmd){ 
	FILE *fp;

	/* return EXIT_GLOBAL command */
	if(!strcmp(cmd, CMD_EXIT_GLOBAL)) return EXIT_GLOBAL;

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
