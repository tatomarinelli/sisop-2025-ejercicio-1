#include <pthread.h>

#define MAX_COMBOS 100

typedef enum {
    SIMPLE, 
    DOBLE,
    COMPLETO
} ComboType;

typedef enum {
    PENDIENTE,
    EN_PROCESO,
    TERMINADO
} ComboStatus;


typedef struct {
    int meatAndCheese;
    int lettuce;
    int tomato;
    int bread;
} Ingredients;

typedef struct {
    int id;
    ComboType type;
    ComboStatus status;
    int preparationTime;
    Ingredients ingredients;
} Combo;

#define QUEUE_MAX 10 // Maximum number of combos in the queue

typedef struct {
    Combo queue[QUEUE_MAX];
    int front; // Index of the front element
    int rear;  // Index of the rear element
    int size;  // Current size of the queue
} ComboQueue;

extern ComboQueue comboQueue; // Properly initialize all fields

extern Combo combos[MAX_COMBOS];
extern int comboCount;
extern pthread_mutex_t comboMutex;

void CreateComboOrder(int id);
void OrdersReport(int option);

int EnqueueCombo(Combo* combo);
int DequeueCombo(Combo* combo);
int IsQueueEmpty();
int IsQueueFull();

const char* GetComboTypeString(ComboType type);
const char* GetComboStatusString(ComboStatus status);