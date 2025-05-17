#include "../include/resources.h"
#include <stdio.h>
#include <unistd.h> // For sleep()


void Init() {
    resources.ingredients.meatAndCheese = 7; // Initial value
    resources.ingredients.lettuce = 7;      // Initial value
    resources.ingredients.tomato = 7;       // Initial value
    resources.ingredients.bread = 7;        // Initial value
    pthread_mutex_init(&resources.mutex, NULL);
}

void* ResourceMonitor() {
    Init();
    while (1) {
        pthread_mutex_lock(&resources.mutex);
        if (resources.ingredients.meatAndCheese < 5) {
            sleep(2);
            resources.ingredients.meatAndCheese = 10;
        }
        if (resources.ingredients.bread < 5) {
            sleep(2);
            resources.ingredients.bread = 10;
        }
        if (resources.ingredients.lettuce < 5) {
            sleep(2);
            resources.ingredients.lettuce = 10;
        }
        if (resources.ingredients.tomato < 5) {
            sleep(2);
            resources.ingredients.tomato = 10;
        }

        pthread_mutex_unlock(&resources.mutex);
        // Sleep for a while to avoid busy-waiting
        sleep(1);
    }
    return NULL;
}

int getTomato(int quantity) {
    pthread_mutex_lock(&resources.mutex);
    if (resources.ingredients.tomato >= quantity) {
        resources.ingredients.tomato -= quantity;
        pthread_mutex_unlock(&resources.mutex);
        return quantity;
    }
    pthread_mutex_unlock(&resources.mutex);
    return -1;
}

int getLettuce(int quantity) {
    pthread_mutex_lock(&resources.mutex);
    if (resources.ingredients.lettuce >= quantity) {
        resources.ingredients.lettuce -= quantity;
        pthread_mutex_unlock(&resources.mutex);
        return quantity;
    }
    pthread_mutex_unlock(&resources.mutex);
    return -1;
}
int getMeatAndCheese(int quantity) {
    pthread_mutex_lock(&resources.mutex);
    if (resources.ingredients.meatAndCheese >= quantity) {
        resources.ingredients.meatAndCheese -= quantity;
        pthread_mutex_unlock(&resources.mutex);
        return quantity;
    }
    pthread_mutex_unlock(&resources.mutex);
    return -1;
}
int getBread(int quantity) {
    pthread_mutex_lock(&resources.mutex);
    if (resources.ingredients.bread >= quantity) {
        resources.ingredients.bread -= quantity;
        pthread_mutex_unlock(&resources.mutex);
        return quantity;
    }
    pthread_mutex_unlock(&resources.mutex);
    return -1;
}

int GetIngredientsForCombo(Combo* combo) {
    while (getTomato(combo->ingredients.tomato) == -1) {
        //printf("Esperando tomate...\n");
        sleep(1);
    }
    while(getLettuce(combo->ingredients.lettuce) == -1) {
        //printf("Esperando lechuga...\n");
        sleep(1);
    }
    while (getMeatAndCheese(combo->ingredients.meatAndCheese) == -1) {
        //printf("Esperando carne y queso...\n");
        sleep(1);
    }
    while (getBread(combo->ingredients.bread) == -1) {
        //printf("Esperando pan...\n");
        sleep(1);
    }

    return 1;
}

void ResourceMonitorReport() {
    char input;
    do {
        sleep(0.5);
        printf("\033[H\033[J");
        printf("=== Recursos Disponibles ===\n");
        printf("Carne y Queso: %d\n", resources.ingredients.meatAndCheese);
        printf("Lechuga: %d\n", resources.ingredients.lettuce);
        printf("Tomate: %d\n", resources.ingredients.tomato);
        printf("Pan: %d\n", resources.ingredients.bread);
        printf("============================\n");
        printf("Presione 'q' para volver al menu principal o cualquier tecla para refrescar\n");
        input = getchar();
    } while (input != 'q');
    
}