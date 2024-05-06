#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define MAX_FILENAME_LENGTH 256

int main() {
    FILE *fp;
    char filename[MAX_FILENAME_LENGTH];
    time_t rawtime;
    struct tm *timeinfo;
    char fileType[MAX_FILENAME_LENGTH] = "Trash Can";
    char fileName[MAX_FILENAME_LENGTH] = "belobog_trashcan.csv";

    fp = fopen("db.log", "a");
    if (fp == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    fprintf(fp, "[%02d/%02d/%02d %02d:%02d:%02d] [%s] [%s]\n",
            timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year % 100,
            timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec,
            fileType, fileName);

    fclose(fp);

    return 0;
}
