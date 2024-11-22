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
    int bakingTime;    // Time to "bake" the recipe in milliseconds
};

struct Recipe recipes[] = {
    {"Cookies", {true, true, false, false, false, false}, {false, true, true}, 1500},
    {"Pancakes", {true, true, false, true, true, false}, {true, true, true}, 1200},
    {"Homemade Pizza Dough", {false, true, true, false, false, false}, {false, false, false}, 1800},
    {"Soft Pretzels", {true, true, true, false, true, false}, {true, false, false}, 2000},
    {"Cinnamon Rolls", {true, true, true, false, false, true}, {true, false, true}, 2500}
};

//------------------SHARED RESOURCES----------------------
sem_t semPantry, semFridge[2], semMixer, semOven, semBowl, semSpoon;

// variable to keep track of recipies
int numRecipes = sizeof(recipes) / sizeof(struct Recipe);

// Function to simulate task duration
void simulate_task(const char* task, int bakerId, const char* recipe, int durationMs) {
    printf("\033[%dmBaker %d: %s %s (will take %d ms).\033[0m\n", 
           31 + bakerId % 7, bakerId, task, recipe, durationMs);
    usleep(durationMs * 1000); // Convert milliseconds to microseconds
}

// Function to grab a pantry item
void grab_pantry_item(int bakerId, int pantryIndex) {
    sem_wait(&semPantry); // Wait for pantry access
    simulate_task("Grabbing item from pantry", bakerId, "", 300);
    sem_post(&semPantry); // Release pantry access
}

// Function to grab a fridge item
void grab_fridge_item(int bakerId, int fridgeIndex) {
    sem_wait(&semFridge[fridgeIndex % 2]); // Wait for fridge access (either fridge 0 or 1)
    simulate_task("Grabbing item from fridge", bakerId, "", 300);
    sem_post(&semFridge[fridgeIndex % 2]); // Release fridge access
}

// Function for bakers to work on recipes
void* baker(void* arg) {
    int bakerId = *(int*)arg;

    for (int recipeIndex = 0; recipeIndex < numRecipes; recipeIndex++) {
        struct Recipe recipe = recipes[recipeIndex];
        printf("\033[%dmBaker %d: Starting recipe '%s'.\033[0m\n", 
               31 + bakerId % 7, bakerId, recipe.name);

        // Grab ingredients from the pantry one by one
        for (int i = 0; i < 6; i++) {
            if (recipe.pantry[i]) {
                grab_pantry_item(bakerId, i);
            }
        }

        // Grab ingredients from the fridge one by one
        for (int i = 0; i < 3; i++) {
            if (recipe.fridge[i]) {
                grab_fridge_item(bakerId, i);
            }
        }

        // Mixing phase
        sem_wait(&semBowl);
        sem_wait(&semSpoon);
        sem_wait(&semMixer);
        simulate_task("Mixing ingredients for", bakerId, recipe.name, 1000);
        sem_post(&semMixer);
        sem_post(&semSpoon);
        sem_post(&semBowl);

        // Baking phase
        sem_wait(&semOven);
        simulate_task("Baking", bakerId, recipe.name, recipe.bakingTime);
        sem_post(&semOven);

        printf("\033[%dmBaker %d: Finished recipe '%s'.\033[0m\n", 
               31 + bakerId % 7, bakerId, recipe.name);
    }

    printf("\033[%dmBaker %d: Completed all recipes!\033[0m\n", 
           31 + bakerId % 7, bakerId);

    return NULL;
}

//-------------------------MAIN------------------------------
int main() {
    int numBakers;

    // Initialize semaphores
    sem_init(&semPantry, 0, 1);           // Only 1 baker in the pantry
    sem_init(&semFridge[0], 0, 1);        // Fridge 0
    sem_init(&semFridge[1], 0, 1);        // Fridge 1
    sem_init(&semMixer, 0, 2);            // 2 mixers available
    sem_init(&semOven, 0, 1);             // 1 oven available
    sem_init(&semBowl, 0, 3);             // 3 bowls available
    sem_init(&semSpoon, 0, 5);            // 5 spoons available

    // Get the number of bakers
    printf("Enter the number of bakers: ");
    scanf("%d", &numBakers);

    // Create threads for bakers
    pthread_t bakers[numBakers];
    int bakerIds[numBakers];
    for (int i = 0; i < numBakers; i++) {
        bakerIds[i] = i + 1;
        pthread_create(&bakers[i], NULL, baker, &bakerIds[i]);
    }

    // Wait for all baker threads to finish
    for (int i = 0; i < numBakers; i++) {
        pthread_join(bakers[i], NULL);
    }

    // Destroy semaphores
    sem_destroy(&semPantry);
    sem_destroy(&semFridge[0]);
    sem_destroy(&semFridge[1]);
    sem_destroy(&semMixer);
    sem_destroy(&semOven);
    sem_destroy(&semBowl);
    sem_destroy(&semSpoon);

    printf("All bakers have completed all recipes!\n");
    return 0;
}

