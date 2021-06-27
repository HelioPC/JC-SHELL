#ifndef __TERM_H__
    #define __TERM_H__

    #include <fcntl.h>
    #include <unistd.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <signal.h>

    /* this TDA represents all the information needed to recieve Terminals id. */
    typedef struct regter{
        unsigned int id_terminal;
        struct regter *next;
    } REGTER;

    /* this TDA represents our linked list of 'REGTER' */
    typedef struct{
        REGTER *first;
    } LISTTERMS;

    /* thread responsible for registering all jcshell-terminal that open the 
    named pipe. */
    void* regTerminalThread();

    /* Sends a signal to a jcshell-terminal specified by string. */
    int exitterminal(char *);

    /* Sends a interrupt signal to all active jcshell-terminal. */
    int exitall(int);

    /* Insert the pid of a jchell-terminal on the linked list of terminals */
    int insert_new_terminal_id(LISTTERMS *, unsigned int);

    /* Creates a linked list of terminals. */
    LISTTERMS *listterm_init();

    /* Remove the pid of a jcshell-terminal from the linked list. */
    int remove_terminal_id(LISTTERMS *, unsigned int);

    /* Frees the space allocated to the linked list and its nodes. */
    void freelsterm(LISTTERMS *);

    /* Checks if the pid exists in the linked list of terminals, if so it
    returns 1, otherwise 0. */
    int isinlistterm(LISTTERMS *, unsigned int);

    /* Returns the number of nodes in the linked list of terminals. */
    int listtermtam(LISTTERMS *);

#endif
