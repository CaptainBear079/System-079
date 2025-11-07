#include "MSG.h"
#include "LIF_Neurons.h"
#include "DSF.h"
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NEURONS 1024
#define LOWEST_WEIGHT -1.0f
#define HIGHEST_WEIGHT 1.0f

typedef struct _VARIANTS_ {
    int Count;
    char** Variants;
} VARIANTS;

int MakeTrainingsData();

int Bot();
