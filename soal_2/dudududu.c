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

// Modes
volatile sig_atomic_t program_mode = 0;

// Inputted values
char input1[10];
char input2[10];
int num1, num2, result;

// Output
char strres[1024];

// Time declared
time_t T;
struct tm tm;

int strint(char* str) {
    if (strcmp(str, "satu") == 0) return 1;
    else if (strcmp(str, "dua") == 0) return 2;
    else if (strcmp(str, "tiga") == 0) return 3;
    else if (strcmp(str, "empat") == 0) return 4;
    else if (strcmp(str, "lima") == 0) return 5;
    else if (strcmp(str, "enam") == 0) return 6;
    else if (strcmp(str, "tujuh") == 0) return 7;
    else if (strcmp(str, "delapan") == 0) return 8;
    else if (strcmp(str, "sembilan") == 0) return 9;
    else exit(EXIT_FAILURE);
}

char* intstr(int num) { // there's quite literally no better way than this
    if (num == 0) return "nol" ;
    else if (num == 1) return "satu";
    else if (num == 2) return "dua";
    else if (num == 3) return "tiga";
    else if (num == 4) return "empat";
    else if (num == 5) return "lima";
    else if (num == 6) return "enam";
    else if (num == 7) return "tujuh";
    else if (num == 8) return "delapan";
    else if (num == 9) return "sembilan";
    else if (num == 10) return "sepuluh";
    else if (num == 11) return "sebelas";
    else if (num == 12) return "dua belas";
    else if (num == 13) return "tiga belas";
    else if (num == 14) return "empat belas";
    else if (num == 15) return "lima belas";
    else if (num == 16) return "enam belas";
    else if (num == 17) return "tujuh belas";
    else if (num == 18) return "delapan belas";
    else if (num == 19) return "sembilan belas";
    else if (num == 20) return "dua puluh";
    else if (num == 21) return "dua puluh satu";
    else if (num == 22) return "dua puluh dua";
    else if (num == 23) return "dua puluh tiga";
    else if (num == 24) return "dua puluh empat";
    else if (num == 25) return "dua puluh lima";
    else if (num == 26) return "dua puluh enam";
    else if (num == 27) return "dua puluh tujuh";
    else if (num == 28) return "dua puluh delapan";
    else if (num == 29) return "dua puluh sembilan";
    else if (num == 30) return "tiga puluh";
    else if (num == 31) return "tiga puluh satu";
    else if (num == 32) return "tiga puluh dua";
    else if (num == 33) return "tiga puluh tiga";
    else if (num == 34) return "tiga puluh empat";
    else if (num == 35) return "tiga puluh lima";
    else if (num == 36) return "tiga puluh enam";
    else if (num == 37) return "tiga puluh tujuh";
    else if (num == 38) return "tiga puluh delapan";
    else if (num == 39) return "tiga puluh sembilan";
    else if (num == 40) return "empat puluh";
    else if (num == 41) return "empat puluh satu";
    else if (num == 42) return "empat puluh dua";
    else if (num == 43) return "empat puluh tiga";
    else if (num == 44) return "empat puluh empat";
    else if (num == 45) return "empat puluh lima";
    else if (num == 46) return "empat puluh enam";
    else if (num == 47) return "empat puluh tujuh";
    else if (num == 48) return "empat puluh delapan";
    else if (num == 49) return "empat puluh sembilan";
    else if (num == 50) return "lima puluh";
    else if (num == 51) return "lima puluh satu";
    else if (num == 52) return "lima puluh dua";
    else if (num == 53) return "lima puluh tiga";
    else if (num == 54) return "lima puluh empat";
    else if (num == 55) return "lima puluh lima";
    else if (num == 56) return "lima puluh enam";
    else if (num == 57) return "lima puluh tujuh";
    else if (num == 58) return "lima puluh delapan";
    else if (num == 59) return "lima puluh sembilan";
    else if (num == 60) return "enam puluh";
    else if (num == 61) return "enam puluh satu";
    else if (num == 62) return "enam puluh dua";
    else if (num == 63) return "enam puluh tiga";
    else if (num == 64) return "enam puluh empat";
    else if (num == 65) return "enam puluh lima";
    else if (num == 66) return "enam puluh enam";
    else if (num == 67) return "enam puluh tujuh";
    else if (num == 68) return "enam puluh delapan";
    else if (num == 69) return "enam puluh sembilan";
    else if (num == 70) return "tujuh puluh";
    else if (num == 71) return "tujuh puluh satu";
    else if (num == 72) return "tujuh puluh dua";
    else if (num == 73) return "tujuh puluh tiga";
    else if (num == 74) return "tujuh puluh empat";
    else if (num == 75) return "tujuh puluh lima";
    else if (num == 76) return "tujuh puluh enam";
    else if (num == 77) return "tujuh puluh tujuh";
    else if (num == 78) return "tujuh puluh delapan";
    else if (num == 79) return "tujuh puluh sembilan";
    else if (num == 80) return "delapan puluh";
    else if (num == 81) return "delapan puluh satu";
    else return "ERROR";
}

void swap_nums() {
    int temp = num1;
    num1 = num2;
    num2 = temp;
}

int opr_func() {
    switch (program_mode) {
        case 0:
            return result = num1 * num2;
        case 1:
            return result = num1 + num2;
        case 2:
            // if (num2 > num1) {
            //     swap_nums(&num1, &num2);
            //     *swapped = 1;
            // }
            return result = num1 - num2;
        case 3:
            // if (num2 > num1) {
            //     swap_nums(&num1, &num2);
            //     *swapped = 1;
            // }
            return result = num1 / num2;
    }
}

char* output_type (char **argv) {
    if (strcmp(argv[1], "-kali") == 0) return "perkalian";
    else if (strcmp(argv[1], "-tambah") == 0) return "penjumlahan";
    else if (strcmp(argv[1], "-kurang") == 0) return "pengurangan";
    else if (strcmp(argv[1], "-bagi") == 0) return "pembagian";
    else exit(EXIT_FAILURE);
}

char* log_out (char **argv) {
    if (strcmp(argv[1], "-kali") == 0) return "KALI";
    else if (strcmp(argv[1], "-tambah") == 0) return "TAMBAH";
    else if (strcmp(argv[1], "-kurang") == 0) return "KURANG";
    else if (strcmp(argv[1], "-bagi") == 0) return "BAGI";
    else exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    // Init time for logging 
    T = time(NULL);
    tm = *localtime(&T);

    // Handling args
    if (argc == 1) {
        return 0;
    }
    else if (argc > 1) {
        if (strcmp(argv[1], "-kali") == 0) {
            program_mode = 0;
        } 
        else if (strcmp(argv[1], "-tambah") == 0) {
            program_mode = 1;
        }
        else if (strcmp(argv[1], "-kurang") == 0) {
            program_mode = 2;
        }
        else if (strcmp(argv[1], "-bagi") == 0) {
            program_mode = 3;
        }
    }

    int fd1[2]; // Used to store two ends of first pipe 
	int fd2[2]; // Used to store two ends of second pipe 

	pid_t p; 

	if (pipe(fd1)==-1) {
		fprintf(stderr, "Pipe Failed" ); 
        perror("Pipe 1 creation failed");
		return 1; 
	} 
	if (pipe(fd2)==-1) { 
		fprintf(stderr, "Pipe Failed" ); 
        perror("Pipe 2 creation failed");
		return 1; 
	} 

	scanf("%s%s", input1, input2);
    while((getchar()) != '\n');

	p = fork(); 
	if (p < 0) { 
		fprintf(stderr, "fork Failed" ); 
        perror("fork failed");
		return 1; 
	} 

	// Parent process 
	else if (p > 0) {
        // int swapped = 0;
        num1 = strint(input1);
        num2 = strint(input2);
        result = opr_func();

        close(fd1[0]);
        write(fd1[1], &result, sizeof(result));
        // write(fd1[1], &swapped, sizeof(swapped));
        close(fd1[1]);

        wait(NULL);

        close(fd2[1]);
        read(fd2[0], strres, sizeof(strres));

        FILE *log = fopen("history.log", "a");
        if (log != NULL) {
            if (strcmp(strres, "ERROR") == 0) {
                fprintf(log, "[%02d/%02d/%02d %02d:%02d:%02d] [%s] ERROR pada %s\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, log_out(argv), output_type(argv));
            }
            else {
                if (program_mode == 0) fprintf(log, "[%02d/%02d/%02d %02d:%02d:%02d] [%s] %s kali %s sama dengan %s\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, log_out(argv), input1, input2, strres);
                else if (program_mode == 1) fprintf(log, "[%02d/%02d/%02d %02d:%02d:%02d] [%s] %s tambah %s sama dengan %s\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, log_out(argv), input1, input2, strres);
                else if (program_mode == 2) fprintf(log, "[%02d/%02d/%02d %02d:%02d:%02d] [%s] %s kurang %s sama dengan %s\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, log_out(argv), input1, input2, strres);
                else if (program_mode == 3) fprintf(log, "[%02d/%02d/%02d %02d:%02d:%02d] [%s] %s bagi %s sama dengan %s\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, log_out(argv), input1, input2, strres);
            }
            fclose(log);
        }
        else {
            perror("Failed to open log file");
            exit(EXIT_FAILURE);
        }
	} 

	// Child process 
	else { 
        // int swapped;
        close(fd1[1]);
        read(fd1[0], &result, sizeof(result));
        // read(fd1[0], &swapped, sizeof(swapped));
        close(fd1[0]);

        strcpy(strres, intstr(result));
        // if (swapped == 1) {
        //     char temp[10];
        //     strcpy(temp, input1);
        //     strcpy(input1, input2);
        //     strcpy(input2, temp);
        // }

        printf("hasil %s %s dan %s adalah %s\n", output_type(argv), input1, input2, strres);

        close(fd2[0]);
        write(fd2[1], strres, strlen(strres)+1);
        close(fd2[1]);
        exit(0);
	} 
    return 0;
}