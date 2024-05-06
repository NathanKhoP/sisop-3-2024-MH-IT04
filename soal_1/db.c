#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

#define MAX_FILENAME_LENGTH 256
#define SHM_SIZE 1024

int main() {
    key_t key = ftok(".", 'b');

    int shmid = shmget(key, SHM_SIZE, 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    char *shm = shmat(shmid, NULL, 0);
    if (shm == (char *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    
    char command[MAX_FILENAME_LENGTH];
    strcpy(command, shm);
    system(command);
    
    if (shmdt(shm) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    char type[MAX_FILENAME_LENGTH];
    char filename[MAX_FILENAME_LENGTH];
    sscanf(command, "%*[^[][%[^]]] [%[^]]]", type, filename);

    FILE *fp;
    fp = fopen("microservices/database/db.log", "a");
    if (fp == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    fprintf(fp, "[%02d/%02d/%02d %02d:%02d:%02d] [%s] [%s]\n",
            timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year % 100,
            timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec,
            type, filename);

    fclose(fp);

    return 0;
}


