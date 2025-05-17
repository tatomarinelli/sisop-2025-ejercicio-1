#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../include/combo.h"
#include "../include/menu.h"



//Combo combos[MAX_COMBOS]; // Define the shared array of combos
int comboCount = 0;       // Define the shared counter for the number of combos
pthread_mutex_t comboMutex = PTHREAD_MUTEX_INITIALIZER; // Define the mutex for thread safety

int nextComboID = 1; // Global counter for generating unique IDs
int indexPool[MAX_COMBOS]; // Pool of reusable IDs
int indexPoolCount = 0;    // Number of IDs in the pool

ComboQueue comboQueue = {{{0}}, 0, -1, 0};

int EnqueueCombo(Combo* combo) {
    if (comboQueue.size == QUEUE_MAX) {
        printf("La cola de combos está llena. No se puede agregar más órdenes.\n");
        return -1; // Queue is full
    }

    comboQueue.rear = (comboQueue.rear + 1) % QUEUE_MAX; // Move rear to the next position
    comboQueue.queue[comboQueue.rear] = *combo; // Add the combo to the queue
    comboQueue.size++; // Increment the size
    return 0; // Success
}

int DequeueCombo(Combo *combo) {
    if (comboQueue.size == 0) {
        printf("La cola de combos está vacía. No hay órdenes para procesar.\n");
        return -1; // Queue is empty
    }

    *combo = comboQueue.queue[comboQueue.front]; // Get the front combo
    comboQueue.front = (comboQueue.front + 1) % QUEUE_MAX; // Move front to the next position
    comboQueue.size--; // Decrement the size

   
    return 0; // Success
}

int IsQueueEmpty() {
    return comboQueue.size == 0;
}

int IsQueueFull() {
    return comboQueue.size == QUEUE_MAX;
}

void ReportPendingCombos() {
    pthread_mutex_lock(&comboMutex);
    printf("=== Reporte de Combos Pendientes ===\n");
    for (int i = 0; i < comboQueue.size; i++) {
        int index = (comboQueue.front + i) % QUEUE_MAX;
        if (comboQueue.queue[index].status == PENDIENTE) {
            printf("ID: %d | Tipo: %s\n", comboQueue.queue[index].id, GetComboTypeString(comboQueue.queue[index].type));
        }
    }
    pthread_mutex_unlock(&comboMutex);
    printf("====================\n");
}

void ReportCombosByStatus(ComboStatus status) {
    pthread_mutex_lock(&comboMutex);
    printf("=== Reporte de Combos %s ===\n", GetComboStatusString(status));
    for (int i = 0; i < QUEUE_MAX; i++) {
        if (comboQueue.queue[i].id != 0 && ((int)status == -1 || comboQueue.queue[i].status == status)) {
            printf("ID: %d | Tipo: %s | Estado: %s\n", comboQueue.queue[i].id, GetComboTypeString(comboQueue.queue[i].type), GetComboStatusString(comboQueue.queue[i].status));
        }
    }
    pthread_mutex_unlock(&comboMutex);
    printf("====================\n");
}

void CreateComboOrder(int id) {
    if (id < 1 || id > 3) {
        printf("ID de combo no válido.\n");
        return;
    }

    Combo *combo = (Combo *)malloc(sizeof(Combo));
    if (combo == 0) {
        printf("Error al asignar memoria para el combo.\n");
        return;
    }

    switch (id) {
        case 1:
            combo->type = SIMPLE;
            combo->preparationTime = 5;
            combo->ingredients.meatAndCheese = 1;
            combo->ingredients.lettuce = 0;
            combo->ingredients.tomato = 0;
            combo->ingredients.bread = 2;
            break;
        case 2:
            combo->type = DOBLE;
            combo->preparationTime = 7;
            combo->ingredients.meatAndCheese = 2;
            combo->ingredients.lettuce = 0;
            combo->ingredients.tomato = 0;
            combo->ingredients.bread = 2;
            break;
        case 3:
            combo->type = COMPLETO;
            combo->preparationTime = 1;
            combo->ingredients.meatAndCheese = 2;
            combo->ingredients.lettuce = 1;
            combo->ingredients.tomato = 1;
            combo->ingredients.bread = 2;
            break;
        default:
            printf("Opción no válida.\n");
            free(combo);
            return;
    }

    combo->status = PENDIENTE;

    pthread_mutex_lock(&comboMutex);
    // Reuse an ID from the pool if available
    if (indexPoolCount > 0) {
    combo->id = indexPool[--indexPoolCount];
    } else {
        combo->id = nextComboID++;
    }

    if (EnqueueCombo(combo) == 0){
        printf("\n=== Orden Creada ===\n");
        printf("ID: %d | Tipo: %s | Estado: %s\n", combo->id, GetComboTypeString(combo->type), GetComboStatusString(combo->status));
        printf("====================\n\n");
        sleep(0.2);
        //printf("Presione Enter para continuar...\n");
        //getchar();
        //getchar();
    }
    
    pthread_mutex_unlock(&comboMutex);

    free(combo);
}

void OrdersReport(int option)
{
    MENU_CLEAN_SCREEN
    switch (option) {
        case 1:
            ReportCombosByStatus(PENDIENTE);
            break;
        case 2:
            ReportCombosByStatus(EN_PROCESO);
            break;
        case 3:
            printf("=== Combos Terminados ===\n");

            /*for (int i = 0; i < comboCount; i++) {
                if (combos[i].status == TERMINADO) {
                    printf("ID: %d | Tipo: %s\n", combos[i].id, GetComboTypeString(combos[i].type));
                }
            }*/
            break;
        case 4:
            ReportCombosByStatus(-1);
            break;
        default:
            printf("Opción no válida.\n");
            break;
    }

    WaitForUserInput();
}

const char* GetComboTypeString(ComboType type) {
    switch (type) {
        case SIMPLE: return "Simple";
        case DOBLE: return "Doble";
        case COMPLETO: return "Completo";
        default: return "Desconocido";
    }
}

const char* GetComboStatusString(ComboStatus status) {
    switch (status) {
        case PENDIENTE: return "Pendiente";
        case EN_PROCESO: return "En Proceso";
        case TERMINADO: return "Terminado";
        default: return "Todos";
    }
}



