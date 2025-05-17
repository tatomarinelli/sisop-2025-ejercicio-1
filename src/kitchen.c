#include <stdio.h>
#include <unistd.h> // For sleep()
#include <pthread.h>
#include "../include/resources.h"
#include <sys/syscall.h> 

// External variables from combo.c
//extern Combo combos[];
//extern int comboCount;
//extern pthread_mutex_t comboMutex;

__thread pid_t tid;

// Barrier for thread synchronization
pthread_mutex_t barrierMutex = PTHREAD_MUTEX_INITIALIZER;
int barrierCount = 0; // Number of threads that have reached the barrier


void* ProcessOrders() {
    tid = syscall(SYS_gettid); // Get the kernel-level TID
    printf("Hilo cocinero iniciado - TID: %d\n", tid);

    // Wait for all threads to reach this point
    pthread_mutex_lock(&barrierMutex);
    barrierCount++;
    pthread_mutex_unlock(&barrierMutex);


    while (1) {
        pthread_mutex_lock(&comboMutex);
        if (!IsQueueEmpty()) {
            Combo combo;
            if (DequeueCombo(&combo) == 0) {
                combo.status = EN_PROCESO;
                pthread_mutex_unlock(&comboMutex);
                
                while (GetIngredientsForCombo(&combo) != 1) {
                    printf("Esperando ingredientes para la orden ID: %d\n", combo.id);
                    sleep(1); // Wait before checking again
                }

                //printf("Procesando orden ID: %d | Tipo: %s\n", combo.id, GetComboTypeString(combo.type));
                sleep(combo.preparationTime);
                //printf("Orden ID: %d completada.\n", combo.id);

                pthread_mutex_lock(&comboMutex);
                combo.status = TERMINADO;
                logWork(&combo);
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
    pthread_t resourceMonitorThread;
    if (pthread_create(&resourceMonitorThread, NULL, ResourceMonitor, NULL) != 0) {
        perror("Error al crear el hilo de monitoreo de recursos");
        return;
    }

    pthread_t kitchenThreads[5];
    for (int i = 0; i < 5; i++) {
        if (pthread_create(&kitchenThreads[i], NULL, ProcessOrders, NULL) != 0) {
            perror("Error al crear el hilo de la cocina");
            return;
        }
    }

    while(barrierCount < 5) {}

    printf("\nHilos cocineros iniciados.\n");
}


void logWork(Combo *combo) {
    FILE *logFile = fopen("orders.log", "a"); // Open the log file in append mode
    if (logFile != NULL) {
        fprintf(logFile, "[TID %d] - Combo - ID: %d, Tipo: %s, Estado: %s\n",
                tid,
                combo->id,
                GetComboTypeString(combo->type),
                GetComboStatusString(combo->status));
        fclose(logFile); // Close the file
    } else {
        printf("Error al abrir el archivo de registro.\n");
    }
}

