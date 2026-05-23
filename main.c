#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

// DEFINES
#define T_STR 100
#define T_MAX_LOCATIONS 5
#define T_MAX_SECTORS 5
#define T_MAX_SENSORS 5
#define T_MAX_INSPECTIONS 5
#define END 0
#define NOT_FOUND -1

// TYPES
typedef char string [T_STR];

enum entities {
    LOCATION,
    SECTOR,
    SENSOR,
    INSPECTION
} typedef entities;

struct datetime {
    int day;
    int month;
    int year;
    int hour;
    int min;
    int sec;
} typedef t_date;

struct sensor_inspection {
    int id;
    int sensor_id;
    int value;
    t_date date_inspection;
} typedef t_inspection;

struct sensor {
    int id;
    int sector_id;
    string name;
    t_inspection inspections[T_MAX_INSPECTIONS];
    int inspections_quantity;
} typedef t_sensor;

struct sector {
    int id;
    int location_id;
    string name;
    string description;
    t_sensor sensors[T_MAX_SENSORS];
    int sensors_quantity;
} typedef t_sector;

struct location {
    int id;
    string name;
    t_sector sectors[T_MAX_SECTORS];
    int sectors_quantity;
} typedef t_location;

// GLOBAL VARS
int running = true;
t_location locations[T_MAX_LOCATIONS] = {
    {
        .id = 1,
        .name = "UFSC",
        .sectors = {
            {
                .id = 1,
                .location_id = 1,
                .name = "Laboratorio IoT",
                .description = "Setor de sensores inteligentes",
                .sensors = {
                    {
                        .id = 1,
                        .sector_id = 1,
                        .name = "Sensor Temperatura",
                        .inspections = {
                            {
                                .id = 1,
                                .sensor_id = 1,
                                .value = 25,
                                .date_inspection = {
                                    .day = 22,
                                    .month = 5,
                                    .year = 2026,
                                    .hour = 10,
                                    .min = 30,
                                    .sec = 0
                                }
                            }
                        },
                        .inspections_quantity = 1
                    }
                },
                .sensors_quantity = 1
            }
        },
        .sectors_quantity = 1
    },
    {
        .id = 2,
        .name = "IFSC",
        .sectors = {
            {
                .id = 2,
                .location_id = 2,
                .name = "Patio Externo",
                .description = "Monitoramento ambiental",
                .sensors = {
                    {
                        .id = 2,
                        .sector_id = 2,
                        .name = "Sensor Umidade",
                        .inspections = {
                            {
                                .id = 2,
                                .sensor_id = 2,
                                .value = 70,
                                .date_inspection = {
                                    .day = 22,
                                    .month = 5,
                                    .year = 2026,
                                    .hour = 11,
                                    .min = 15,
                                    .sec = 10
                                }
                            }
                        },
                        .inspections_quantity = 1
                    }
                },
                .sensors_quantity = 1
            }
        },
        .sectors_quantity = 1
    }
};
int locations_quantity = 2;
t_location location_selected = { .id = NOT_FOUND };
t_sector location_sector_selected = { .id = NOT_FOUND };
t_sensor location_sector_sensor_selected = { .id = NOT_FOUND };
t_inspection location_sector_sensor_inspection_selected = { .id = NOT_FOUND };

// FUNCTIONS TYPES
void menu_locations(void);
void action_menu_locations(int option);
void menu_sectors(void);
void action_menu_sectors(int option);
void menu_sensors(void);
void action_menu_sensors(int option);
void action_menu_sensors_inspection(int option);

t_location createNewLocation(void);
t_location findLocation(int location_id);
void listAllLocations(void);
void selectLocation(void);
t_sector createNewSector(void);
t_sector findSector(int sector_id);
void listAllSectorsFromLocation(void);
void selectSector(void);
void listAllSensorsFromSector(void);
t_sensor findSensor(int sensor_id);
void selectSensor(void);
void listAllInspectionsFromSensor(void);
t_inspection findInspection(int inspection_id);
void selectInspection(void);
void removeEnterFromString(string str);
void resetStatesSelected(entities entity);
bool checkExistenceId(int id, entities entity);

int main(){
    srand(time(NULL));
    for(;;){
        if(!running){
            return 0;
        }
    if(
        location_selected.id != NOT_FOUND &&
        location_sector_selected.id != NOT_FOUND &&
        location_sector_sensor_selected.id != NOT_FOUND
    ){
        menu_sensors();
    } else if(
        location_selected.id != NOT_FOUND &&
        location_sector_selected.id != NOT_FOUND
    ){
        menu_sensors();
    } else if(
        location_selected.id != NOT_FOUND
    ){
        menu_sectors(); 
    } else {
        menu_locations();
    }
}
    return 0;
}
//MENU FUNCTIONS
void menu_locations(){
        int opt;
        printf("Escolha uma opção (0-9): \n");
        printf("1. Criar planta (0-9): \n");
        printf("2. Selecionar planta (0-9): \n");
        printf("0. Fechar (Sair do programa):. \n");
        scanf("%i", &opt);
        getchar();
        if(opt == 0){
            running = false;
            return;
        }
        action_menu_locations(opt);
}
void action_menu_locations(int option){
    switch(option){
            case 1: break;
            case 2: 
                selectLocation();
            break;
            default: break;
        }
}
void menu_sectors(){
        int opt;
        printf("Planta: %s. Escolha uma opção (0-9): \n", location_selected.name);
        printf("1. Criar setor. \n");
        printf("2. Listar todos Setores. \n");
        printf("3. Selecionar setor. \n");
        printf("0. Voltar. \n");
        scanf("%i", &opt);
        getchar();
        action_menu_sectors(opt);
}
void action_menu_sectors(int option){
        switch (option)
            {
            case 1:
                break;
            case 2:
                listAllSectorsFromLocation();
                break;
            case 3:
                selectSector();
                break;
            case 0: 
                resetStatesSelected(LOCATION);
                break;
            default:
                break;
        }
}
void menu_sensors(){
        int opt;
        printf("Setor: %s. Escolha uma opção (0-9): \n", location_sector_selected.name);
        if(location_sector_sensor_selected.id == NOT_FOUND){
        printf("1. Criar sensor. \n");
        printf("2. Listar sensor. \n");
        printf("3. Selecionar sensor. \n");
        printf("0. Voltar. \n");
        scanf("%i", &opt);
        getchar();
        action_menu_sensors(opt);
        } else {
        printf("Sensor: %s. Escolha uma opção (0-9): \n", location_sector_sensor_selected.name);
        printf("1. Listar leituras do sensor. \n");
        printf("0. Voltar. \n");
        scanf("%i", &opt);
        getchar();
        action_menu_sensors_inspection(opt);
        }
}
void action_menu_sensors(int option){
    switch (option)
        {
            case 1:
                break;
            case 2:
                listAllSensorsFromSector();
                break;
            case 3:
                selectSensor();
                break;
            case 0: 
                resetStatesSelected(SECTOR);
                break;
            default:
                break;
        }
}
void action_menu_sensors_inspection(int option){
    switch (option)
        {
            case 1:
                listAllInspectionsFromSensor();
                break;
            case 0: 
                resetStatesSelected(SENSOR);
                break;
            default:
                break;
        }
}
//LOCATION FUNCTIONS
t_location createNewLocation(void){
    t_location new_location;
    string name;
    printf("Digite o nome: \n");
    fgets(new_location.name, T_STR, stdin);
    new_location.name[strlen(new_location.name) - 1] = '\0';
    new_location.id = locations_quantity + 1;
    new_location.sectors_quantity = 0;
    printf("Nova planta cadastrada com sucesso. \n");
    return new_location;
}
t_location findLocation(int location_id){
    t_location location;
    for(int i = 0; i < locations_quantity; i++){
        if(locations[i].id == location_id){
            location = locations[i];
        }
    }
    return location;
}
void listAllLocations(void){
    for(int i = 0; i < locations_quantity; i++){
        printf("ID: %i, Nome: %s. \n", locations[i].id, locations[i].name);
    }
    printf("O número total de Plantas é: %i. \n", locations_quantity);
}
void selectLocation(void){
    if(locations_quantity < 1){
        printf("Não existem plantas disponíveis. \n");
        return;
    }
    printf("Selecione uma planta (Location): \n");
    listAllLocations();
    printf("Digite o id da planta (Location): \n");

    resetStatesSelected(LOCATION);
    int location_id;
    scanf("%i", &location_id);
    int locationIsValid = checkExistenceId(location_id, LOCATION);
    if(locationIsValid){
        location_selected = findLocation(location_id);
    }
}

//SECTOR FUNCTIONS
t_sector createNewSector(){
    t_sector new_sector;
    new_sector.id = sizeof(location_selected.sectors) / sizeof(t_sector);
    new_sector.location_id = location_selected.id;
    fgets(new_sector.name, T_STR, stdin);
    removeEnterFromString(new_sector.name);
    fgets(new_sector.description, T_STR, stdin);
    removeEnterFromString(new_sector.description);
    printf("Novo setor (Sector) Criado com sucesso. \n");
    location_selected.sectors_quantity++;
    return new_sector;
}
t_sector findSector(int sector_id){
    t_sector sector;
    for(int i = 0; i < location_selected.sectors_quantity; i++){
        if(location_selected.sectors[i].id == sector_id){
            sector = location_selected.sectors[i];
        }
    }
    return sector;
}
void listAllSectorsFromLocation(){
    for(int i = 0; i < T_MAX_SECTORS; i++){
        printf("ID: %i,  Nome do setor: %s. \n", location_selected.sectors[i].id, location_selected.sectors[i].name);
    }
}
void selectSector(void){
    if(location_selected.sectors_quantity < 1){
        printf("Não existem setores disponíveis. \n");
        return;
    }
    printf("Selecione um setor (Sector): \n");
    listAllSectorsFromLocation();
    printf("Digite o id do setor (Sector): \n");
    int sector_id;
    scanf("%i", &sector_id);
    int sectorIsValid = checkExistenceId(sector_id, SECTOR);
    if(sectorIsValid){
        location_sector_selected = findSector(sector_id);
    }
}

//SENSORS FUNCTIONS
t_sensor findSensor(int sensor_id){
    t_sensor sensor;
    for(int i = 0; i < location_sector_selected.sensors_quantity; i++){
        if(location_sector_selected.sensors[i].id == sensor_id){
            sensor = location_sector_selected.sensors[i];
        }
    }
    return sensor;
}
void listAllSensorsFromSector(){
    for(int i = 0; i < location_sector_selected.sensors_quantity; i++){
        printf("ID: %i,  Nome do setor: %s. \n", location_sector_selected.sensors[i].id, location_sector_selected.sensors[i].name);
    }
}
void selectSensor(void){
    if(location_sector_selected.sensors_quantity < 1){
        printf("Não existem sensores disponíveis. \n");
        return;
    }
    printf("Selecione um sensor (Sensor): \n");
    listAllSensorsFromSector();
    printf("Digite o id do sensor (Sector): \n");
    int sensor_id;
    scanf("%i", &sensor_id);
    int sensorIsValid = checkExistenceId(sensor_id, SENSOR);
    if(sensorIsValid){
        location_sector_sensor_selected = findSensor(sensor_id);
    }
}

//INSPECTIONS FUNCTIONS
t_inspection findInspection(int inspection_id){
    t_inspection inspection;
    for(int i = 0; i < location_sector_sensor_selected.inspections_quantity; i++){
        if(location_sector_sensor_selected.inspections[i].id == inspection_id){
            inspection = location_sector_sensor_selected.inspections[i];
        }
    }
    return inspection;
}
void listAllInspectionsFromSensor(){
    for(int i = 0; i < location_sector_sensor_selected.inspections_quantity; i++){
        printf("ID: %i,  Valor: %i. \n", location_sector_sensor_selected.inspections[i].id, location_sector_sensor_selected.inspections[i].value);
    }
}
void selectInspection(void){
    if(location_sector_sensor_selected.inspections_quantity < 1){
        printf("Não existem leituras disponíveis. \n");
        return;
    }
    printf("Selecione uma leitura (Inspection): \n");
    listAllInspectionsFromSensor();
    printf("Digite o id da leitura (Inspection): \n");
    int inspection_id;
    scanf("%i", &inspection_id);
    int inspectionIsValid = checkExistenceId(inspection_id, INSPECTION);
    if(inspectionIsValid){
        location_sector_sensor_inspection_selected = findInspection(inspection_id);
    }
}

// UTILS FUNCTIONS
void removeEnterFromString(string str){
    str[strlen(str) - 1] = '\0';
}
void resetStatesSelected(entities entity){
    switch(entity){
        case LOCATION: 
                location_selected.id = NOT_FOUND;
            break;
        case SECTOR:
                location_sector_selected.id = NOT_FOUND;
            break;
        case SENSOR:
                location_sector_sensor_selected.id = NOT_FOUND;
            break;
        case INSPECTION:
                location_sector_sensor_inspection_selected.id = NOT_FOUND;
            break;
    }
}
bool checkExistenceId(int id, entities entity){
    int hasFoundId = 0;
    switch(entity){
        case LOCATION: 
            for(int i = 0; i < locations_quantity; i++){
                if(locations[i].id == id){
                    hasFoundId = 1;
                }
            }
            break;
        case SECTOR:
            for(int i = 0; i < location_selected.sectors_quantity; i++){
                if(location_selected.sectors[i].id == id){
                    hasFoundId = 1;
                }
            }
            break;
        case SENSOR:
            for(int i = 0; i < location_sector_selected.sensors_quantity; i++){
                if(location_sector_selected.sensors[i].id == id){
                    hasFoundId = 1;
                }
            }
            break;
        case INSPECTION:
            for(int i = 0; i < location_sector_sensor_selected.inspections_quantity; i++){
                if(location_sector_sensor_selected.inspections[i].id == id){
                    hasFoundId = 1;
                }
            }
            break;
    }
    return hasFoundId;
}