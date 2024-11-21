#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
	
enum Ingredient {
	FLOUR,				//0
	SUGAR,				//1
	YEAST,				//2
	BAKING_SODA,		//3
	SALT,				//4
	CINNAMON,			//5
	EGGS,				//6
	MILK,				//7
	BUTTER				//8
};

//----------------------RECIPIES-----------------------------
struct Cookies{
	enum Ingredient flour;
	enum Ingredient sugar;
	enum Ingredient milk;
	enum Ingredient butter;
}

struct Pancakes{
	enum Ingredient flour;
	enum Ingredient sugar;
	enum Ingredient bakingSoda;
	enum Ingredient salt;
	enum Ingredient egg;
	enum Ingredient milk;
	enum Ingrdeient butter;
}

struct HomemadePizzaDough{
	enum Ingredient yeast;
	enum Ingredient sugar;
	enum Ingredient salt;	
}

struct SoftPretzel{
	enum Ingredient flour;
	enum Ingredient sugar;
	enum Ingredient salt;
	enum Ingredient yeast;
	enum Ingredient bakingSoda;
	enum Ingredient egg;
}

struct CinnamonRolls{
	enum Ingredient flour;
	enum Ingredient sugar;
	enum Ingredient salt;
	enum Ingredient butter;
	enum Ingredient eggs;
	enum Ingredient cinnamon;
}

//------------------SHARED MEMORY STUFF----------------------
struct pantryData{
	int flour 	= 1;
	int sugar 	= 2;
	int yeast 	= 3;
	int bakingSoda 	= 4;
	int salt 	= 5;
	int cinnamon 	= 6;
}

struct fridgeData{
	int Eggs 	= 7;
	int Milk 	= 8;
	int Butter 	= 9;
}

//-------------------SEMAPHORE STUFF-------------------------
sem_t semPantry;
sem_t semFridge;
sem_t semMixer;
sem_t semBowl;
sem_t semSpoon;
sem_t semOven;


// signals for semaphores
//struct sembuf using = {0, -1, SEM_UNDO};
//struct sembuf done = {0, +1, SEM_UNDO};

//sem_init(&semPantry, 0, 1) // 1 pantry
//sem_init(&semFridge, 0, 2) // 2 fridges
//sem_init(&semMixer, 0, 2) // 2 mixers
//sem_init(&semBowl, 0, 3) // 3 bowls
//sem_init(&semSpoon, 0, 5) // 5 spoons
//sem_init(&semOven, 0, 1) // 1 Oven

//---------------------THREAD STUFF--------------------------
//location where the new thread is stored (I think the ids of the bakers)
pthread_t bakerId;
// attributes for the thread being created
pthread_attr_t baker_attr
// argument passed to start - (I think this is the number of threads)
int threadArgument;

// baker thread
void* baker (void* arg){
	// decide what recipe to do

	// loop for doing recipe
	// while(recipe not done)
	
		// look at recipe and choose get what ingredients to grab
			// add all the ingredients needed to an array	
	
		// loop to grab ingredients 
		// while(ingredientsArray[] != NULL)
			// sem_wait(semaphore name) to wait to access
		
			// access and grab the one thing
				// check for numbers that are in the array
				// remove ingredient number from array 

			// sem_post(semaphore name)
			// set that thing to true to mark that it has it	
		// loop

		// wait for the bowl mixer and spoon to make 
			// simulate time to mix ingredients
		// wait for the oven 
			// simulate time to cook ingredients
	
		// tell recipe done
	
	// loop back to next recipe


}
//-------------------------MAIN------------------------------
	struct Cookies *cookieRecipe = {FLOUR, SUGAR, MILK, BUTTER};
	struct Pancakes *pancakesRecipe;
	struct HomeMadePizzaDough *pizzaRecipe;
	struct SoftPretzel *pretzelRecipe;
	struct CinnamonRolls *rollsRecipe;
	
	sem_init(&semPantry, 0, 1) // 1 pantry
	sem_init(&semFridge, 0, 2) // 2 fridges
	sem_init(&semMixer, 0, 2) // 2 mixers
	sem_init(&semBowl, 0, 3) // 3 bowls
	sem_init(&semSpoon, 0, 5) // 5 spoons
	sem_init(&semOven, 0, 1) // 1 Oven

	printf("Please enter the number of bakers.");
	// again not sure if threadargument is the right variable 
	scanf("%d", threadArgument);

	// create a thread for each baker
	for(int i = 0; i < threadArgument; i++){
		pthread_create(&bakerId, &baker_attr, baker, &threadArgument);
	}
	


	
	return 0;
}



















