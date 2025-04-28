#include <stdio.h>
#include "../include/menu.h"
#include "../include/combo.h"

void title() {
    printf("\033[H\033[J");
    printf("Sistema de gestión de combos.\n");
    printf(SEP);
}

void statusMenuStr() {
    title();
    printf("Ordenes por estado:\n");
    printf("1. Pendientes\n");
    printf("2. En Proceso\n");
    printf("3. Terminado\n");
    printf(SEP);
    printf("4. Todas las ordenes\n");
    printf(SEP);
    printf("5. Volver\n");
    printf(SEP);
    printf("Su opción: ");
}

void comboMenuStr() {
    title();
    printf("Elegir un combo:\n");
    printf("1. Combo Simple\n");
    printf("2. Combo Doble\n");
    printf("3. Combo Completo\n");
    printf(SEP);
    printf("4. Volver\n");
    printf(SEP);
    printf("Su opción: ");
}

void mainMenuStr() {
    title();
    printf("1. Crear Combo\n");
    printf("2. Mostrar Combos\n");
    printf(SEP);
    printf("3. Salir\n");
    printf(SEP);
    printf("Su opción: ");
}


void showCreateComboMenu() {
    comboMenuStr();
    int option;
    scanf("%d", &option);
    
    if (option == 4) {
        printf("Volviendo al menú principal...\n");
        return;
    }

    CreateComboOrder(option);
}

void showStatusMenu() {
    statusMenuStr();
    int option;
    scanf("%d", &option);
    
    if (option == 5) {
        printf("Volviendo al menú principal...\n");
        return;
    }

    OrdersReport(option);
}

int RenderMenu() {
    title();
    mainMenuStr();
    int option;
    scanf("%d", &option);
    
    switch (option) {
        case 1:
            showCreateComboMenu();
            break;
        case 2:
            showStatusMenu();
            break;
        case 3:
            printf("Saliendo del programa.\n");
            return -1;
            break;
        default:
            printf("Opción no válida.\n");
            break;
    }

    return 1;
}

void WaitForUserInput() {
    printf("Presione Enter para continuar...");
    getchar(); // Clear the input buffer
    getchar(); // Wait for user to press Enter
}



