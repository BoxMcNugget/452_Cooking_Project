#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> // For usleep()

//----------------------RECIPES-----------------------------
struct Recipe {
    const char* name;
    bool pantry[6];    // Flour, Sugar, Yeast, Baking Soda, Salt, Cinnamon
    bool fridge[3];    // Eggs, Milk, Butter
    int bakingTime;    // Time to "bake" the recipe
};

struct Recipe recipes[] = {
    {"Cookies", {true, true, false, false, false, false}, {false, true, true}, 1500},
    {"Pancakes", {true, true, false, true, true, false}, {true, true, true}, 1200},
    {"Homemade Pizza Dough", {false, true, true, false, false, false}, {false, false, false}, 1800},
    {"Soft Pretzels", {true, true, true, false, false, false}, {true, false, false}, 2000},
    {"Cinnamon Rolls", {true, true, true, false, false, true}, {true, false, true}, 2500}
};

//------------------SHARED MEMORY STUFF----------------------
sem_t semPantry, semFridge, semMixer, semOven, semBowl, semSpoon;
pthread_mutex_t recipeLock = PTHREAD_MUTEX_INITIALIZER;

int numRecipes = sizeof(recipes) / sizeof(struct Recipe);
int currentRecipe = 0; // Index of the next recipe to assign

// Function to simulate task duration
void simulate_task(const char* task, int bakerId, const char* recipe, int durationMs) {
    printf("\033[%dmBaker %d: %s %s (will take %d ms).\033[0m\n", 
           31 + bakerId % 7, bakerId, task, recipe, durationMs);
    usleep(durationMs * 1000); // Convert milliseconds to microseconds
}

// Function to get the next recipe
int get_next_recipe() {
    pthread_mutex_lock(&recipeLock);
    if (currentRecipe >= numRecipes) {
        pthread_mutex_unlock(&recipeLock);
        return -1; // No more recipes
    }
    int recipeIndex = currentRecipe++;
    pthread_mutex_unlock(&recipeLock);
    return recipeIndex;
}

// Baker thread function
void* baker(void* arg) {
    int id = *(int*)arg;

    printf("\033[%dmBaker %d: Starting work.\033[0m\n", 31 + id % 7, id); // Colored output

    while (1) {
        int recipeIndex = get_next_recipe();
        if (recipeIndex == -1) {
            break; // No more recipes to process
        }

        struct Recipe* recipe = &recipes[recipeIndex];
        printf("\033[%dmBaker %d: Starting %s.\033[0m\n", 31 + id % 7, id, recipe->name);

        // Pantry access
        sem_wait(&semPantry);
        printf("\033[%dmBaker %d: Accessing the pantry for %s.\033[0m\n", 31 + id % 7, id, recipe->name);
        for (int i = 0; i < 6; i++) {
            if (recipe->pantry[i]) {
                printf("\033[%dmBaker %d: Taking pantry ingredient %d for %s.\033[0m\n", 31 + id % 7, id, i + 1, recipe->name);
            }
        }
        sem_post(&semPantry);

        // Simulate ingredient gathering
        simulate_task("Gathering ingredients for", id, recipe->name, 500);

        // Fridge access
        sem_wait(&semFridge);
        printf("\033[%dmBaker %d: Accessing the fridge for %s.\033[0m\n", 31 + id % 7, id, recipe->name);
        for (int i = 0; i < 3; i++) {
            if (recipe->fridge[i]) {
                printf("\033[%dmBaker %d: Taking fridge ingredient %d for %s.\033[0m\n", 31 + id % 7, id, i + 1, recipe->name);
            }
        }
        sem_post(&semFridge);

        // Simulate ingredient preparation
        simulate_task("Preparing ingredients for", id, recipe->name, 500);

        // Mixing
        sem_wait(&semBowl);
        sem_wait(&semSpoon);
        sem_wait(&semMixer);
        printf("\033[%dmBaker %d: Mixing ingredients for %s.\033[0m\n", 31 + id % 7, id, recipe->name);
        simulate_task("Mixing ingredients for", id, recipe->name, 1000);
        sem_post(&semMixer);
        sem_post(&semSpoon);
        sem_post(&semBowl);

        // Baking
        sem_wait(&semOven);
        printf("\033[%dmBaker %d: Baking %s.\033[0m\n", 31 + id % 7, id, recipe->name);
        simulate_task("Baking", id, recipe->name, recipe->bakingTime);
        sem_post(&semOven);

        printf("\033[%dmBaker %d: Finished %s.\033[0m\n", 31 + id % 7, id, recipe->name);
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

