// - INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// - DEFINES
#define T_STR 100
#define T_MAX_LOCATIONS 5
#define T_MAX_SECTORS 5
#define T_MAX_SENSORS 5
#define T_MAX_INSPECTIONS 5
#define T_MAX_SENSORS_TYPES 5
#define END 0
#define NOT_FOUND -1

// - TYPES
typedef char string [T_STR];
enum entities {
    LOCATION,
    SECTOR,
    SENSOR,
    INSPECTION
} typedef entities;
typedef enum {
    TEMPERATURE = 0,
    VIBRATION,
    PRESSURE,
    CURRENT,
    HUMIDITY
} sensor_types;
struct datetime {
    string date;
} typedef t_date_string;
struct sensor_inspection {
    int id;
    int sensor_id;
    float value;
    time_t date_inspection;
} typedef t_inspection;
struct sensor {
    int id;
    int sector_id;
    string name;
    sensor_types sensor_type;
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
const string sensor_type_string[T_MAX_SENSORS_TYPES] = {"TEMPERATURE", "VIBRATION", "PRESSURE", "CURRENT", "HUMIDITY"};

bool running = true;

t_location locations[T_MAX_LOCATIONS];
int locations_quantity = 0;

int location_selected_idx = NOT_FOUND;
int sector_selected_idx = NOT_FOUND;
int sensor_selected_idx = NOT_FOUND;
int inspection_selected_idx = NOT_FOUND;

// FUNCTIONS TYPES
void menu_locations(void);
void menu_sectors(void);
void menu_sensors(void);

void action_menu_locations(int option);
void action_menu_sectors(int option);
void action_menu_sensors(int option);
void action_menu_sensors_inspection(int option);

t_location createNewLocation(void);
t_location findLocation(int location_id);
void listAllLocations(void);
void selectLocation(void);
int findLocationIdx(int location_id);

t_sector createNewSector(void);
t_sector findSector(int sector_id);
void listAllSectorsFromLocation(void);
void selectSector(void);
int findSectorIdx(int sector_id);

t_sensor createNewSensor(void);
t_sensor findSensor(int sensor_id);
void listAllSensorsFromSector(void);
void selectSensor(void);
int findSensorIdx(int sensor_id);

t_inspection createNewInspection(void);
t_inspection findInspection(int inspection_id);
void listAllInspectionsFromSensor(void);
void selectInspection(void);
int findInspectionIdx(int inspection_id);

void removeEnterFromString(string str);
void resetStatesSelected(entities entity);
bool checkExistenceId(int id, entities entity);
void formatStringToSystemPattern(string str);
void showError(const char *error_message);
t_date_string convertTimestampToString(time_t timestamp);

int main(){
    srand(time(NULL));
    for(;;){
        if(!running){
            return 0;
        }
        if(
            location_selected_idx != NOT_FOUND &&
            sector_selected_idx != NOT_FOUND
        ){
            menu_sensors();
        } else if(
            location_selected_idx != NOT_FOUND
        ){
            menu_sectors(); 
        } else {
            menu_locations();
        }   
    }
    return 0;
}

// - MENU FUNCTIONS
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
            case 1: 
                if(locations_quantity == T_MAX_LOCATIONS){
                    showError("Número de plantas máximo atigido.");
                    break;
                }
                int new_index_to_insert = locations_quantity;
                locations[new_index_to_insert] = createNewLocation();
                locations_quantity++;
            break;
            case 2: 
                selectLocation();
            break;
            default: break;
        }
}
void menu_sectors(){
        int opt;
        printf("Planta: %s. Escolha uma opção (0-9): \n", locations[location_selected_idx].name);
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
                if(locations[location_selected_idx].sectors_quantity == T_MAX_SECTORS){
                    showError("Número de setores máximo atigido.");
                    break;
                }

                int new_index_to_insert = 
                locations[location_selected_idx].sectors_quantity;

                locations[location_selected_idx]
                .sectors[new_index_to_insert] = createNewSector();

                locations[location_selected_idx].sectors_quantity++;
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
        if(sensor_selected_idx == NOT_FOUND){
        printf("Setor: %s. Escolha uma opção (0-9): \n", 
            locations[location_selected_idx]
            .sectors[sector_selected_idx].name
        );
        printf("1. Criar sensor. \n");
        printf("2. Listar sensores. \n");
        printf("3. Selecionar sensor. \n");
        printf("0. Voltar. \n");
        scanf("%i", &opt);
        getchar();
        action_menu_sensors(opt);
        } else {
        printf("Sensor: %s. Escolha uma opção (0-9): \n", 
            locations[location_selected_idx]
            .sectors[sector_selected_idx]
            .sensors[sensor_selected_idx].name
        );
        printf("1. Gerar leitura no sensor. \n");
        printf("2. Listar leituras do sensor. \n");
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
                if(locations[location_selected_idx].sectors[sector_selected_idx].sensors_quantity == T_MAX_SENSORS){
                    showError("Número de sensores máximo atigido.");
                    break;
                }

                int new_index_to_insert = 
                locations[location_selected_idx]
                .sectors[sector_selected_idx].sensors_quantity;

                locations[location_selected_idx]
                .sectors[sector_selected_idx]
                .sensors[new_index_to_insert] = createNewSensor();

                locations[location_selected_idx]
                .sectors[sector_selected_idx].sensors_quantity++;
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
                if(locations[location_selected_idx]
                    .sectors[sector_selected_idx]
                    .sensors[sensor_selected_idx]
                    .inspections_quantity == T_MAX_INSPECTIONS){
                    showError("Número de leituras máximo atigido.");
                    break;
                }

                int new_index_to_insert = 
                locations[location_selected_idx]
                .sectors[sector_selected_idx]
                .sensors[sensor_selected_idx].inspections_quantity;

                locations[location_selected_idx]
                .sectors[sector_selected_idx]
                .sensors[sensor_selected_idx]
                .inspections[new_index_to_insert] = createNewInspection();

                locations[location_selected_idx]
                .sectors[sector_selected_idx]
                .sensors[sensor_selected_idx].inspections_quantity++;
                break;
            case 2:
                listAllInspectionsFromSensor();
                break;
            case 0: 
                resetStatesSelected(SENSOR);
                break;
            default:
                break;
        }
}

// - LOCATION FUNCTIONS
t_location createNewLocation(void){
    t_location new_location;
    printf("Digite o nome: \n");
    fgets(new_location.name, T_STR, stdin);
    formatStringToSystemPattern(new_location.name);
    new_location.id = locations_quantity + 1;
    new_location.sectors_quantity = 0;
    printf("Nova planta cadastrada com sucesso. \n");
    return new_location;
}
t_location findLocation(int location_id){
    t_location location;
    location.id = NOT_FOUND;
    for(int i = 0; i < locations_quantity; i++){
        if(locations[i].id == location_id){
            location = locations[i];
            return location;
        }
    }
    if(location.id == NOT_FOUND) showError("Nenhuma planta encontrado.");
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
        location_selected_idx = findLocationIdx(location_id);
    } else {
        showError("O Id que você inseriu não corresponde a nenhuma planta existente");
    }
}
int findLocationIdx(int location_id){
    int idx = NOT_FOUND;
    for(int i = 0; i < locations_quantity; i++){
        if(locations[i].id == location_id){
            idx = i;
            return idx;
        }
    }
    if(idx == NOT_FOUND) printf("Nenhuma location encontrada. \n");
    return idx;
};

// - SECTOR FUNCTIONS
t_sector createNewSector(){
    t_sector new_sector;
    new_sector.id = locations[location_selected_idx].sectors_quantity + 1;
    new_sector.location_id = locations[location_selected_idx].id;
    new_sector.sensors_quantity = 0;
    printf("Digite o nome do setor: \n");
    fgets(new_sector.name, T_STR, stdin);
    formatStringToSystemPattern(new_sector.name);
    printf("Digite uma descrição do setor: \n");
    fgets(new_sector.description, T_STR, stdin);
    formatStringToSystemPattern(new_sector.description);
    printf("Novo setor (Sector) Criado com sucesso. \n");
    return new_sector;
}
t_sector findSector(int sector_id){
    t_sector sector;
    sector.id = NOT_FOUND;
    for(int i = 0; i < locations[location_selected_idx].sectors_quantity; i++){
        if(locations[location_selected_idx].sectors[i].id == sector_id){
            sector = locations[location_selected_idx].sectors[i];
            return sector;
        }
    }
    if(sector.id == NOT_FOUND) showError("Nenhum setor encontrado.");
    return sector;
}
void listAllSectorsFromLocation(){
    if(locations[location_selected_idx].sectors_quantity < 1){
        printf("Não existem setores disponíveis. \n");
        return;
    }
    for(int i = 0; i < locations[location_selected_idx].sectors_quantity; i++){
        printf("ID: %i,  Nome do setor: %s. \n", locations[location_selected_idx].sectors[i].id, locations[location_selected_idx].sectors[i].name);
    }
}
void selectSector(void){
    if(locations[location_selected_idx].sectors_quantity < 1){
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
        sector_selected_idx = findSectorIdx(sector_id);
    } else {
        showError("O Id que você inseriu não corresponde a nenhum setor existente");
    }
}
int findSectorIdx(int sector_id){
    int idx = NOT_FOUND;
    if(location_selected_idx == NOT_FOUND){
        printf("Impossível executar essa ação sem uma location selecionada. \n");
        return idx;
    }
    for(int i = 0; i < locations[location_selected_idx].sectors_quantity; i++){
        if(locations[location_selected_idx]
            .sectors[i].id == sector_id){
            idx = i;
            return idx;
        }
    }
    if(idx == NOT_FOUND) printf("Nenhum sector encontrada. \n");
    return idx;
};

// - SENSORS FUNCTIONS
t_sensor createNewSensor(void){
    t_sensor new_sensor;
    new_sensor.id = locations[location_selected_idx].sectors[sector_selected_idx].sensors_quantity + 1;
    new_sensor.sector_id = locations[location_selected_idx].sectors[sector_selected_idx].id;
    new_sensor.inspections_quantity = 0;
    printf("Digite o nome do sensor: \n");
    fgets(new_sensor.name, T_STR, stdin);
    formatStringToSystemPattern(new_sensor.name);
    printf("Digite o número correspondente ao tipo de sensor: \n");
    printf("0. TEMPERATURA \n");
    printf("1. VIBRAÇÃO \n");
    printf("2. PRESSÃO \n");
    printf("3. CORRENTE \n");
    printf("4. UMIDADE \n");
    int number_typed;
    scanf("%i", &number_typed);
    getchar();
    int is_number_typed_valid = number_typed == 0 || number_typed == 1 || number_typed == 2 || number_typed == 3 || number_typed == 4;
    if(is_number_typed_valid){
        new_sensor.sensor_type = number_typed;
        printf("Novo sensor (Sensor) Criado com sucesso. \n");
    } else {
        new_sensor.sensor_type = 0;
        showError("você não digitou um número válido. - Tipo Padrão: Temperatura (0).");
    }
    return new_sensor;
}
t_sensor findSensor(int sensor_id){
    t_sensor sensor;
    sensor.id = NOT_FOUND;
    for(int i = 0; i < locations[location_selected_idx].sectors[sector_selected_idx].sensors_quantity; i++){
        if(locations[location_selected_idx].sectors[sector_selected_idx].sensors[i].id == sensor_id){
            sensor = locations[location_selected_idx].sectors[sector_selected_idx].sensors[i];
            return sensor;
        }
    }
    if(sensor.id == NOT_FOUND) showError("Nenhum sensor encontrado.");
    return sensor;
}
void listAllSensorsFromSector(){
    if(locations[location_selected_idx].sectors[sector_selected_idx].sensors_quantity < 1){
        printf("Não existem sensores disponíveis. \n");
        return;
    }
    for(int i = 0; i < locations[location_selected_idx].sectors[sector_selected_idx].sensors_quantity; i++){
        printf("ID: %i,  Nome do sensor: %s., tipo de sensor: %s  \n", 
            locations[location_selected_idx]
            .sectors[sector_selected_idx]
            .sensors[i].id, locations[location_selected_idx]
            .sectors[sector_selected_idx].sensors[i].name, 
            sensor_type_string[locations[location_selected_idx]
            .sectors[sector_selected_idx]
            .sensors[i].sensor_type]);
    }
}
void selectSensor(void){
    if(locations[location_selected_idx].sectors[sector_selected_idx].sensors_quantity < 1){
        printf("Não existem sensores disponíveis. \n");
        return;
    }
    printf("Selecione um sensor (Sensor): \n");
    listAllSensorsFromSector();
    printf("Digite o id do sensor (Sensor): \n");
    int sensor_id;
    scanf("%i", &sensor_id);
    int sensorIsValid = checkExistenceId(sensor_id, SENSOR);
    if(sensorIsValid){
        sensor_selected_idx = findSensorIdx(sensor_id);
    } else {
        showError("O Id que você inseriu não corresponde a nenhum sensor existente");
    } 
}
int findSensorIdx(int sensor_id){
    int idx = NOT_FOUND;
    if(location_selected_idx == NOT_FOUND ||
       sector_selected_idx == NOT_FOUND
    ){
        printf("Impossível executar essa ação sem um setor selecionado. \n");
        return idx;
    }
    for(int i = 0; i < locations[location_selected_idx].sectors[sector_selected_idx].sensors_quantity; i++){
        if(locations[location_selected_idx]
            .sectors[sector_selected_idx]
            .sensors[i].id == sensor_id){
            idx = i;
            return idx;
        }
    }
    if(idx == NOT_FOUND) printf("Nenhum sensor encontrado. \n");
    return idx;
};

// - INSPECTIONS FUNCTIONS
t_inspection createNewInspection(void){
    t_inspection new_inspection;
    new_inspection.id = locations[location_selected_idx].sectors[sector_selected_idx].sensors[sensor_selected_idx].inspections_quantity + 1;
    new_inspection.sensor_id = locations[location_selected_idx].sectors[sector_selected_idx].sensors[sensor_selected_idx].id;
    printf("Digite o valor da leitura: \n");
    scanf("%f", &new_inspection.value);
    getchar();
    time_t date;
    time(&date); 
    new_inspection.date_inspection = date;
    printf("%li.\n", date);
    printf("Leitura gerada com sucesso. \n");
    return new_inspection;
}
t_inspection findInspection(int inspection_id){
    t_inspection inspection;
    inspection.id = NOT_FOUND;
    for(int i = 0; i < locations[location_selected_idx].sectors[sector_selected_idx].sensors[sensor_selected_idx].inspections_quantity; i++){
        if(locations[location_selected_idx].sectors[sector_selected_idx].sensors[sensor_selected_idx].inspections[i].id == inspection_id){
            inspection = locations[location_selected_idx].sectors[sector_selected_idx].sensors[sensor_selected_idx].inspections[i];
            return inspection;
        }
    }
    if(inspection.id == NOT_FOUND) showError("Nenhuma leitura encontrado.");
    return inspection;
}
void listAllInspectionsFromSensor(){
    if(locations[location_selected_idx].sectors[sector_selected_idx].sensors[sensor_selected_idx].inspections_quantity < 1){
        printf("Não existem leituras disponíveis. \n");
        return;
    }
    for(int i = 0; i < locations[location_selected_idx].sectors[sector_selected_idx].sensors[sensor_selected_idx].inspections_quantity; i++){
            
        t_date_string date_struct = convertTimestampToString(
            locations[location_selected_idx]
            .sectors[sector_selected_idx]
            .sensors[sensor_selected_idx]
            .inspections[i].date_inspection);

        printf("ID: %i,  Valor: %f. Data: %s. \n", 
            locations[location_selected_idx].sectors[sector_selected_idx].sensors[sensor_selected_idx].inspections[i].id, 
            locations[location_selected_idx].sectors[sector_selected_idx].sensors[sensor_selected_idx].inspections[i].value,
            date_struct.date
        );
    }
}
void selectInspection(void){
    if(locations[location_selected_idx].sectors[sector_selected_idx].sensors[sensor_selected_idx].inspections_quantity < 1){
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
        inspection_selected_idx = findInspectionIdx(inspection_id);
    } else {
        showError("O Id que você inseriu não corresponde a nenhuma inspection existente");
    }
}
int findInspectionIdx(int inspection_id){
    int idx = NOT_FOUND;
    if(location_selected_idx == NOT_FOUND ||
       sector_selected_idx == NOT_FOUND ||
       sensor_selected_idx == NOT_FOUND 
    ){
        printf("Impossível executar essa ação sem um sensor selecionado. \n");
        return idx;
    }
    for(int i = 0; i < locations[location_selected_idx].sectors[sector_selected_idx].sensors[sensor_selected_idx].inspections_quantity; i++){
         if(locations[location_selected_idx]
            .sectors[sector_selected_idx]
            .sensors[sensor_selected_idx]
            .inspections[i].id == inspection_id){
            idx = i;
            return idx;
        }
    }
    return idx;
};

// - UTILS
void resetStatesSelected(entities entity){
    switch(entity){
        case LOCATION:
                {
                location_selected_idx = NOT_FOUND;
                sector_selected_idx = NOT_FOUND;
                sensor_selected_idx = NOT_FOUND;
                inspection_selected_idx = NOT_FOUND;
                break;
                }
        case SECTOR:
                {
                sector_selected_idx = NOT_FOUND;
                sensor_selected_idx = NOT_FOUND;
                inspection_selected_idx = NOT_FOUND;
                break;
                }
        case SENSOR:
                {
                sensor_selected_idx = NOT_FOUND;
                inspection_selected_idx = NOT_FOUND;
                break;
                }
        case INSPECTION:
                {
                inspection_selected_idx = NOT_FOUND;
                break;
                }
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
            for(int i = 0; i < locations[location_selected_idx].sectors_quantity; i++){
                if(locations[location_selected_idx].sectors[i].id == id){
                    hasFoundId = 1;
                }
            }
            break;
        case SENSOR:
            for(int i = 0; i < locations[location_selected_idx]
                .sectors[sector_selected_idx]
                .sensors_quantity; i++){
                if(locations[location_selected_idx]
                    .sectors[sector_selected_idx]
                    .sensors[i].id == id){
                    hasFoundId = 1;
                }
            }
            break;
        case INSPECTION:
            for(int i = 0; i < locations[location_selected_idx]
                .sectors[sector_selected_idx]
                .sensors[sensor_selected_idx]
                .inspections_quantity; i++){
                if(locations[location_selected_idx]
                    .sectors[sector_selected_idx]
                    .sensors[sensor_selected_idx]
                    .inspections[i].id == id){
                    hasFoundId = 1;
                }
            }
            break;
    }
    return hasFoundId;
}
void removeEnterFromString(string str){
    str[strlen(str) - 1] = '\0';
}
void formatToUpperString(string str){
    for(int i = 0; str[i] != '\0'; i++){
        str[i] = toupper(str[i]);
    }
}
void formatStringToSystemPattern(string str){
    removeEnterFromString(str);
    formatToUpperString(str);
}
void showError(const char *error_message){
    printf("Erro! Mensagem do sistema: %s.\n", error_message);
}
t_date_string convertTimestampToString(time_t timestamp){
    t_date_string struct_date;
    struct tm* tm_info = localtime(&timestamp);
    char date_str[50];
    strftime(date_str, sizeof(date_str), "%d/%m/%Y %H:%M:%S", tm_info);
    strcpy(struct_date.date, date_str);
    return struct_date;
}