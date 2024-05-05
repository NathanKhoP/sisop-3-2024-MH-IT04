#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

char* get_anime_info2(const char* buffer) {
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


char* get_anime_info(const char* anime_name, const char* csv_file) {
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
  printf("%s comand", command);
  // Membaca output dari perintah awk dan menyimpannya dalam variabel result
  if (fgets(result, sizeof(result), fp) == NULL) {
    perror("fgets failed");
    exit(EXIT_FAILURE);
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
  printf("%s aniename", anime_name);
  // Kembalikan pointer ke nama anime
  return anime_name;
  }

int main() {

  // URL dari file yang ingin diunduh
  const char url[] = "https://drive.google.com/uc?id=10p_kzuOgaFY3WT6FVPJIXFbkej2s9f50&export=download";
  const char file_name[] = "../myanimelist.csv"; // Sesuaikan dengan nama file yang diinginkan

  // Memanggil fungsi untuk mengunduh file
  download_file(url, file_name);
  char buffer[] = "edit TES,Jumat,Romance,APA,completed";
  char* anime_info = get_anime_info2(buffer);
  if (anime_info != NULL) {
    printf("Anime info: %s\n", anime_info);
    free(anime_info); // Dealokasi memori setelah penggunaan selesai
    }
  // String buffer
  // char buffer[] = "edit Slime,Jumat,Action,Haikyuu,Ongoing";

  // // Mendapatkan nama anime dengan indeks 0 (Kanokari)
  // char* anime_name_1 = get_anime_name(buffer, 0);
  // if (anime_name_1 != NULL) {
  //   printf("Nama anime (index 0): %s\n", anime_name_1);
  //   free(anime_name_1); // Dealokasi memori
  //   }
  // else {
  //   printf("Token tidak ditemukan atau indeks di luar jangkauan\n");
  //   }

  // char* old_anime_name = get_anime_name(buffer, 0);
  // char* old_anime = get_anime_info(old_anime_name, "../myanimelist.csv");

  // char* new_anime_name = get_anime_name(buffer, 3);
  // char* new_anime = get_anime_info(new_anime_name, "../myanimelist.csv");

  // // Mendapatkan informasi anime Haikyuu dari file CSV
  // char* anime_info = get_anime_info("Haikyuu", "../myanimelist.csv");
  // printf("Informasi anime Haikyuu: %s\n", new_anime);
  // printf("Informasi anime Haikyuu: %s\n", old_anime);
  // free(anime_info); // Dealokasi memori

  return 0;
  }
