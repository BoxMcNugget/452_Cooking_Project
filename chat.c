#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>

//----------------------RECIPES-----------------------------
struct Recipe {
    const char* name;
    bool pantry[6];    // Flour, Sugar, Yeast, Baking Soda, Salt, Cinnamon
    bool fridge[3];    // Eggs, Milk, Butter
};

struct Recipe recipes[] = {
    {"Cookies", {true, true, false, false, false, false}, {false, true, true}},
    {"Pancakes", {true, true, false, true, true, false}, {true, true, true}},
    {"Homemade Pizza Dough", {false, true, true, false, false, false}, {false, false, false}},
    {"Soft Pretzels", {true, true, true, false, false, false}, {true, false, false}},
    {"Cinnamon Rolls", {true, true, true, false, false, true}, {true, false, true}}
};

//------------------SHARED MEMORY STUFF----------------------
sem_t semPantry, semFridge, semMixer, semOven, semBowl, semSpoon;
pthread_mutex_t ramsiedLock = PTHREAD_MUTEX_INITIALIZER;

// Baker thread function
void* baker(void* arg) {
    int id = *(int*)arg;
    int numRecipes = sizeof(recipes) / sizeof(struct Recipe);

    printf("\033[%dmBaker %d: Starting work.\033[0m\n", 31 + id % 7, id); // Colored output

    for (int r = 0; r < numRecipes; r++) {
        printf("\033[%dmBaker %d: Starting %s.\033[0m\n", 31 + id % 7, id, recipes[r].name);

        // Pantry access
        sem_wait(&semPantry);
        printf("\033[%dmBaker %d: Accessing the pantry.\033[0m\n", 31 + id % 7, id);
        for (int i = 0; i < 6; i++) {
            if (recipes[r].pantry[i]) {
                printf("\033[%dmBaker %d: Taking pantry ingredient %d.\033[0m\n", 31 + id % 7, id, i + 1);
            }
        }
        sem_post(&semPantry);

        // Fridge access
        sem_wait(&semFridge);
        printf("\033[%dmBaker %d: Accessing the fridge.\033[0m\n", 31 + id % 7, id);
        for (int i = 0; i < 3; i++) {
            if (recipes[r].fridge[i]) {
                printf("\033[%dmBaker %d: Taking fridge ingredient %d.\033[0m\n", 31 + id % 7, id, i + 1);
            }
        }
        sem_post(&semFridge);

        // Mixing
        sem_wait(&semBowl);
        sem_wait(&semSpoon);
        sem_wait(&semMixer);
        printf("\033[%dmBaker %d: Mixing ingredients for %s.\033[0m\n", 31 + id % 7, id, recipes[r].name);
        sem_post(&semMixer);
        sem_post(&semSpoon);
        sem_post(&semBowl);

        // Baking
        sem_wait(&semOven);
        printf("\033[%dmBaker %d: Baking %s.\033[0m\n", 31 + id % 7, id, recipes[r].name);
        sem_post(&semOven);

        printf("\033[%dmBaker %d: Finished %s.\033[0m\n", 31 + id % 7, id, recipes[r].name);

        // Ramsied scenario
        if (rand() % 10 == 0) { // 10% chance of being Ramsied
            pthread_mutex_lock(&ramsiedLock);
            printf("\033[1;31mBaker %d: Got Ramsied! Restarting %s.\033[0m\n", id, recipes[r].name);
            pthread_mutex_unlock(&ramsiedLock);
            r--; // Restart the current recipe
        }
    }

    printf("\033[%dmBaker %d: Finished all recipes!\033[0m\n", 31 + id % 7, id);
    pthread_exit(NULL);
}

//-------------------------MAIN------------------------------
int main() {
    int numBakers;

    printf("Enter the number of bakers: ");
    scanf("%d", &numBakers);

    // Initialize semaphores
    sem_init(&semPantry, 0, 1);
    sem_init(&semFridge, 0, 2);
    sem_init(&semMixer, 0, 2);
    sem_init(&semOven, 0, 1);
    sem_init(&semBowl, 0, 3);
    sem_init(&semSpoon, 0, 5);

    pthread_t bakerIds[numBakers];
    int bakerArgs[numBakers];

    // Create baker threads
    for (int i = 0; i < numBakers; i++) {
        bakerArgs[i] = i + 1;
        pthread_create(&bakerIds[i], NULL, baker, &bakerArgs[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < numBakers; i++) {
        pthread_join(bakerIds[i], NULL);
    }

    // Destroy semaphores
    sem_destroy(&semPantry);
    sem_destroy(&semFridge);
    sem_destroy(&semMixer);
    sem_destroy(&semOven);
    sem_destroy(&semBowl);
    sem_destroy(&semSpoon);

    printf("All bakers have finished their work!\n");
    return 0;
}

