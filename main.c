#include <stdio.h>

// DEFINES
#define T_STR 100
#define T_MAX_LOCATIONS 5
#define T_MAX_SECTORS 5
#define T_MAX_SENSORS 5
#define T_MAX_INSPECTIONS 5

// TYPES
typedef char string [T_STR];

// ENTITIES
struct datetime {
    int day;
    int month;
    int year;
    int hour;
    int min;
    int sec;
} typedef t_date;

struct location {
    int id;
    string name;
} typedef t_location;

struct sensor_inspection {
    int id;
    int sensor_id;
    int value;
    t_date date_inspection;
} typedef t_sensor_inspection;

struct sensor {
    int id;
    int sector_id;
    string name;
    t_sensor_inspection inspections[T_MAX_INSPECTIONS];
} typedef t_sensor;

struct sector {
    int id;
    int location_id;
    string name;
    string description;
    t_sensor sensors[T_MAX_SENSORS];
} typedef t_sector;

// GLOBAL VARS
t_location locations[T_MAX_LOCATIONS];
// FUNCTIONS TYPES
int menu(void);
void createNewLocation(void);
void listAllLocations(void);
void deleteLocation(void);
void selectLocation(void);

int main(){
    int opt = menu();
    switch(opt){
        case 1: createNewLocation();
         break;
        case 2: listAllLocations();
        break;
        case 3: deleteLocation();
        break;
    }
    return 0;
}

int menu(){
    for(;;){
        int opt = 0;
        printf("Bem-vindo. \n");
        printf("Escolha uma opção (0-9): \n");
        printf("0. Fechar (Sair do programa):. \n");
        scanf("%i", &opt);
        return opt;
    }
}

void createNewLocation(void){}
void listAllLocations(void){}
void deleteLocation(void){}
void selectLocation(void){}