#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

//---------------INGREDIENTS/TOOLS STUFF---------------------



//-------------------SEMAPHORE STUFF-------------------------
int semId;

// signals for semaphores
struct sembuf using = {0, -1, SEM_UNDO};
struct sembuf using = {0, +1, SEM_UNDO};

//---------------------THREAD STUFF--------------------------
//location where the new thread is stored (I think the ids of the bakers)
pthread_t bakerId;
// attributes for the thread being created
pthread_attr_t baker_attr
// argument passed to start - (I think this is the number of threads)
int threadArgument;

// baker thread
void* baker (void* arg){
	int* N = (int *)arg;
	
	
}
//-------------------------MAIN------------------------------
int main(){
	printf("Please enter the number of bakers.");
	// again not sure if this is the right variable 
	scanf("%d", threadArgument);

	// create a thread for each baker
	for(int i = 0; i < threadArgument; i++){
		pthread_create(&bakerId, &baker_attr, baker, &threadArgument)
	}
	return 0;
}
