#include <stdio.h>
#include "../include/menu.h"
#include "../include/kitchen.h"

void InitializeLogFile() {
    remove("orders.log"); // Remove the file if it exists
}

int main()
{
    InitializeLogFile();
    StartKitchenThread();
    WaitForUserInput();

    //int run = 1;
    while (RenderMenu())
    {
    }
    return 0;
}

