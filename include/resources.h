#include <pthread.h>
#include "kitchen.h"

typedef struct {
    Ingredients ingredients;
    pthread_mutex_t mutex;
} SharedResources;

SharedResources resources;

int GetIngredientsForCombo(Combo* combo);
void* ResourceMonitor();
void ResourceMonitorReport();