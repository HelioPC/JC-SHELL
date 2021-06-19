#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "commandlinereader.h"
#include "ourheadfile.h"

int main(int argc, char **argv){
    int fd, fd_aux;
    char buffer[BUFFERSIZE];

    if(argc != 2){
        perror("\033[31mjcshell-terminal only takes 2 args");
        exit(EINVAL);
    }

    if(strcmp(argv[1], NAMED_PIPE) != 0){
        perror("\033[31mArg 2 must be named pipe \"jcshell-in\"");
        exit(EINVAL);
    }

    CLEAR();

    printf("\t\t%sJC-SHELL %s%s\n", PURPLE, VERSION, NORM);
    
    if(access(NAMED_PIPE, F_OK) == -1){
        perror("\033[31mNamed pipe \"jcshell-in\" not found");
        exit(ENOENT);
    }

    if((fd = open(NAMED_PIPE, O_WRONLY, S_IWUSR)) < 0){
        perror("\033[31mCouldn\'t open named pipe \"jchell-in\"");
        exit(OPEN_FILE_FAILED);
    }

    if(access(NAMED_PIPE_REG, F_OK) == -1){
        perror("\033[31mNamed pipe \"jcshell-in\" not found");
        exit(ENOENT);
    }

    if((fd_aux = open(NAMED_PIPE_REG, O_WRONLY, S_IWUSR)) < 0){
        perror("\033[31mCouldn\'t open named pipe \"jchell-in\"");
        exit(OPEN_FILE_FAILED);
    }

    sprintf(buffer, "%d", (int) getpid());

    write(fd_aux, buffer, strlen(buffer));

    close(fd_aux);

    puts("Insert your commands:");

    while(1){
		/* Read the command */
        if(fgets(buffer, BUFFERSIZE, stdin) == NULL) continue;

        if(feof(stdin) != 0) strcpy(buffer, "exit");

        if(strcmp(buffer, "exit\n") == 0){
            sprintf(buffer, "exit-%d\n", (int) getpid());
            write(fd, buffer, strlen(buffer));
        }
        
        else write(fd, buffer, strlen(buffer));
    }
}
