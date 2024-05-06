#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILENAME_LENGTH 256

typedef struct {
    char name[MAX_FILENAME_LENGTH];
    float rating;
} Place;

void printBestPlace(Place place, const char *type) {
    printf("Type: %s\n", type);
    printf("Filename: %s\n", place.name);
    printf("------------------\n");
    printf("Name: %s\n", place.name);
    printf("Rating: %.1f\n", place.rating);
}

int main() {
    Place trashCan = {"", 0};
    Place parkingLot = {"", 0};

    FILE *file;
    char filename[MAX_FILENAME_LENGTH];
    float rating;
    char type[MAX_FILENAME_LENGTH];

    file = fopen("/Documents/dev/shm/belobog_trashcan.csv", "r");
    if (file) {
        while (fscanf(file, "%s %f", filename, &rating) == 2) {
            if (rating > trashCan.rating) {
                trashCan.rating = rating;
                strcpy(trashCan.name, filename);
            }
        }
        fclose(file);
    }

    file = fopen("/Documents/dev/shm/osaka_parkinglot.csv", "r");
    if (file) {
        while (fscanf(file, "%s %f", filename, &rating) == 2) {
            if (rating > parkingLot.rating) {
                parkingLot.rating = rating;
                strcpy(parkingLot.name, filename);
            }
        }
        fclose(file);
    }

    printBestPlace(trashCan, "Trash Can");
    printBestPlace(parkingLot, "Parking Lot");

    return 0;
}
