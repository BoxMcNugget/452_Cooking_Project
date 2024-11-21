#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

//----------------------RECIPIES-----------------------------
struct Cookies{
	bool flour;
	bool sugar;
	bool milk;
	bool butter;
}
struct Pancakes{
	bool flour;
	bool sugar;
	bool bakingSoda;
	bool salt;
	bool eggs;
	bool milk;
	bool butter;
}
struct HomemadePizzaDough{
	bool yeast;
	bool sugar;
	bool salt;
}
struct SoftPretzel{
	bool flour;
	bool sugar;
	bool salt;
	bool yeast;
	bool bakingSoda;
	bool egg;
}
struct CinnamonRolls{
	bool flour;
	bool sugar;
	bool salt;
	bool butter;
	bool eggs;
	bool cinnamon;
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
	int Eggs 	= 1;
	int Milk 	= 2;
	int Butter 	= 3;
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

sem_init(&semPantry, 0, 1) // 1 pantry
sem_init(&semFridge, 0, 2) // 2 fridges
sem_init(&semMixer, 0, 2) // 2 mixers
sem_init(&semBowl, 0, 3) // 3 bowls
sem_init(&semSpoon, 0, 5) // 5 spoons
sem_init(&semOven, 0, 1) // 1 Oven

//---------------------THREAD STUFF--------------------------
//location where the new thread is stored (I think the ids of the bakers)
pthread_t bakerId;
// attributes for the thread being created
pthread_attr_t baker_attr
// argument passed to start - (I think this is the number of threads)
int threadArgument;

// baker thread
void* baker (void* arg){
	
		
	
}
//-------------------------MAIN------------------------------
	struct Cookies *cookieRecipe;
	struct Pancakes *pancakesRecipe;
	struct HomeMadePizzaDough *pizzaRecipe;
	struct SoftPretzel *pretzelRecipe;
	struct CinnamonRolls *rollsRecipe;

	printf("Please enter the number of bakers.");
	// again not sure if threadargument is the right variable 
	scanf("%d", threadArgument);

	// create a thread for each baker
	for(int i = 0; i < threadArgument; i++){
		pthread_create(&bakerId, &baker_attr, baker, &threadArgument)
	}
	



	//
	// creating semids
	// Check if this way works here TODO********************************
 
	// get the semId for pantry
	if((semPantryId = semget(IPC_PRIVATE, 1, S_IRUSR|S_IWUSR)) == -1){
		perror("Semget: semget failed");
		exit(0);
	}
	// get the semId for fridge
	if((semFridgeId = semget(IPC_PRIVATE, 1, S_IRUSR|S_IWUSR)) == -1){
		perror("Semget: semget failed");
		exit(0);
	}
	
	return 0;
}



















