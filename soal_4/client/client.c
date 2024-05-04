#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 8080

int main(int argc, char const* argv[]) {
  struct sockaddr_in address;
  int sock = 0, valread;
  struct sockaddr_in serv_addr;
  char* hello = "Hello from client";
  char buffer[1024] = { 0 };
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n Socket creation error \n");
    return -1;
    }

  memset(&serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    printf("\nInvalid address/ Address not supported \n");
    return -1;
    }

  if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
    printf("\nConnection Failed \n");
    return -1;
    }



  char cmd[100];

  while (1) {
    printf("\nYou: ");
    scanf("%[^\n]", cmd);
    getchar(); // Membersihkan karakter newline dari buffer stdin

    send(sock, cmd, strlen(cmd), 0);

    if (strcmp(cmd, "exit") == 0) {
      printf("Server:\n");
      printf("Exiting the client\n");
      break; // Keluar dari loop
      }
    else if (strcmp(cmd, "tampilkan") == 0) {
      printf("Server:\n");
      system("cat ../myanimelist.csv | awk '{print NR \". \" $0}'");
      }
    else if (strncmp(cmd, "hari ", 5) == 0) {
      char* hari = cmd + 5; // Mengambil string setelah "hari "
      char command[100];
      sprintf(command, "grep '^%s,' ../myanimelist.csv | awk -F, '{print NR \". \" $3}'", hari);
      printf("Server:\n");
      system(command);
      }
    else if (strncmp(cmd, "genre ", 6) == 0) {
      char* genre = cmd + 6; // Mengambil string setelah "genre "
      char command[100];
      sprintf(command, "grep ',%s,' ../myanimelist.csv | awk -F, '{print NR \". \" $3}'", genre);
      printf("Server:\n");
      system(command);
      }
    else if (strncmp(cmd, "add ", 4) == 0) {
      char* data = cmd + 4;
      char command[100];
      sprintf(command, "echo \"%s\" >> ../myanimelist.csv", data);
      printf("Server:\n");
      system(command);
      printf("Anime berhasil ditambahkan\n");
      }
    else if (strncmp(cmd, "status ", 7) == 0) {
      char* name = cmd + 7;
      char command[100];
      sprintf(command, "grep ',%s,' ../myanimelist.csv | awk -F, '{print $4}'", name);
      printf("Server:\n");
      system(command);
      }
    else if (strncmp(cmd, "edit ", 5) == 0) {
      char* input = cmd + 5;
      char command[250];

      // Pisahkan input berdasarkan koma
      char* anime = strtok(input, ",");
      char* hari = strtok(NULL, ",");
      char* genre = strtok(NULL, ",");
      char* judul = strtok(NULL, ",");
      char* status = strtok(NULL, ",");

      // Membangun perintah untuk mengedit file CSV menggunakan perintah awk
      sprintf(command, "awk -F, '{ if ($3 == \"%s\") { print \"%s,%s,%s,%s\" } else { print $0 } }' ../myanimelist.csv > temp.csv && mv temp.csv ../myanimelist.csv", anime, hari, genre, judul, status);

      printf("Server:\n");
      system(command);
      printf("Data telah diubah.\n");
      }
    else if (strncmp(cmd, "delete ", 7) == 0) {
      char* anime = cmd + 7;
      char command[200];

      // Membangun perintah untuk menghapus baris berdasarkan nama anime menggunakan awk
      sprintf(command, "awk -F, '$3 != \"%s\"' ../myanimelist.csv > temp.csv && mv temp.csv ../myanimelist.csv", anime);

      printf("Server:\n");
      system(command);
      printf("Data telah dihapus.\n");
      }
    else {
      printf("Invalid Command\n");
      }
    }

  return 0;

  }