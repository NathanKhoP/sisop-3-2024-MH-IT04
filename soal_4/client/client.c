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


    if (strlen(cmd) > 0) {
      send(sock, cmd, strlen(cmd), 0);

      // Membaca data dari server
      char full_output[8192] = { 0 }; // Menampung seluruh output dalam satu string
      valread = read(sock, full_output, sizeof(full_output));
      if (valread > 0) {
        printf("Server:\n%s\n", full_output); // Menampilkan seluruh output
        }

      if (strcmp(cmd, "exit") == 0) {
        break;
        }
      // Mereset buffer setelah pemrosesan perintah
      memset(buffer, 0, 1024);
      }

    }
  close(sock);
  return 0;
  }