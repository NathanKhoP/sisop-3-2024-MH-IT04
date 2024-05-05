#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <dirent.h>
#include <signal.h>
#include <time.h>
#include <arpa/inet.h>

#include "actions.c"

#define PORT 8080
// #define DEBUG
#define MAX_LEN 1024

void log_func(char* who, char* cmd, char* arg) {
    // Time
    time_t T;
    struct tm tm;
    T = time(NULL);
    tm = *localtime(&T);

    FILE *log = fopen("/home/etern1ty/sisop_works/modul_3/soal_3/server/race.log", "a");
    if (log != NULL) {
        fprintf(log, "[%s] [%02d/%02d/%02d %02d:%02d:%02d]: [%s] [%s]\n", who, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, cmd, arg);
        fclose(log);
    }
}

int main() {
    // Creating RPC (Modul)
    int serv_socket, cli_socket;
    struct sockaddr_in serv_addr, cli_addr;
    int addrlen = sizeof(serv_addr);
    char buffer[MAX_LEN] = {0};
    
    // create socket
    if ((serv_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(serv_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // define server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    // bind socket to server address
    if (bind(serv_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // listen on port
    if (listen(serv_socket, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    // Making the program run as a daemon (Modul)
    pid_t pid, sid;
    pid = fork();
    if (pid < 0) {
        printf("Fork Failed!\n");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
    umask(0);
    if (setsid() < 0) {
        perror("Error: setsid() failed");
    }
    if ((chdir("/")) < 0) {
        exit(EXIT_FAILURE);
    }
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    char cmd[MAX_LEN], stat[MAX_LEN], *resp;
    int mark;

    while (1) {
        // accept incoming connection
        if ((cli_socket = accept(serv_socket, (struct sockaddr *)&serv_addr, (socklen_t *)&addrlen)) < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        #ifdef DEBUG
            printf("Connection established - [%s] on port [%d]\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
        #endif
        
        // clear buffer + read
        memset(buffer, 0, MAX_LEN);
        int cli_buf = read(cli_socket, buffer, MAX_LEN);
        if (cli_buf < 0) {
            perror("Read failed");
            break;
        } 
        else if (cli_buf == 0) {
            #ifdef DEBUG
                printf("Connection to client disconnected\n");
            #endif
            break;
        }

        #ifdef DEBUG
            printf("Data received: %s\n", buffer);
        #endif

        resp = NULL;
        mark = 1;
        
        // Handling args
        sscanf(buffer, "%s %s", cmd, stat);
        if (strcmp(cmd, "Gap") == 0) {
            float cur_gap = atof(stat); 
            resp = gap_func(cur_gap);
        }
        else if (strcmp(cmd, "Fuel") == 0) {
            float cur_fuel = atof(stat);
            resp = fuel_func(cur_fuel);
        }
        else if (strcmp(cmd, "Tire") == 0) {
            int cur_tire = atoi(stat);
            resp = tire_func(cur_tire);
        }
        else if (strcmp(cmd, "Tire_Change") == 0) {
            resp = tire_change(stat);
        }
        else {
            mark = 0;
        }

        log_func("Driver", cmd, stat);
        if (mark) {
            log_func("Paddock", cmd, resp);
        }

        if (resp != NULL && send(cli_socket, resp, strlen(resp), 0) < 0){
            perror("Send buffer failed");
        };
        
        close(cli_socket);
        sleep(5);
    }
}