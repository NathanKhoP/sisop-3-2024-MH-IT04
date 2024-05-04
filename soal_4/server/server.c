#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#define PORT 8080

void log_command(const char* action, const char* cmd) {
  // Buka file log
  FILE* log_file = fopen("change.log", "a");
  if (log_file == NULL) {
    perror("Failed to open log file");
    return;
    }

  // Mendapatkan waktu saat ini
  time_t now;
  struct tm* local_time;
  char timestamp[30];
  time(&now);
  local_time = localtime(&now);
  strftime(timestamp, sizeof(timestamp), "%d/%m/%y", local_time);

  // Menulis log ke file
  if (strcmp(action, "add") == 0) {
    fprintf(log_file, "[%s][ADD] %s\n", timestamp, cmd);
    }
  if (strcmp(action, "edit") == 0) {
    fprintf(log_file, "[%s][EDIT] %s\n", timestamp, cmd);
    }
  if (strcmp(action, "delete") == 0) {
    fprintf(log_file, "[%s][DEL] %s\n", timestamp, cmd);
    }`

    // Menutup file log
      fclose(log_file);
  }
int main(int argc, char const* argv[]) {
  int server_fd, new_socket, valread;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  char buffer[1024] = { 0 };
  char* hello = "Hello from server";

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
    }

  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
    }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
    }

  if (listen(server_fd, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
    }

  if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
    perror("accept");
    exit(EXIT_FAILURE);
    }

  while (1) {
    valread = read(new_socket, buffer, 1024);
    buffer[valread] = '\0'; // Menambahkan null-terminator ke akhir string yang dibaca

    printf("Received: %s\n\n", buffer); // Memisahkan string buffer menjadi action dan cmd

    char action[50];
    char cmd[1024];
    sscanf(buffer, "%s %[^\n]", action, cmd);

    // Memasukkan action dan cmd ke dalam log
    log_command(action, cmd);

    if (strcmp(buffer, "exit") == 0) {
      break;
      }

    }
  return 0;
  }