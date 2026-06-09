// IS_TESTING iniciado em TRUE para preenchimento automático de data no vetor de plantas.
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
#define T_MAX_SENSORS 3
#define T_MAX_INSPECTIONS 5
#define T_MAX_SENSORS_TYPES 5
#define T_MAX_INSPECTIONS_ON_SAME_DAY 2
#define END 0
#define NOT_FOUND -1
 
// - TYPES
typedef char string [T_STR];
typedef enum {
    LOCATION,
    SECTOR,
    SENSOR,
    INSPECTION
} entities;
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
    float range_min;
    float range_max;
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
 
// GLOBAL SCOPE VARS
const string sensor_type_string[T_MAX_SENSORS_TYPES] = {"TEMPERATURE", "VIBRATION", "PRESSURE", "CURRENT", "HUMIDITY"};
const string sensor_type_unit[T_MAX_SENSORS_TYPES] = {"°C", "mm/s", "PSI", "A", "%"};
 
bool running = true;
bool is_testing = false;
 
t_location locations[T_MAX_LOCATIONS];
int locations_quantity = 0;
 
int location_selected_idx = NOT_FOUND;
int sector_selected_idx = NOT_FOUND;
int sensor_selected_idx = NOT_FOUND;
int inspection_selected_idx = NOT_FOUND;
 
// FUNCTIONS TYPES
void menuLocations(void);
void menuSectors(void);
void menuSensors(void);
 
void actionMenuLocations(int option);
void actionMenuSectors(int option);
void actionMenuSensors(int option);
void actionMenuSensorsInspection(int option);
 
t_location createNewLocation(void);
t_location findLocationStruct(int location_id);
void listAllLocations(void);
void selectLocation(void);
int findLocationIdx(int location_id);
 
t_sector createNewSector(void);
t_sector findSectorStruct(int sector_id);
void listAllSectorsFromLocation(void);
void selectSector(void);
int findSectorIdx(int sector_id);
void generateReportsOfSectors(void);
 
t_sensor createNewSensor(void);
t_sensor findSensorStruct(int sensor_id);
void listAllSensorsFromSector(void);
void selectSensor(void);
int findSensorIdx(int sensor_id);
 
t_inspection createNewInspection(void);
t_inspection findInspectionStruct(int inspection_id);
void listAllInspectionsFromSensor(void);
void selectInspection(void);
int findInspectionIdx(int inspection_id);

void searchForSectorDescription(void);
void searchForSensorType(void);
void generateReportOfSensors(void);
void generateReportOfInspections(void);
void generateReportOfInspectionsVariation(void);
void generateReportOfInspectionsAverage(void);

void removeEnterFromString(string str);
void formatToUpperString(string str);
void formatStringToSystemPattern(string str);
t_date_string convertTimestampToString(time_t timestamp, bool enable_hours);

int checkQuantityOfInspectionsOnDate(time_t timestamp);
void resetStatesSelected(entities entity);
bool checkExistenceId(int id, entities entity);
bool isSameDay(time_t firstTimestamp, time_t secondTimestamp);
void generateDataAndFulfillForTest(void);
 
int main(){
    srand(time(NULL));
    if(is_testing) generateDataAndFulfillForTest();
    for(;;){
        if(!running){
            return 0;
        }
        if(
            location_selected_idx != NOT_FOUND &&
            sector_selected_idx != NOT_FOUND
        ){
            menuSensors();
        } else if(
            location_selected_idx != NOT_FOUND
        ){
            menuSectors(); 
        } else {
            menuLocations();
        }   
    }
    return 0;
}
 
// - MENU FUNCTIONS
void menuLocations(){
        int opt;
        printf("Escolha uma opção: \n");
        printf("1. Criar planta: \n");
        printf("2. Listar todas plantas: \n");
        printf("3. Selecionar planta: \n");
        printf("4. Gerar relatorio de sensores \n");
        printf("5. Gerar relatório de leituras \n");
        printf("6. Gerar relatório de setores \n");
        printf("7. Pesquisar sensor por tipo \n");
        printf("8. Pesquisar setor por descrição \n");
        printf("9. Relatório de médias de leitura por sensor \n");
        printf("10. Relatório de variação de leitura \n");
        printf("0. Fechar (Sair do programa) \n");
        scanf("%i", &opt);
        getchar();
        if(opt == 0){
            running = false;
            return;
        }
        actionMenuLocations(opt);
}
void actionMenuLocations(int option){
    switch(option){
            case 1: 
                if(locations_quantity == T_MAX_LOCATIONS){
                    printf("Erro: Número de plantas máximo atigido. \n");
                    break;
                }
                int new_index_to_insert = locations_quantity;
 
                t_location new_location = createNewLocation();
                if(new_location.id == NOT_FOUND){
                    printf("Erro: Erro na criação. \n");
                    break;
                }
 
                locations[new_index_to_insert] = new_location;
                locations_quantity++;
            break;
            case 2: 
                listAllLocations();
            break;
            case 3:
                selectLocation(); 
            break;
            case 4:
                generateReportOfSensors();
            break;
            case 5:
                generateReportOfInspections();
            break;
            case 6:
                generateReportsOfSectors();
            break;
            case 7:
                searchForSensorType();
            break;
            case 8:
                searchForSectorDescription();
            break;
            case 9:
                generateReportOfInspectionsAverage();
            break;
            case 10:
                generateReportOfInspectionsVariation();
            break;
            default: break;
        }
}
void menuSectors(){
        int opt;
        printf("Planta: %s \nEscolha uma opção: \n", locations[location_selected_idx].name);
        printf("1. Criar setor \n");
        printf("2. Listar todos Setores \n");
        printf("3. Selecionar setor \n");
        printf("0. Voltar (Menu de plantas) \n");
        scanf("%i", &opt);
        getchar();
        actionMenuSectors(opt);
}
void actionMenuSectors(int option){
        switch (option)
            {
            case 1:
                if(locations[location_selected_idx].sectors_quantity == T_MAX_SECTORS){
                    printf("Erro: Número de setores máximo atigido. \n");
                    break;
                }
 
                int new_index_to_insert = 
                locations[location_selected_idx].sectors_quantity;
 
                t_sector new_sector = createNewSector();
                if(new_sector.id == NOT_FOUND){
                    printf("Erro: Erro na criação. \n");
                    break;
                }
 
                locations[location_selected_idx]
                .sectors[new_index_to_insert] = new_sector;
 
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
void menuSensors(){
        int opt;
        if(sensor_selected_idx == NOT_FOUND){
        printf("Planta: %s \nSetor: %s \nEscolha uma opção: \n", 
            locations[location_selected_idx].name,
            locations[location_selected_idx]
            .sectors[sector_selected_idx].name
        );
        printf("1. Criar sensor \n");
        printf("2. Listar sensores \n");
        printf("3. Selecionar sensor \n");
        printf("0. Voltar (Menu de setores) \n");
        scanf("%i", &opt);
        getchar();
        actionMenuSensors(opt);
        } else {
        printf("Planta: %s \nSetor: %s \nSensor: %s\nEscolha uma opção: \n", 
            locations[location_selected_idx].name,
            locations[location_selected_idx]
            .sectors[sector_selected_idx].name,
            locations[location_selected_idx]
            .sectors[sector_selected_idx]
            .sensors[sensor_selected_idx].name
        );
        printf("1. Gerar leitura no sensor \n");
        printf("2. Listar leituras do sensor \n");
        printf("0. Voltar (Menu de sensores) \n");
        scanf("%i", &opt);
        getchar();
        actionMenuSensorsInspection(opt);
        }
}
void actionMenuSensors(int option){
    switch (option)
        {
            case 1:
                if(locations[location_selected_idx].sectors[sector_selected_idx].sensors_quantity == T_MAX_SENSORS){
                    printf("Erro: Número de sensores máximo atigido. \n");
                    break;
                }
 
                int new_index_to_insert = 
                locations[location_selected_idx]
                .sectors[sector_selected_idx].sensors_quantity;
 
                t_sensor new_sensor = createNewSensor();
 
                if(new_sensor.id == NOT_FOUND){
                    printf("Erro: Erro na criação. \n");
                    break;
                }
 
                locations[location_selected_idx]
                .sectors[sector_selected_idx]
                .sensors[new_index_to_insert] = new_sensor;
 
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
void actionMenuSensorsInspection(int option){
    switch (option)
        {  
            case 1:
                if(locations[location_selected_idx]
                    .sectors[sector_selected_idx]
                    .sensors[sensor_selected_idx]
                    .inspections_quantity == T_MAX_INSPECTIONS){
                    printf("Erro: Número de leituras máximo atigido. \n");
                    break;
                }
 
                int new_index_to_insert = 
                locations[location_selected_idx]
                .sectors[sector_selected_idx]
                .sensors[sensor_selected_idx].inspections_quantity;
 
                t_inspection new_inspection = createNewInspection();
 
                if(new_inspection.id == NOT_FOUND){
                    printf("Erro: Erro na criação. \n");
                    break;
                }
 
                locations[location_selected_idx]
                .sectors[sector_selected_idx]
                .sensors[sensor_selected_idx]
                .inspections[new_index_to_insert] = new_inspection;
 
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
    printf("- Digite o nome: \n");
    fgets(new_location.name, T_STR, stdin);
    formatStringToSystemPattern(new_location.name);
    new_location.id = locations_quantity + 1;
    new_location.sectors_quantity = 0;
    printf("- Nova planta cadastrada com sucesso. \n");
    return new_location;
}
t_location findLocationStruct(int location_id){
    t_location location;
    location.id = NOT_FOUND;
    for(int i = 0; i < locations_quantity; i++){
        if(locations[i].id == location_id){
            location = locations[i];
            return location;
        }
    }
    return location;
}
void listAllLocations(void){
    if(locations_quantity < 1){
        printf("- Não existem plantas disponíveis. \n");
        return;
    }
    for(int i = 0; i < locations_quantity; i++){
        printf("ID: %i, Nome: %s. \n", locations[i].id, locations[i].name);
    }
    printf("- O número total de Plantas é: %i. \n", locations_quantity);
}
void selectLocation(void){
    if(locations_quantity < 1){
        printf("- Não existem plantas disponíveis. \n");
        return;
    }
    printf("Selecione uma planta (Location): \n");
    listAllLocations();
    printf("Digite o id da planta (Location): \n");
    int location_id;
    scanf("%i", &location_id);
    getchar();
    int locationIsValid = checkExistenceId(location_id, LOCATION);
    if(locationIsValid){
        resetStatesSelected(LOCATION);
        location_selected_idx = findLocationIdx(location_id);
    } else {
        printf("- Erro: O Id que você inseriu não corresponde a nenhuma planta existente. \n");
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
    if(idx == NOT_FOUND) printf("- Nenhuma location encontrada. \n");
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
    printf("- Novo setor (Sector) Criado com sucesso. \n");
    return new_sector;
}
t_sector findSectorStruct(int sector_id){
    t_sector sector;
    sector.id = NOT_FOUND;
    for(int i = 0; i < locations[location_selected_idx].sectors_quantity; i++){
        if(locations[location_selected_idx].sectors[i].id == sector_id){
            sector = locations[location_selected_idx].sectors[i];
            return sector;
        }
    }
    return sector;
}
void listAllSectorsFromLocation(){
    if(locations[location_selected_idx].sectors_quantity < 1){
        printf("- Não existem setores disponíveis. \n");
        return;
    }
 
    if(locations[location_selected_idx].id != NOT_FOUND) printf("Total de setores: %i. \n", locations[location_selected_idx].sectors_quantity);
 
    for(int i = 0; i < locations[location_selected_idx].sectors_quantity; i++){
        printf("ID: %i,  Nome do setor: %s. \n", locations[location_selected_idx].sectors[i].id, locations[location_selected_idx].sectors[i].name);
    }
}
void selectSector(void){
    if(locations[location_selected_idx].sectors_quantity < 1){
        printf("- Não existem setores disponíveis. \n");
        return;
    }
    printf("Selecione um setor (Sector): \n");
    listAllSectorsFromLocation();
    printf("Digite o id do setor (Sector): \n");
    int sector_id;
    scanf("%i", &sector_id);
    getchar();
    int sectorIsValid = checkExistenceId(sector_id, SECTOR);
    if(sectorIsValid){
        sector_selected_idx = findSectorIdx(sector_id);
    } else {
        printf("Erro: O Id que você inseriu não corresponde a nenhum setor existente. \n");
    }
}
int findSectorIdx(int sector_id){
    int idx = NOT_FOUND;
    if(location_selected_idx == NOT_FOUND){
        printf("- Impossível executar essa ação sem uma location selecionada. \n");
        return idx;
    }
    for(int i = 0; i < locations[location_selected_idx].sectors_quantity; i++){
        if(locations[location_selected_idx]
            .sectors[i].id == sector_id){
            idx = i;
            return idx;
        }
    }
    if(idx == NOT_FOUND) printf("- Nenhum sector encontrada. \n");
    return idx;
};

// - SENSORS FUNCTIONS
t_sensor createNewSensor(void){
    t_sensor new_sensor;
    if(locations[location_selected_idx].sectors[sector_selected_idx].sensors_quantity == T_MAX_SENSORS){
        new_sensor.id = NOT_FOUND;
        printf("Erro: No máximo 3 sensores por setor. \n");
        return new_sensor;
    }
    new_sensor.id = locations[location_selected_idx].sectors[sector_selected_idx].sensors_quantity + 1;
    new_sensor.sector_id = locations[location_selected_idx].sectors[sector_selected_idx].id;
    new_sensor.inspections_quantity = 0;
    printf("Digite o nome do sensor: \n");
    fgets(new_sensor.name, T_STR, stdin);
    formatStringToSystemPattern(new_sensor.name);
    printf("Digite o número correspondente ao tipo de sensor: \n");
    printf("0. TEMPERATURA - °C \n");
    printf("1. VIBRAÇÃO - mm/s \n");
    printf("2. PRESSÃO - PSI \n");
    printf("3. CORRENTE - A\n");
    printf("4. UMIDADE - %% \n");
    int number_typed;
    scanf("%i", &number_typed);
    getchar();
    int is_number_typed_valid = number_typed == 0 || number_typed == 1 || number_typed == 2 || number_typed == 3 || number_typed == 4;

    if(!is_number_typed_valid){
        new_sensor.id = NOT_FOUND;
        printf("Erro: você não digitou um número válido para o tipo do sensor. \n");
        return new_sensor;
    }

    new_sensor.sensor_type = number_typed;
    printf("Digite o range mínimo do sensor: \n");
    scanf("%f", &new_sensor.range_min); 
    getchar();
    printf("Digite o range máximo do sensor: \n");
    scanf("%f", &new_sensor.range_max); 
    getchar();

    printf("- Novo sensor (Sensor) Criado com sucesso. \n");
    return new_sensor;
}
t_sensor findSensorStruct(int sensor_id){
    t_sensor sensor;
    sensor.id = NOT_FOUND;
    for(int i = 0; i < locations[location_selected_idx].sectors[sector_selected_idx].sensors_quantity; i++){
        if(locations[location_selected_idx].sectors[sector_selected_idx].sensors[i].id == sensor_id){
            sensor = locations[location_selected_idx].sectors[sector_selected_idx].sensors[i];
            return sensor;
        }
    }
    return sensor;
}
void listAllSensorsFromSector(){
    if(locations[location_selected_idx].sectors[sector_selected_idx].sensors_quantity < 1){
        printf("- Não existem sensores disponíveis. \n");
        return;
    }
 
    if(locations[location_selected_idx].id && 
        locations[location_selected_idx].sectors[sector_selected_idx].id) printf("Total de sensors: %i. \n", locations[location_selected_idx].sectors[sector_selected_idx].sensors_quantity);
 
    for(int i = 0; i < locations[location_selected_idx].sectors[sector_selected_idx].sensors_quantity; i++){
        printf("ID: %i,  Nome do sensor: %s., tipo de sensor: %s, Faixa do sensor: %.2f - %.2f, Unidade de medida: %s. \n",
            locations[location_selected_idx]
            .sectors[sector_selected_idx]
            .sensors[i].id, 
 
            locations[location_selected_idx]
            .sectors[sector_selected_idx].sensors[i].name, 
 
            sensor_type_string[locations[location_selected_idx]
            .sectors[sector_selected_idx]
            .sensors[i].sensor_type],
 
            locations[location_selected_idx]
            .sectors[sector_selected_idx].sensors[i].range_min,
 
            locations[location_selected_idx]
            .sectors[sector_selected_idx].sensors[i].range_max,
 
            sensor_type_unit[locations[location_selected_idx]
            .sectors[sector_selected_idx]
            .sensors[i].sensor_type]
            );
    }
}
void selectSensor(void){
    if(locations[location_selected_idx].sectors[sector_selected_idx].sensors_quantity < 1){
        printf("- Não existem sensores disponíveis. \n");
        return;
    }
    printf("Selecione um sensor (Sensor): \n");
    listAllSensorsFromSector();
    printf("Digite o id do sensor (Sensor): \n");
    int sensor_id;
    scanf("%i", &sensor_id);
    getchar();
    int sensorIsValid = checkExistenceId(sensor_id, SENSOR);
    if(sensorIsValid){
        sensor_selected_idx = findSensorIdx(sensor_id);
    } else {
        printf("Erro: O Id que você inseriu não corresponde a nenhum sensor existente \n");
    } 
}
int findSensorIdx(int sensor_id){
    int idx = NOT_FOUND;
    if(location_selected_idx == NOT_FOUND ||
       sector_selected_idx == NOT_FOUND
    ){
        printf("- Impossível executar essa ação sem um setor selecionado. \n");
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
    if(idx == NOT_FOUND) printf("- Nenhum sensor encontrado. \n");
    return idx;
};

// - INSPECTIONS FUNCTIONS
t_inspection createNewInspection(void){
    t_sensor sensor = locations[location_selected_idx].sectors[sector_selected_idx].sensors[sensor_selected_idx];
    t_inspection new_inspection;
    new_inspection.id = locations[location_selected_idx].sectors[sector_selected_idx].sensors[sensor_selected_idx].inspections_quantity + 1;
    new_inspection.sensor_id = locations[location_selected_idx].sectors[sector_selected_idx].sensors[sensor_selected_idx].id;
 
    time_t date;
    time(&date); 
    new_inspection.date_inspection = date;
    int quantityOfInspectionsOnThisDay = checkQuantityOfInspectionsOnDate(new_inspection.date_inspection);
    if(quantityOfInspectionsOnThisDay >= T_MAX_INSPECTIONS_ON_SAME_DAY){
        printf("Erro: Você não pode gerar mais de duas leituras no mesmo dia. \n");
        new_inspection.id = NOT_FOUND;
        return new_inspection;
    }
 
    printf("Digite o valor da leitura (%.2f-%.2f): \n", sensor.range_min, sensor.range_max);
    scanf("%f", &new_inspection.value);
 
    if(
    new_inspection.value < locations[location_selected_idx].sectors[sector_selected_idx].sensors[sensor_selected_idx].range_min ||
    new_inspection.value > locations[location_selected_idx].sectors[sector_selected_idx].sensors[sensor_selected_idx].range_max
    ){
        printf("- Range mínimo (%.2f) e o Range máximo (%.2f).\n", 
            locations[location_selected_idx]
            .sectors[sector_selected_idx]
            .sensors[sensor_selected_idx].range_min,
            locations[location_selected_idx]
            .sectors[sector_selected_idx]
            .sensors[sensor_selected_idx].range_max);
 
        printf("Erro: Você precisa gerar um valor entre o Range mínimo e o range máximo. \n");
        new_inspection.id = NOT_FOUND;
        return new_inspection;
    }
 

    printf("- Leitura gerada com sucesso. \n");
    return new_inspection;
}
t_inspection findInspectionStruct(int inspection_id){
    t_inspection inspection;
    inspection.id = NOT_FOUND;
    for(int i = 0; i < locations[location_selected_idx].sectors[sector_selected_idx].sensors[sensor_selected_idx].inspections_quantity; i++){
        if(locations[location_selected_idx].sectors[sector_selected_idx].sensors[sensor_selected_idx].inspections[i].id == inspection_id){
            inspection = locations[location_selected_idx].sectors[sector_selected_idx].sensors[sensor_selected_idx].inspections[i];
            return inspection;
        }
    }
    return inspection;
}
void listAllInspectionsFromSensor(){
    if(locations[location_selected_idx].sectors[sector_selected_idx].sensors[sensor_selected_idx].inspections_quantity < 1){
        printf("- Não existem leituras disponíveis. \n");
        return;
    }
 
    if(locations[location_selected_idx].id 
        && locations[location_selected_idx].sectors[sector_selected_idx].id
        && locations[location_selected_idx].sectors[sector_selected_idx].sensors[sensor_selected_idx].id){
            printf("Total de Leituras: %i. \n", 
            locations[location_selected_idx].sectors[sector_selected_idx].sensors[sensor_selected_idx].inspections_quantity);
        }
 
    for(int i = 0; i < locations[location_selected_idx].sectors[sector_selected_idx].sensors[sensor_selected_idx].inspections_quantity; i++){
 
        t_date_string date_struct = convertTimestampToString(
            locations[location_selected_idx]
            .sectors[sector_selected_idx]
            .sensors[sensor_selected_idx]
            .inspections[i].date_inspection, true);
 
        printf("ID: %i,  Valor: %.2f %s. Data: %s. \n", 
            locations[location_selected_idx].sectors[sector_selected_idx].sensors[sensor_selected_idx].inspections[i].id, 
            locations[location_selected_idx].sectors[sector_selected_idx].sensors[sensor_selected_idx].inspections[i].value,
            sensor_type_unit[locations[location_selected_idx].sectors[sector_selected_idx].sensors[sensor_selected_idx].sensor_type],
            date_struct.date
        );
    }
}
void selectInspection(void){
    if(locations[location_selected_idx].sectors[sector_selected_idx].sensors[sensor_selected_idx].inspections_quantity < 1){
        printf("- Não existem leituras disponíveis. \n");
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
        printf("Erro: O Id que você inseriu não corresponde a nenhuma inspection existente. \n");
    }
}
int findInspectionIdx(int inspection_id){
    int idx = NOT_FOUND;
    if(location_selected_idx == NOT_FOUND ||
       sector_selected_idx == NOT_FOUND ||
       sensor_selected_idx == NOT_FOUND 
    ){
        printf("- Impossível executar essa ação sem um sensor selecionado. \n");
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
int checkQuantityOfInspectionsOnDate(time_t timestamp){
    int counter = 0;
    t_sensor sensor = locations[location_selected_idx].sectors[sector_selected_idx].sensors[sensor_selected_idx];
    for(int i = 0; i < sensor.inspections_quantity; i++){
        if(isSameDay(sensor.inspections[i].date_inspection, timestamp)){
            counter++;
        }
    }
    return counter;
}

// - REPORT AND SEARCH FUNCTIONS
void searchForSectorDescription(void){
    string description;
    printf("Digite uma descrição de um setor:\n");
    fgets(description, T_STR, stdin);
 
    formatStringToSystemPattern(description);
 
    for(int i = 0; i < locations_quantity; i++){
        for(int j = 0; j < locations[i].sectors_quantity; j++){
            if(strcmp(description, locations[i].sectors[j].description) == 0){
                printf("\n");
                printf("Resultado da busca: \n");
                printf("ID: %i\nNome: %s \nDescrição: %s \nQuantidade de sensores: %i\n", 
                    locations[i].sectors[j].id,
                    locations[i].sectors[j].name,
                    locations[i].sectors[j].description , 
                    locations[i].sectors[j].sensors_quantity 
                    );
                printf("\n");
            }
        }
    }
}
void searchForSensorType(void){
 
    int sensor_type_option;
 
    printf("Qual o tipo do sensor:\n");
    printf("0. TEMPERATURA\n");
    printf("1. VIBRAÇÃO\n");
    printf("2. PRESSÃO\n");
    printf("3. CORRENTE\n");
    printf("4. UMIDADE\n");
    printf("Digite a opção: \n");
 
    scanf("%i", &sensor_type_option);
    getchar();
 
    for(int i = 0; i < locations_quantity; i++){
        for(int j = 0; j < locations[i].sectors_quantity; j++){
            for(int k = 0; k < locations[i].sectors[j].sensors_quantity; k++){
                if( sensor_type_option == locations[i].sectors[j].sensors[k].sensor_type){
                    printf("\n");
                    printf("Resultado da busca: \n");
                    printf("ID: %i \nNome: %s \nTipo: %s \nMin/Max:[%.3f/%.3f] \nTotal de leituras: %i\n", 
                    locations[i].sectors[j].sensors[k].id,
                    locations[i].sectors[j].sensors[k].name,
                    sensor_type_string[locations[i].sectors[j].sensors[k].sensor_type], 
                    locations[i].sectors[j].sensors[k].range_min, locations[i].sectors[j].sensors[k].range_max,
                    locations[i].sectors[j].sensors[k].inspections_quantity
                    );
                    printf("\n");
                }
            }
        }
    }
}  
void generateReportOfInspections(void){
    int option;
    printf("Escolha uma opção: \n");
    printf("0. Por Todos os locais. \n");
    printf("1. Por local. \n");
    printf("Digite a opção: \n");
    scanf("%i", &option);
 
    int is_option_valid = option == 0 || option == 1;
    if(!is_option_valid){
        printf("Erro: Opção digitada inválida. \n");
        return;
    }
 
    if(option == 0){
        for(int i = 0; i < locations_quantity; i++){
            for(int j = 0; j < locations[i].sectors_quantity; j++){
                for(int k = 0; k < locations[i].sectors[j].sensors_quantity; k++){
                    for(int l = 0; l < locations[i].sectors[j].sensors[k].inspections_quantity; l++){
                            t_date_string date_struct = convertTimestampToString(locations[i].sectors[j].sensors[k].inspections[l].date_inspection, true);
                            printf("\n");
                            printf("Planta: %s\nSetor: %s\nSensor: %s\nValor da leitura: %.2f %s\nData: %s. \n",
                                locations[i].name,
                                locations[i].sectors[j].name,
                                locations[i].sectors[j].sensors[k].name,
                                locations[i].sectors[j].sensors[k].inspections[l].value,
                                sensor_type_unit[locations[i].sectors[j].sensors[k].sensor_type],
                                date_struct.date
                            );
                            printf("\n");

                        }
                    }
                }
        }
    } else {
        int option_location;
        printf("Escolha um local. \n");
        for(int i = 0; i < locations_quantity; i++){
            printf("%i. Nome: %s. \n", locations[i].id, locations[i].name);
        }
        printf("Digite a opção: \n");
        scanf("%i", &option_location);
 
        for(int i = 0; i < locations_quantity; i++){
                if(locations[i].id != option_location) continue;
                    for(int j = 0; j < locations[i].sectors_quantity; j++){
                        for(int k = 0; k < locations[i].sectors[j].sensors_quantity; k++){
                            for(int l = 0; l < locations[i].sectors[j].sensors[k].inspections_quantity; l++){
                                t_date_string date_struct = convertTimestampToString(locations[i].sectors[j].sensors[k].inspections[l].date_inspection, true);
                                printf("\n");
                                printf("Planta: %s\nSetor: %s\nSensor: %s\nValor da leitura: %.2f %s\nData: %s. \n",
                                locations[i].name,
                                locations[i].sectors[j].name,
                                locations[i].sectors[j].sensors[k].name,
                                locations[i].sectors[j].sensors[k].inspections[l].value,
                                sensor_type_unit[locations[i].sectors[j].sensors[k].sensor_type],
                                date_struct.date
                            );
                            printf("\n");
                        }
                    }
                }
        }
    }
}
void generateReportOfInspectionsVariation(void){
    int option;
    printf("Escolha um tipo de relatório: \n");
    printf("0. Por Setor\n");
    printf("1. Por Sensor\n");
    printf("Digite a opção: \n");
    scanf("%i", &option);

    int is_option_valid = option == 0 || option == 1;
    if(!is_option_valid){
        printf("Erro: Opção digitada inválida. \n");
        return;
    }
    
 
    if(option == 0){
        selectLocation();
        if(location_selected_idx == NOT_FOUND) return;
        printf("Planta: %s \n", locations[location_selected_idx].name);
        selectSector();
        if(sector_selected_idx == NOT_FOUND) return;
        printf("Setor: %s \n", locations[location_selected_idx].sectors[sector_selected_idx].name);

        for(int k = 0; k < locations[location_selected_idx].sectors[sector_selected_idx].sensors_quantity; k++){
                        t_sensor sensor = locations[location_selected_idx].sectors[sector_selected_idx].sensors[k];
                        for(int m = 0; m < sensor.inspections_quantity; m++){
                            for(int n = m + 1; n < sensor.inspections_quantity; n++){
                                if (isSameDay(
                                        sensor.inspections[m].date_inspection,
                                        sensor.inspections[n].date_inspection))
                                {
                                    t_inspection first_inspection_of_the_day;
                                    t_inspection second_inspection_of_the_day;
                                    t_date_string struct_date = convertTimestampToString(sensor.inspections[m].date_inspection, false);

                                    if(sensor.inspections[m].date_inspection <= sensor.inspections[n].date_inspection){
                                        first_inspection_of_the_day  = sensor.inspections[m];
                                        second_inspection_of_the_day = sensor.inspections[n];
                                    } else {
                                        first_inspection_of_the_day  = sensor.inspections[n];
                                        second_inspection_of_the_day = sensor.inspections[m];
                                    }

                                    float variation_between_values = second_inspection_of_the_day.value - first_inspection_of_the_day.value;

                                    printf("\n");
                                    
                                    printf("Planta: %s \n", 
                                    locations[location_selected_idx].name
                                    );

                                    printf("  Setor: %s \n", 
                                        locations[location_selected_idx].sectors[sector_selected_idx].name
                                    );
                                     printf("    Sensor: %s \n", 
                                        sensor.name
                                    );
                                    printf("      Data: %s -> Variação do sensor: %.2f %s.\n", 
                                        struct_date.date,
                                        variation_between_values,
                                        sensor_type_unit[sensor.sensor_type]
                                    );
                                    printf("\n");
                            }
                        }
                    }
                }
        resetStatesSelected(LOCATION);
    } else {
        selectLocation();
        printf("Planta: %s \n", locations[location_selected_idx].name);
        selectSector();
        if(sector_selected_idx == NOT_FOUND) return;
        printf("Setor: %s \n", locations[location_selected_idx].sectors[sector_selected_idx].name);
        selectSensor();
        if(sensor_selected_idx == NOT_FOUND) return;
        printf("Sensor: %s \n", locations[location_selected_idx].sectors[sector_selected_idx].sensors[sensor_selected_idx].name);

        t_sensor sensor = locations[location_selected_idx].sectors[sector_selected_idx].sensors[sensor_selected_idx];
        for(int m = 0; m < sensor.inspections_quantity; m++){
                for(int n = m + 1; n < sensor.inspections_quantity; n++){
                    if (isSameDay(
                        sensor.inspections[m].date_inspection,
                        sensor.inspections[n].date_inspection))
                    {
                    t_inspection first_inspection_of_the_day;
                    t_inspection second_inspection_of_the_day;
                    t_date_string struct_date = convertTimestampToString(sensor.inspections[m].date_inspection, false);

                    if(sensor.inspections[m].date_inspection <= sensor.inspections[n].date_inspection){
                        first_inspection_of_the_day  = sensor.inspections[m];
                        second_inspection_of_the_day = sensor.inspections[n];
                    } else {
                        first_inspection_of_the_day  = sensor.inspections[n];
                        second_inspection_of_the_day = sensor.inspections[m];
                    }

                    float variation_between_values = second_inspection_of_the_day.value - first_inspection_of_the_day.value;
                    printf("\n");

                    printf("Planta: %s \n", 
                        locations[location_selected_idx].name
                    );

                    printf("  Setor: %s \n", 
                        locations[location_selected_idx].sectors[sector_selected_idx].name
                    );
                    printf("    Sensor: %s \n", 
                        sensor.name
                    );
                    printf("      Data: %s -> Variação do sensor: %.2f %s.\n", 
                        struct_date.date,
                        variation_between_values,
                        sensor_type_unit[sensor.sensor_type]
                    );
                    printf("\n");
            }
        }
    }
        resetStatesSelected(LOCATION);
    }
};
void generateReportOfInspectionsAverage(void){
    for(int i = 0; i < locations_quantity; i++){
        for(int j = 0; j < locations[i].sectors_quantity; j++){
            for(int k = 0; k < locations[i].sectors[j].sensors_quantity; k++){
                float add_values = 0.0;
                for(int l = 0; l < locations[i].sectors[j].sensors[k].inspections_quantity; l++){
                    add_values = add_values +locations[i].sectors[j].sensors[k].inspections[l].value;
                }
                if( locations[i].sectors[j].sensors[k].inspections_quantity > 0){
                
                printf("\n");
                printf("Planta: %s \n", 
                locations[i].name
                );
                printf("  Setor: %s\n", 
                locations[i].sectors[j].name
                );
                printf("    Sensor: %s\n", 
                locations[i].sectors[j].sensors[k].name 
                );
                printf("       Média: %.4f %s. \n", 
                add_values / (float)locations[i].sectors[j].sensors[k].inspections_quantity,
                sensor_type_unit[locations[i].sectors[j].sensors[k].sensor_type]
                );
                printf("\n");

                }else{
                printf("- O sensor: %s ainda não possui leituras.\n", 
                locations[i].sectors[j].sensors[k].name);
                }
            }
        }
    }
};
void generateReportOfSensors(void){
 
    int option;
    int sensor_type_option;
    printf("Escolha um tipo de relatório: \n");
    printf("0. Geral\n");
    printf("1. Por tipo\n");
    printf("Digite a opção: \n");
    scanf("%i", &option);
    getchar();
 
    int is_option_valid = option == 0 || option == 1;
    if(!is_option_valid){
        printf("Erro: Opção digitada inválida. \n");
        return;
    }
 
    if(option == 0){
    for(int i = 0; i < locations_quantity; i++){
                for(int j = 0; j < locations[i].sectors_quantity; j++){
                    for(int k = 0; k < locations[i].sectors[j].sensors_quantity; k++){
                        printf("\n");

                        printf("ID: %i\nNome: %s \nTipo: %s \nMin/Max: [%.3f/%.3f] \nTotal de leituras: %i\n", 
                        locations[i].sectors[j].sensors[k].id,
                        locations[i].sectors[j].sensors[k].name,
                        sensor_type_string[locations[i].sectors[j].sensors[k].sensor_type], 
                        locations[i].sectors[j].sensors[k].range_min, locations[i].sectors[j].sensors[k].range_max,
                        locations[i].sectors[j].sensors[k].inspections_quantity
                        );

                        printf("\n");
                    }
                }
            }
        }else{
            printf("Qual o tipo do sensor:\n");
            printf("0. TEMPERATURA\n");
            printf("1. VIBRAÇÃO\n");
            printf("2. PRESSÃO\n");
            printf("3. CORRENTE\n");
            printf("4. UMIDADE\n");
            printf("Digite a opção: \n");
 
            scanf("%i", &sensor_type_option);
 
            for(int i = 0; i < locations_quantity; i++){
                for(int j = 0; j < locations[i].sectors_quantity; j++){
                    for(int k = 0; k < locations[i].sectors[j].sensors_quantity; k++){
                        if( sensor_type_option == locations[i].sectors[j].sensors[k].sensor_type){
                            printf("\n");

                            printf("ID: %i \n Nome: %s \n Tipo: %s \n Min/Max:[%.3f/%.3f] \n Total de leituras: %i\n", 
                                locations[i].sectors[j].sensors[k].id,
                                locations[i].sectors[j].sensors[k].name,
                                sensor_type_string[locations[i].sectors[j].sensors[k].sensor_type], 
                                locations[i].sectors[j].sensors[k].range_min, locations[i].sectors[j].sensors[k].range_max,
                                locations[i].sectors[j].sensors[k].inspections_quantity
                                );
                            printf("\n");
                        }
 
                    }
                }
            }
        }
}
void generateReportsOfSectors(void){
    for(int i = 0; i < locations_quantity; i++){
                for(int j = 0; j < locations[i].sectors_quantity; j++){
                    printf("\n");
                    printf("ID: %i\nNome: %s \nDescrição: %s \nQuantidade de sensores: %i \n", 
                    locations[i].sectors[j].id,
                    locations[i].sectors[j].name,
                    locations[i].sectors[j].description , 
                    locations[i].sectors[j].sensors_quantity 
                    );
                    printf("\n");                    
                }
            }
 
}

// - HELPER FUNCTIONS
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
    bool hasFoundId = false;
    switch(entity){
        case LOCATION: 
        {
            t_location location = findLocationStruct(id);
            if(location.id != NOT_FOUND){
                hasFoundId = true;
            }
            break;
        }
        case SECTOR:
        {
            t_sector sector = findSectorStruct(id);
            if(sector.id != NOT_FOUND){
                hasFoundId = true;
            }
            break;
        }
        case SENSOR:
        {
            t_sensor sensor = findSensorStruct(id);
            if(sensor.id != NOT_FOUND){
                hasFoundId = true;
            }
            break;
        }
        case INSPECTION:
        {
            t_inspection inspection = findInspectionStruct(id);
            if(inspection.id != NOT_FOUND){
                hasFoundId = true;
            }
            break;
        }
    }
    return hasFoundId;
}
void removeEnterFromString(string str){
    if(strlen(str) < 1) return;
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
t_date_string convertTimestampToString(time_t timestamp, bool enable_hours){
    t_date_string struct_date;
    struct tm* tm_info = localtime(&timestamp);
    char date_str[50];
    if(enable_hours){
        strftime(date_str, sizeof(date_str), "%d/%m/%Y %H:%M:%S", tm_info);
    } else {
        strftime(date_str, sizeof(date_str), "%d/%m/%Y", tm_info);
    }
    strcpy(struct_date.date, date_str);
    return struct_date;
}
bool isSameDay(time_t firstTimestamp, time_t secondTimestamp){
    struct tm *date_a = localtime(&firstTimestamp);
    struct tm date_a_copy = *date_a;
    struct tm *date_b = localtime(&secondTimestamp);
 
    return date_a_copy.tm_mday == date_b->tm_mday &&
           date_a_copy.tm_mon  == date_b->tm_mon  &&
           date_a_copy.tm_year == date_b->tm_year;
}
void generateDataAndFulfillForTest(void){
    locations_quantity = 1;

    locations[0].id = 1;
    strcpy(locations[0].name, "PLANTA SAO PAULO");
    locations[0].sectors_quantity = 2;

    locations[0].sectors[0].id = 1;
    locations[0].sectors[0].location_id = 1;
    strcpy(locations[0].sectors[0].name, "PRODUCAO");
    strcpy(locations[0].sectors[0].description, "AREA PRINCIPAL DE PRODUCAO");
    locations[0].sectors[0].sensors_quantity = 2;

    locations[0].sectors[0].sensors[0].id = 1;
    locations[0].sectors[0].sensors[0].sector_id = 1;
    strcpy(locations[0].sectors[0].sensors[0].name, "SENSOR TEMPERATURA FORNO");
    locations[0].sectors[0].sensors[0].sensor_type = TEMPERATURE;
    locations[0].sectors[0].sensors[0].range_min = 100.0;
    locations[0].sectors[0].sensors[0].range_max = 500.0;
    locations[0].sectors[0].sensors[0].inspections_quantity = 4;

    locations[0].sectors[0].sensors[0].inspections[0].id = 1;
    locations[0].sectors[0].sensors[0].inspections[0].sensor_id = 1;
    locations[0].sectors[0].sensors[0].inspections[0].value = 320.5;
    locations[0].sectors[0].sensors[0].inspections[0].date_inspection = time(NULL) - 86400 - 3600;

    locations[0].sectors[0].sensors[0].inspections[1].id = 2;
    locations[0].sectors[0].sensors[0].inspections[1].sensor_id = 1;
    locations[0].sectors[0].sensors[0].inspections[1].value = 410.0;
    locations[0].sectors[0].sensors[0].inspections[1].date_inspection = time(NULL) - 86400 - 1800;

    locations[0].sectors[0].sensors[0].inspections[2].id = 3;
    locations[0].sectors[0].sensors[0].inspections[2].sensor_id = 1;
    locations[0].sectors[0].sensors[0].inspections[2].value = 280.0;
    locations[0].sectors[0].sensors[0].inspections[2].date_inspection = time(NULL) - 7200;

    locations[0].sectors[0].sensors[0].inspections[3].id = 4;
    locations[0].sectors[0].sensors[0].inspections[3].sensor_id = 1;
    locations[0].sectors[0].sensors[0].inspections[3].value = 460.0;
    locations[0].sectors[0].sensors[0].inspections[3].date_inspection = time(NULL) - 3600;

    locations[0].sectors[0].sensors[1].id = 2;
    locations[0].sectors[0].sensors[1].sector_id = 1;
    strcpy(locations[0].sectors[0].sensors[1].name, "SENSOR VIBRACAO ESTEIRA");
    locations[0].sectors[0].sensors[1].sensor_type = VIBRATION;
    locations[0].sectors[0].sensors[1].range_min = 0.0;
    locations[0].sectors[0].sensors[1].range_max = 50.0;
    locations[0].sectors[0].sensors[1].inspections_quantity = 2;

    locations[0].sectors[0].sensors[1].inspections[0].id = 1;
    locations[0].sectors[0].sensors[1].inspections[0].sensor_id = 2;
    locations[0].sectors[0].sensors[1].inspections[0].value = 11.0;
    locations[0].sectors[0].sensors[1].inspections[0].date_inspection = time(NULL) - 5400;

    locations[0].sectors[0].sensors[1].inspections[1].id = 2;
    locations[0].sectors[0].sensors[1].inspections[1].sensor_id = 2;
    locations[0].sectors[0].sensors[1].inspections[1].value = 14.5;
    locations[0].sectors[0].sensors[1].inspections[1].date_inspection = time(NULL) - 1800;

    locations[0].sectors[1].id = 2;
    locations[0].sectors[1].location_id = 1;
    strcpy(locations[0].sectors[1].name, "UTILIDADES");
    strcpy(locations[0].sectors[1].description, "AREA DE VAPOR E AR COMPRIMIDO");
    locations[0].sectors[1].sensors_quantity = 1;

    locations[0].sectors[1].sensors[0].id = 3;
    locations[0].sectors[1].sensors[0].sector_id = 2;
    strcpy(locations[0].sectors[1].sensors[0].name, "SENSOR PRESSAO CALDEIRA");
    locations[0].sectors[1].sensors[0].sensor_type = PRESSURE;
    locations[0].sectors[1].sensors[0].range_min = 5.0;
    locations[0].sectors[1].sensors[0].range_max = 150.0;
    locations[0].sectors[1].sensors[0].inspections_quantity = 2;

    locations[0].sectors[1].sensors[0].inspections[0].id = 1;
    locations[0].sectors[1].sensors[0].inspections[0].sensor_id = 3;
    locations[0].sectors[1].sensors[0].inspections[0].value = 40.0;
    locations[0].sectors[1].sensors[0].inspections[0].date_inspection = time(NULL) - 7200;

    locations[0].sectors[1].sensors[0].inspections[1].id = 2;
    locations[0].sectors[1].sensors[0].inspections[1].sensor_id = 3;
    locations[0].sectors[1].sensors[0].inspections[1].value = 130.0;
    locations[0].sectors[1].sensors[0].inspections[1].date_inspection = time(NULL) - 1800;
}