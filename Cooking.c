#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

//----------------------RECIPIES-----------------------------

// Define what a recipe is 
struct Recipe{
	// recipe name
	const char* name;

	// pantry and correlating index
	bool pantry[6];
	//Flour		1
	//Sugar		2
	//Yeast		3
	//Baking Sode	4
	//Salt		5
	//Cinnamon	6

	// fridge and correlating index
	bool fridge[3];
	//Eggs 		1
	//Milk 		2
	//Butter	3
};

struct Recipe recipes[] = {
	// setting the recipies to tell what ingredients are needed
	//{name ,{pantry bool}, {fridge bool}}

	{"Cookies", {true, true, false, false, false, false}, {false, true, true}},
	{"Pancakes", {true, true, false, true, true, false}, {true, true, true}},
	{"Homemade Pizza Dough", {false, true, true, false, false, false}, {false, false, false}},
	{"Soft Pretzles", {true, true, true, false, true, false}, {true, false, false}},
	{"Cinnamon Rolls", {true, true, true, false, false, true}, {true, false, true}},
};

// variable to keep track of recipes
int numRecipes = sizeof(recipes) / sizeof(struct Recipe);


//-------------------SEMAPHORE STUFF-------------------------
// declaring semaphores
sem_t semPantry;
sem_t semFridge[2];
sem_t semMixer;
sem_t semBowl;
sem_t semSpoon;
sem_t semOven;

pthread_mutex_t ramsiedLock = PTHREAD_MUTEX_INITIALIZER;

//------------------HELPER FUNCTIONS-------------------------
void doTask(const char* task, int bakerId, const char* recipe, int waitTime){
	// print out the baker, task, and recipe for updaating the viewer
	printf("\033[%dmBaker %d: %s %s \n",31 + bakerId % 7, bakerId, task, recipe);
	// sleep to simulate the task
	usleep(waitTime * 1000);
}

void grabPantryIngredient(int bakerId, int pantryIndex) {
	// wait for pantry
	sem_wait(&semPantry);
	// simulate doing a task
	doTask("Grabbing ingredient from pantry", bakerId, "", 250);
	// release the pantry
	sem_post(&semPantry);
}

void grabFridgeIngredient(int bakerId, int fridgeIndex) {
	// wait for fridge
	sem_wait(&semPantry);
	// simulate doing a task
	doTask("Grabbing ingredient from fridge", bakerId, "", 250);
	// release the fridge
	sem_post(&semPantry);
}



//---------------------THREAD STUFF--------------------------
// baker thread
void* baker (void* arg){
	int bakerId = *(int*)arg;

	// for every recipe
	for(int recipeNum = 0; recipeNum < numRecipes; recipeNum++){
		// instaciate a new recipe struct for that recipe
		struct Recipe recipe = recipes[recipeNum];
		// print starting recipe
		printf("\033[%dmBaker %d: Starting recipe %s \n",31 + bakerId % 7, bakerId, recipe.name);
		
		// grab all the ingredients from pantry
		for(int i = 0; i < 6; i++){
			// if that pantry ingredient is set to true
			if(recipe.pantry[i]){
				// grab that item at index i
				grabPantryIngredient(bakerId, i);
			}
		}
		

		// grab all the ingredients from fridge
		for(int i = 0; i < 3; i++){
			// if that fridge ingredient is set to true
			if(recipe.fridge[i]){
				// grab that item at index i
				grabFridgeIngredient(bakerId, i);
			}
		}
		
		//wait for bowl
		sem_wait(&semBowl);
		//wait for spoon
		sem_wait(&semSpoon);
		//wait for mixer
		sem_wait(&semMixer);
		
		// mix ingredients once all three are collected
		doTask("Mixing ingredients for", bakerId, recipe.name, 750);
		
		//release bowl
		sem_post(&semMixer);
		//release spoon
		sem_post(&semSpoon);
		//release mixer
		sem_post(&semBowl);

		//wait for the oven to be free
		sem_wait(&semOven);
		//simulate the baking
		doTask("Baking the", bakerId, recipe.name, 2000);
		//release the oven
		sem_post(&semOven);

		//print that baker has finished the recipe
		printf("\033[%dmBaker %d: Finished recipe %s \n",31 + bakerId % 7, bakerId, recipe.name);
	        

		if (rand() % 10 == 0) { // 10% chance of being Ramsied
			pthread_mutex_lock(&ramsiedLock);
			printf("\033[1;31mBaker %d: Got Ramsied! Restarting %s.\033[0m\n", bakerId, recipes[recipeNum].name);
			pthread_mutex_unlock(&ramsiedLock);
			recipeNum--; // Restart the current recipe
	        }
	}
	
	// after all the recipes have been cooked print finished
	printf("\033[%dmBaker %d: Completed all recipes :) \n", 31 + bakerId % 7, bakerId);

	return 0;
}

//-------------------------MAIN------------------------------
int main() {
	int numBakers;
	
	//initilize semaphores
	sem_init(&semPantry, 0, 1); // 1 pantry
	sem_init(&semFridge[0], 0, 2); // fridge 1
	sem_init(&semFridge[1], 0, 2); // fridge 2
	sem_init(&semMixer, 0, 2); // 2 mixers
	sem_init(&semBowl, 0, 3); // 3 bowls
	sem_init(&semSpoon, 0, 5); // 5 spoons
	sem_init(&semOven, 0, 1); // 1 Oven


	// ask for number of bakers
	printf("Please enter the number of bakers: ");
	// again not sure if threadargument is the right variable 
	scanf("%d", &numBakers);

	// create threads to hold each baker
	pthread_t bakers[numBakers];
	// variable to keep track of baker ids
	int bakerIds[numBakers];

	// for each baker
	for(int i = 0; i < numBakers; i++) {
		// Incrementing the ids
		bakerIds[i] = i + 1;
		// create a thread
		pthread_create(&bakers[i], NULL, baker, &bakerIds[i]);
	}

	// for every baker
	for(int i = 0; i < numBakers; i++){
		// join the threads back
		pthread_join(bakers[i], NULL);
	}

	// destroy all semaphores	
	sem_destroy(&semPantry);
	sem_destroy(&semFridge[0]);
	sem_destroy(&semFridge[1]);
	sem_destroy(&semMixer);
	sem_destroy(&semBowl);
	sem_destroy(&semSpoon);
	sem_destroy(&semOven);

	printf("All bakers have completed all recipies :) \n");

	return 0;
}



















