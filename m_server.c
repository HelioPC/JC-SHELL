#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 4444

typedef struct
{
    int clients[1000];
    int qtd;
} LISTA;

LISTA *lcl, *times;

pthread_t monitorThread[1000], handler_thread;

pthread_mutex_t mutex;

pthread_cond_t var_cond;

int sockfd, ret, total_time = 0;

struct sockaddr_in serverAddr;

int newSocket;
struct sockaddr_in newAddr;

socklen_t addr_size;

char buffer[1024];
pid_t childpid;

void calculate()
{
    int count = 0;

    for (size_t i = 0; i < times->qtd; i++)
    {
        count += times->clients[i];
    }
    total_time = count;
}

void *acceptConections(void *args);
void *handle_time(void *args);

int main()
{
    int client_sock;
    int counter = 0;
    void *result;

    lcl = (LISTA *)malloc(sizeof(LISTA));
    times = (LISTA *)malloc(sizeof(LISTA));

    if (lcl == NULL || times == NULL)
    {
        printf("Falha ao criar lista\n");
        exit(1);
    }
    lcl->qtd = 0;
    times->qtd = 0;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&var_cond, NULL);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        printf("[-]Error in connection.\n");
        exit(1);
    }
    printf("[+]Server Socket is created.\n");

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    ret = bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (ret < 0)
    {
        printf("[-]Error in binding.\n");
        exit(1);
    }
    printf("[+]Bind to port %d\n", 4444);

    if (listen(sockfd, 10) == 0)
    {
        printf("[+]Listening....\n");
    }
    else
    {
        printf("[-]Error in binding.\n");
    }

    if (pthread_create(&handler_thread, NULL, handle_time, NULL) < 0)
    {
        puts("\033[31mError creating monitor thread\033[m");
        exit(1);
    }

    while ((client_sock = accept(sockfd, (struct sockaddr *)&newAddr, &addr_size)) > 0)
    {
        printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
        if (pthread_create(&monitorThread[counter++], NULL, acceptConections, (void *)&client_sock) < 0)
        {
            puts("\033[31mError creating monitor thread\033[m");
            exit(1);
        }
        pthread_mutex_lock(&mutex);
        lcl->clients[lcl->qtd] = client_sock;
        lcl->qtd++;
        pthread_mutex_unlock(&mutex);
    }

    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }

    for (size_t i = 0; i < counter; i++)
    {
        pthread_join(monitorThread[i], &result);
    }

    close(client_sock);

    return 0;
}

void *acceptConections(void *args)
{
    int sock = *(int *)args;
    int my_time;
    while (1)
    {
        recv(sock, buffer, 1024, 0);
        if (strcmp(buffer, ":exit") == 0)
        {
            printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
            break;
        }
        else
        {
            printf("Tempo do nó: %s UTC\n", buffer);
            my_time = atoi(buffer);
            pthread_mutex_lock(&mutex);
            times->clients[times->qtd] = my_time;
            times->qtd++;
            printf("sockets: %d, times: %d\n", lcl->qtd, times->qtd);
            pthread_mutex_unlock(&mutex);
            pthread_cond_signal(&var_cond);
        }
    }
    pthread_exit(NULL);
}

void *handle_time(void *args)
{
    pthread_mutex_lock(&mutex);

    while (!(lcl->qtd == times->qtd && times->qtd >= 2))
    {
        printf("Em espera...\n");
        pthread_cond_wait(&var_cond, &mutex);
    }

    printf("sockets: %d, times: %d\n", lcl->qtd, times->qtd);

    if (lcl->qtd == times->qtd && times->qtd >= 2)
    {
        calculate();
        char msg[180];
        sprintf(msg, "Tempo corrente para todos os nós: %d UTC", total_time);

        for (size_t i = 0; i < lcl->qtd; i++)
        {
            if (send(lcl->clients[i], msg, strlen(msg), 0) < 1)
            {
                printf("erro ao enviar: socket desligado");
                continue;
            }
        }
        printf("Tempo corrente para todos os nós: %d UTC", total_time);
    }
    total_time = 0;
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}
