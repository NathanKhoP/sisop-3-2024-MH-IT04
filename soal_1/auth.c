#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define MAX_FILENAME_LENGTH 256

int main() {
    DIR *dir;
    struct dirent *entry;
    dir = opendir("new-data");
    if (dir) {
        while ((entry = readdir(dir)) != NULL) {
            char *filename = entry->d_name;
            int len = strlen(filename);
            if (len >= 13 && strcmp(filename + len - 13, "bolobog_trashcan.csv") == 0) {
                char command[MAX_FILENAME_LENGTH];
                sprintf(command, "cp new-data/%s /dev/shm/%s", filename, filename);
                system(command);
            } else if (len >= 16 && strcmp(filename + len - 16, "osaka_parkinglot.csv") == 0) {
                char command[MAX_FILENAME_LENGTH];
                sprintf(command, "cp new-data/%s /dev/shm/%s", filename, filename);
                system(command);
            } else {
                char command[MAX_FILENAME_LENGTH];
                sprintf(command, "rm new-data/%s", filename);
                system(command);
            }
        }
        closedir(dir);
    }

    return 0;
}
