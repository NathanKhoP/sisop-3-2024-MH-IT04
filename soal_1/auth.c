#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MAX_FILENAME_LENGTH 256
#define SHM_SIZE 1024

int main() {
    DIR *dir;
    struct dirent *entry;
    dir = opendir("new-data");
    if (dir) {
        key_t key = ftok(".", 'a');
        int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
        if (shmid == -1) {
            perror("shmget");
            exit(EXIT_FAILURE);
        }
        char *shm = shmat(shmid, NULL, 0);
        if (shm == (char *)-1) {
            perror("shmat");
            exit(EXIT_FAILURE);
        }
        
        while ((entry = readdir(dir)) != NULL) {
            char *filename = entry->d_name;
            int len = strlen(filename);
            if (len >= 17 && strcmp(filename + len - 17, "_trashcan.csv") == 0) {
                char command[MAX_FILENAME_LENGTH];
                sprintf(command, "cp new-data/%s Documents/microservices/database/%s", filename, filename);
                strcpy(shm, command);
            } else if (len >= 20 && strcmp(filename + len - 20, "_parkinglot.csv") == 0) {
                char command[MAX_FILENAME_LENGTH];
                sprintf(command, "cp new-data/%s Documents/microservices/database/%s", filename, filename);
                strcpy(shm, command);
            } else {
                char command[MAX_FILENAME_LENGTH];
                sprintf(command, "rm new-data/%s", filename);
                strcpy(shm, command);
            }
        }
        
       
        if (shmdt(shm) == -1) {
            perror("shmdt");
            exit(EXIT_FAILURE);
        }

       
        if (shmctl(shmid, IPC_RMID, NULL) == -1) {
            perror("shmctl");
            exit(EXIT_FAILURE);
        }
        
        closedir(dir);
    }

    return 0;
}


