#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* gap_func(float gap) {
    if (gap <= 3.5) return "Gogogo";
    else if (gap <= 10) return "Push";
    else return "Stay out of trouble";
}

char* fuel_func(float fuel) {
    if (fuel > 0.8) return "Push Push Push";
    else if (fuel >= 0.5) return "You can go";
    else return "Conserve Fuel";
}

char* tire_func(int tire) {
    if (tire > 80) return "Go Push Go Push";
    else if (tire >= 50) return "Good Tire Wear";
    else if (tire > 30) return "Conserve Your Tire";
    else return "Box Box Box";
}

char* tire_change(char* type) {
    if (strcmp(type, "Soft") == 0) return "Mediums Ready";
    else if (strcmp(type, "Medium") == 0) return "Box for Softs";
}