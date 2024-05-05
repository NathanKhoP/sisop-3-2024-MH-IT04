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

#define PORT 8080
#define IP "127.0.0.1"
#define MAX_LEN 1024
#define DEBUG

int main(int argc, char *argv[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    int num1 = 10, num2 = 20, result;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, IP, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    char buf[MAX_LEN], resp[MAX_LEN];

    char space = ' ';
    char arg1[MAX_LEN];

    if (argc < 5) {
        printf("Usage: %s [-cmd <arg>] [-stat <arg>]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    else if ((strcmp(argv[1], "-cmd") == 0 && strcmp(argv[3], "-stat") == 0) || (strcmp(argv[1], "-stat") == 0 && strcmp(argv[3], "-cmd") == 0)) {
        if (strcmp(argv[1], "-cmd") == 0) {
            strcpy(arg1, argv[2]);
            strncat(arg1, &space, 1);
            strcpy(buf, strcat(arg1, argv[4]));
        } 
        else {
            strcpy(arg1, argv[4]);
            strncat(arg1, &space, 1);
            strcpy(buf, strcat(arg1, argv[2]));
        }
        // strcpy(buf, strcat(arg1, argv[4]));

        printf("Driver  : [%s]\n", buf);
        send(sock, buf, strlen(buf), 0);
        memset(resp, 0, MAX_LEN);

        int ser_buf = read(sock, resp, MAX_LEN);
        resp[ser_buf] = '\0';
        printf("Paddock : [%s]\n", resp);
    }
    return 0;
}