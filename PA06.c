#include "random437.h"
#include <pthread.h>
#include <sys/types.h>
#include <getopt.h>
#include <time.h>

int MAXPERCAR;
int CARNUM;
int numCarsAvailable;
const int MAXWAITPEOPLE = 800;
const int LOADING_TIME = 7;
int * waitingArea;
int peopleInWaitingArea = 0;
int totalWaitTime = 0;
pthread_t tid;

/** 
 * @param int hour
 * @return random number of people  
 * returns randomly generated number of people who will arrive in the give hour 
 * time is between 9:00am - 7:00pm
*/ 
int getArrivals(int timestep){
	int meanArrival = 0;

	if(timestep < 120) { meanArrival = 25; }
	else if(timestep >= 120 && timestep < 300) { meanArrival = 45; }
	else if(timestep >= 300 && timestep < 420) { meanArrival = 35; }
	else { meanArrival = 25; }
	
	return poissonRandom(meanArrival);
}

/**
 * Initialize the waiting area array.
 * initially the waiting time none.
 */
void initiateWaitingArea() {
	for (int i = 0; i < MAXWAITPEOPLE; i++) {
		waitingArea[i] = 0;
	}
}	


/**
 * increases waiting time. 
 */
void increseWaitingTime() {
	for (int i = 0; i < MAXWAITPEOPLE; i++) {
		if (waitingArea[i] == 0) {
			waitingArea[i]++;
		}
	} 
}

// this is a mock thread_run method taken from lecture
// need to implement for explorer
static int count = 0;
void* thread_run(void* parm) {
	for (int i = 0; i < 5; i++) {
		count++;
		printf("The thread_run method count is = %d\n", count);
		sleep(1);
	}
	return NULL;
}


/**
 * drive the explorer, for each explorer (CARNUM), create a new thread.
 * each ride in explorer consists of MAXPERCAR or less people.
 */
void explorerThread() {
	int numOfPeopleRiding;
	// for each car, load the min number of people, increase 
	// total waiting time and create a thread.
	for (int i = 0; i < CARNUM; i++) {
		if (peopleInWaitingArea > 0) {
			// number of people taking ride could be less than MAXPERCAR
			// getting the min number of people to load
			numOfPeopleRiding = peopleInWaitingArea < MAXPERCAR ? peopleInWaitingArea : MAXPERCAR;
		}
		for (int i = 0; i < numOfPeopleRiding; i++) {
			totalWaitTime++;
		}
		// TODO: need to implement thread_run function
		pthread_create(&tid, NULL, thread_run, (void *) (int *) malloc(sizeof(int)));
		pthread_join(tid, NULL);
		// number of people waiting decreases on the car takes some people on a ride
		peopleInWaitingArea -= numOfPeopleRiding;
	}
}


void* runCar(int numPassengers){
	numCarsAvailable--;
	//do something 
	//factor in load and ride time
	numCarsAvailable++;
}

int * getHourMinuteSec(int timestep){

	int hours;
	int minutes;
	int seconds;
//	int hoursMinsSecs[3];

  hours = timestep / 60; 
  minutes = timestep % 60; 
  seconds = 0;
        
  int hoursMinsSecs[3] = {hours, minutes, seconds};
        
  return hoursMinsSecs;

}


void writeToFile(int timestep, int num_rejected, int num_arrivals, int num_waiting){
	
	FILE *output_file = fopen("output.txt", "w");
	
	int * hoursMinsSecs;
	
	hoursMinsSecs = getHourMinuteSec(timestep);
	
	fprintf(output_file, "%d arrive %d reject %d wait-line %d at %d:%d:%d\n", timestep, num_arrivals, num_rejected, num_waiting, hoursMinSecs[0], hoursMinsSecs[1], hoursMinsSecs[2]);
	
	fclose(output_file);

}


int main(int argc, char *argv[]) {
	int opt;
	// handling user command line argument using getopt
  while((opt = getopt(argc, argv, "N:M:")) != -1) {
    switch(opt){
      case 'N':
        CARNUM = atoi(optarg);
        break;
      case 'M':
        MAXPERCAR = atoi(optarg);
        break;
      default:
        printf("Incorrect Flags\n");
        return(-1);
    }
  }
	
	int hours, minutes, seconds;
	int hoursMinutesSecs[3];
	int meanArrival;	
	char timestamp[8];
	int num_passengers;
	int num_rejected;
	int num_arrivals;
	int num_waiting;
	
	waitingArea = (int *)malloc(MAXWAITPEOPLE);
	
	for(int timestep = 0; timestep < 600; timestep++){ // 600 minutes = 6000ms == 6 seconds (1 min = 10 ms) -> (1 sec = 0.167 ms)

		num_rejected = 0;
		num_arrivals = getMeanArrival(timestep);
		num_waiting += num_arrivals;
		
		if(num_waiting > MAXWAITPEOPLE){
			num_rejected = num_waiting - MAXWAITPEOPLE;
			num_waiting = MAXWAITPEOPLE;
		}
		
		//write to file
		writeToFile(timestep, num_rejected, num_arrivals, num_waiting);
	}
}


