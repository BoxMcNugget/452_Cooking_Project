#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>



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
}

struct Recipe recipes[] = {
	// setting the recipies to tell what ingredients are needed
	//{name ,{pantry bool}, {fridge bool}}

	{"Cookies", {true, true, false, false, false, false}, {false, true, true}}
	{"Pancakes", {true, true, false, true, true, false}, {true, true, true}}
	{"Homemade Pizza Dough", {false, true, true, false, false, false}, {false, false, false}}
	{"Soft Pretzles", {true, true, true, false, true, false}, {true, false, false}}
	{"Cinnamon Rolls", {true, true, true, false, false, true}, {true, false, true}}
}

//-------------------SEMAPHORE STUFF-------------------------
// declaring semaphores
sem_t semPantry;
sem_t semFridge[2];
sem_t semMixer;
sem_t semBowl;
sem_t semSpoon;
sem_t semOven;

//------------------HELPER FUNCTIONS-------------------------
void grabPantryIngredient(int bakerId, int pantryIndex) {
	// wait for pantry
	sem_wait(&semPantry);
	
	
	
	// release the pantry
	sem_post(&semPantry)
}



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
	struct Cookies *cookieRecipe = {1, 2, 8, 9};
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



















