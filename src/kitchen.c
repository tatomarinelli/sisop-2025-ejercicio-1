#include <stdio.h>
#include <unistd.h> // For sleep()
#include <pthread.h>
#include "../include/combo.h"

// External variables from combo.c
extern Combo combos[];
extern int comboCount;
extern pthread_mutex_t comboMutex;

void* ProcessOrders() {
    while (1) {
        pthread_mutex_lock(&comboMutex);
        if (!IsQueueEmpty()) {
            Combo combo;
            if (DequeueCombo(&combo) == 0) {
                pthread_mutex_unlock(&comboMutex);

                printf("Procesando orden ID: %d | Tipo: %s\n", combo.id, GetComboTypeString(combo.type));
                sleep(5);
                printf("Orden ID: %d completada.\n", combo.id);

                pthread_mutex_lock(&comboMutex);
                combo.status = TERMINADO;
                LogCombo(&combo);
                pthread_mutex_unlock(&comboMutex);
            }
        } else {
            pthread_mutex_unlock(&comboMutex);
            sleep(1); // Wait before checking again
        }
    }
    return NULL;
}

void StartKitchenThread() {
    pthread_t kitchenThread;

    if (pthread_create(&kitchenThread, NULL, ProcessOrders, NULL) != 0) {
        perror("Error al crear el hilo de la cocina");
        return;
    }

    printf("Hilo de cocina iniciado.\n");

    // Optionally, wait for the thread to finish (if needed)
    // pthread_join(kitchenThread, NULL);
}

