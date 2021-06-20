#include <stdio.h>
#include <fcntl.h>
#define __USE_POSIX 1
#define __USE_XOPEN_EXTENDED
#include <signal.h>
#include <sys/stat.h>
#include "commandlinereader.h"
#include "ourheadfile.h"

void sighandler(int);

int exit_, fd;
char stats_pipe[30];
struct sigaction sact;

int main(int argc, char **argv){
    int fd_aux, fdin;
    char buffer[BUFFERSIZE];

    if(argc != 2){
        perror("\033[31mjcshell-terminal only takes 1 args:\033[m ./jcshell-"
        "terminal \'PIPE TO JCSHELL\'");
        exit(EINVAL);
    }

    if(strcmp(argv[1], NAMED_PIPE) != 0){
        perror("\033[31mArgument must be named pipe \"jcshell-in\"");
        exit(EINVAL);
    }

    CLEAR();

    printf("\t\t%sJCSHELL-TERMINAL %s%s\n", PURPLE, VERSION_TERM, NORM);
    
    if(access(NAMED_PIPE, F_OK) == -1){
        perror("\033[31mNamed pipe \"jcshell-in\" not found");
        exit(ENOENT);
    }

    if((fd = open(NAMED_PIPE, O_WRONLY, S_IWUSR)) < 0){
        perror("\033[31mCouldn\'t open named pipe \"jcshell-in\"");
        exit(OPEN_FILE_FAILED);
    }

    if(access(NAMED_PIPE_REG, F_OK) == -1){
        perror("\033[31mNamed pipe \"jcshell-in\" not found");
        exit(ENOENT);
    }

    if((fd_aux = open(NAMED_PIPE_REG, O_WRONLY, S_IWUSR)) < 0){
        perror("\033[31mCouldn\'t open named pipe \"jcshell-in\"");
        exit(OPEN_FILE_FAILED);
    }

    exit_ = 0;

    sact.sa_handler = sighandler;
	sigemptyset(&sact.sa_mask);
	sact.sa_flags = 0;
	
	sigaction(SIGTERM, &sact, NULL);

    sprintf(buffer, "%d", (int) getpid());

    write(fd_aux, buffer, strlen(buffer));

    close(fd_aux);

    strcpy(stats_pipe, NAMED_PIPE_AUX);
    strcat(stats_pipe, buffer);

    if(mkfifo(stats_pipe, S_IRWXU | S_IRWXG | S_IRWXO) < 0){
		perror("\033[31mError creating auxiliary FIFO.");
		exit(ENOENT);
	}

    while(1){
        
        if(!exit_) printf("%s$%s ", BLUE, GREEN);
		/* Read the command */
        if(fgets(buffer, BUFFERSIZE, stdin) == NULL) continue;

        if(feof(stdin) != 0) strcpy(buffer, "exit");

        if(strstr(buffer, "exit")) exit_ = 1;

        if(strcmp(buffer, "exit\n") == 0){
            sprintf(buffer, "exit-%d\n", (int) getpid());
            write(fd, buffer, strlen(buffer));
        }

        else if(!strcmp(buffer, "stats\n")){
            write(fd, stats_pipe, strlen(stats_pipe));

            if((fdin = open(stats_pipe, O_RDONLY, S_IRUSR | S_IWUSR)) < 0){
                perror("\033[31mCouldn\'t open stats pipe");
                exit(OPEN_FILE_FAILED);
            }

            strdel(buffer);

            while(read(fdin, buffer, 25) > 0) printf("%s", buffer);

            close(fdin);
        }
        
        else write(fd, buffer, strlen(buffer));
    }
}

void sighandler(int sig){
    exit_ = sig;
    unlink(stats_pipe);
    close(fd);
    exit(EXIT_SUCCESS);
}
