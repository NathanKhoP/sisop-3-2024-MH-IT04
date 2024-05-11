#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 1024 
#define PORT 8080

char* get_new_anime(const char* buffer) {
  char* anime_info;
  char* token;
  char buffer_copy[strlen(buffer) + 1]; // Salin string buffer ke buffer_copy

  // Salin string buffer ke buffer_copy
  strcpy(buffer_copy, buffer);

  // Memulai pemisahan string menggunakan strtok
  token = strtok(buffer_copy, ",");
  // Melakukan pemisahan string sampai menemukan token kedua
  for (int i = 0; i < 1; i++) {
    token = strtok(NULL, ",");
    }

  // Mengalokasikan memori untuk string anime_info
  anime_info = (char*)malloc(strlen(token) + 1);
  if (anime_info == NULL) {
    printf("Memory allocation failed\n");
    return NULL;
    }

  // Menyalin token kedua dan token-token berikutnya ke dalam string anime_info
  strcpy(anime_info, token);
  while ((token = strtok(NULL, ",")) != NULL) {
    strcat(anime_info, ",");
    strcat(anime_info, token);
    }

  // Mengembalikan string anime_info
  return anime_info;
  }


char* get_anime_info(const char* anime_name, const char* csv_file, int sock) {
  char command[BUFFER_SIZE];
  char result[BUFFER_SIZE];

  // Format perintah awk untuk mencari nama anime tertentu dalam file CSV
  snprintf(command, BUFFER_SIZE, "awk -F, '$3 == \"%s\" { print }' %s", anime_name, csv_file);

  // Menjalankan perintah awk dan menangkap outputnya
  FILE* fp = popen(command, "r");
  if (fp == NULL) {
    perror("popen failed");
    exit(EXIT_FAILURE);
    }
  // Membaca output dari perintah awk dan menyimpannya dalam variabel result
  if (fgets(result, sizeof(result), fp) == NULL) {
    char* invalid = "Invalid Command\n";
    send(sock, invalid, strlen(invalid), 0);
    }

  // Menutup pipa yang terhubung dengan perintah awk
  pclose(fp);

  // Menghapus karakter newline dari akhir string jika ada
  result[strcspn(result, "\n")] = 0;

  // Mengembalikan hasil pencarian
  return strdup(result);
  }

char* get_anime_name(const char* buffer, int index) {
  // Buat salinan string buffer untuk diproses
  char* buffer_copy = strdup(buffer);

  // Deklarasi variabel token dan pointer nama anime
  char* token = NULL;
  char* anime_name = NULL;

  // Pemisahan string menggunakan strtok
  token = strtok(buffer_copy, " ");
  token = strtok(NULL, ",");
  // Melompati token-tokem sebelumnya untuk mencapai token yang diinginkan
  for (int i = 0; i < index; i++) {
    token = strtok(NULL, ",");
    // Memeriksa apakah token telah mencapai akhir string atau tidak
    if (token == NULL) {
      // Jika token tidak ditemukan, dealokasikan memori yang dialokasikan untuk buffer_copy dan kembalikan NULL
      free(buffer_copy);
      return NULL;
      }
    }

  // Alokasikan memori untuk nama anime dan salin token ke dalamnya
  anime_name = strdup(token);

  // Dealokasi memori yang dialokasikan untuk buffer_copy
  free(buffer_copy);
  // Kembalikan pointer ke nama anime
  return anime_name;
  }

void send_command_output(int sock, const char* command) {
  // Menjalankan perintah sistem dan menangkap outputnya
  FILE* fp;
  char output[BUFFER_SIZE] = { 0 };
  char full_output[8192] = { 0 }; // Menampung seluruh output dalam satu string
  fp = popen(command, "r");
  if (fp == NULL) {
    printf("Failed to run command\n");
    return;
    }
  while (fgets(output, sizeof(output), fp) != NULL) {
    // Menggabungkan setiap baris output ke dalam string penuh
    strcat(full_output, output);
    }
  // Mengirimkan seluruh output ke client dalam satu pesan
  send(sock, full_output, strlen(full_output), 0);
  pclose(fp);
  }


void log_command(const char* action, const char* cmd, int sock) {
  // Buka file log
  FILE* log_file = fopen("../change.log", "a");
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

  char* anime_name = get_anime_name(cmd, 0);
  // Menulis log ke file
  if (strcmp(action, "add") == 0) {
    fprintf(log_file, "[%s][ADD] %s ditambahkan\n", timestamp, anime_name);
    }

  if (strcmp(action, "edit") == 0) {
    char* old_anime_name = get_anime_name(cmd, 0);
    char* old_anime = get_anime_info(old_anime_name, "../myanimelist.csv", sock);

    char* new_anime = get_new_anime(cmd);

    fprintf(log_file, "[%s][EDIT] %s diubah menjadi %s\n", timestamp, old_anime, new_anime);
    }
  if (strcmp(action, "delete") == 0) {
    fprintf(log_file, "[%s][DEL] %s berhasil dihapus\n", timestamp, anime_name);
    }

  // Menutup file log
  fclose(log_file);
  }

int main(int argc, char const* argv[]) {
  int server_fd, new_socket, valread;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  char buffer[BUFFER_SIZE] = { 0 };
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
    valread = read(new_socket, buffer, BUFFER_SIZE);
    buffer[valread] = '\0'; // Menambahkan null-terminator ke akhir string yang dibaca

    if (valread > 0) {
      if (strcmp(buffer, "exit") == 0) {
        printf("Received: %s\n", buffer);
        char* exit = "\nExiting the client\n";
        send(new_socket, exit, strlen(exit), 0);
        break; // Keluar dari loop
        }
      else if (strcmp(buffer, "tampilkan") == 0) {
        char command[200] = "cat ../myanimelist.csv | awk -F, '{print NR \". \" $3}'";
        printf("Received: %s\n\n", buffer);
        send_command_output(new_socket, command);
        }
      else if (strncmp(buffer, "hari ", 5) == 0) {
        char* hari = buffer + 5; // Mengambil string setelah "hari "
        char command[100];
        sprintf(command, "grep '^%s,' ../myanimelist.csv | awk -F, '{print NR \". \" $3}'", hari);
        printf("Received: %s\n\n", buffer);
        send_command_output(new_socket, command);
        }
      else if (strncmp(buffer, "genre ", 6) == 0) {
        char* genre = buffer + 6; // Mengambil string setelah "genre "
        char command[100];
        sprintf(command, "grep ',%s,' ../myanimelist.csv | awk -F, '{print NR \". \" $3}'", genre);
        printf("Received: %s\n\n", buffer);
        send_command_output(new_socket, command);
        }
      else if (strncmp(buffer, "add ", 4) == 0) {
        char* data = buffer + 4;
        char command[100];
        sprintf(command, "echo \"%s\" >> ../myanimelist.csv", data);
        printf("Received: %s\n\n", buffer);
        send_command_output(new_socket, command);
        log_command("add", buffer, new_socket);
        char* comment = "Anime berhasil ditambahkan\n";
        send(new_socket, comment, strlen(comment), 0);
        }
      else if (strncmp(buffer, "status ", 7) == 0) {
        char* name = buffer + 7;
        char command[100];
        sprintf(command, "grep ',%s,' ../myanimelist.csv | awk -F, '{print $4}'", name);
        printf("Received: %s\n\n", buffer);
        send_command_output(new_socket, command);
        }
      else if (strncmp(buffer, "edit ", 5) == 0) {
        char* input = buffer + 5;
        char command[250];

        log_command("edit", buffer, new_socket);
        printf("Received: %s\n\n", buffer);
        // Pisahkan input berdasarkan koma
        char* anime = strtok(input, ",");
        char* hari = strtok(NULL, ",");
        char* genre = strtok(NULL, ",");
        char* judul = strtok(NULL, ",");
        char* status = strtok(NULL, ",");
        // Membangun perintah untuk mengedit file CSV menggunakan perintah awk
        sprintf(command, "awk -F, '{ if ($3 == \"%s\") { print \"%s,%s,%s,%s\" } else { print $0 } }' ../myanimelist.csv > temp.csv && mv temp.csv ../myanimelist.csv", anime, hari, genre, judul, status);

        send_command_output(new_socket, command);

        char* comment = "Anime berhasil diedit\n";
        send(new_socket, comment, strlen(comment), 0);
        }
      else if (strncmp(buffer, "delete ", 7) == 0) {
        char* anime = buffer + 7;
        char command[200];

        // Membangun perintah untuk menghapus baris berdasarkan nama anime menggunakan awk
        sprintf(command, "awk -F, '$3 != \"%s\"' ../myanimelist.csv > temp.csv && mv temp.csv ../myanimelist.csv", anime);
        log_command("delete", buffer, new_socket);
        printf("Received: %s\n\n", buffer);
        send_command_output(new_socket, command);
        char* comment = "Anime berhasil dihapus\n";
        send(new_socket, comment, strlen(comment), 0);
        }
      else {
        printf("Received: %s\n\n", buffer);
        char* invalid = "Invalid Command\n";
        send(new_socket, invalid, strlen(invalid), 0);

        memset(buffer, 0, BUFFER_SIZE);
        }
      }
    }

  return 0;
  }