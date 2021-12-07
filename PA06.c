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
int peopleInWaitingArea = 0;
int totalWaitTime = 0;
int waitingArea[800];
double totalWaitingTime; // in seconds
int totalArrived ;
int totalRejected;
int totalWaited;
char timeString[10];

pthread_t tid;
pthread_mutex_t mutex;

struct tm timeInfo ={0};

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
	
	// return poissonRandom(meanArrival);
	return poissonRandom(meanArrival);
}

/**
 * Initialize the waiting area array.
 * initially the waiting time none.
 */
void initiateWaitingSpace() {
	for (int i = 0; i < MAXWAITPEOPLE; i++) {
		waitingArea[i] = -1;
	}
}	


/**
 * increases waiting time. 
 */
void increaseWaitingTime() {
	for (int i = 0; i < MAXWAITPEOPLE; i++) {
		if (waitingArea[i] == 0) {
			waitingArea[i]++;
		}
	} 
}


void* run_car(void* numPassengers){

	int * num_passengers = (int *) num_passengers;
	int leftPeople = num_passengers[0];

	for(int i =0; i< MAXWAITPEOPLE; i++){
		pthread_mutex_lock(&mutex);
		numCarsAvailable--;
		if(leftPeople > 0){
			waitingArea[i] = waitingArea[i + leftPeople]; 			
		} else {
			waitingArea[i] = -1;			
		}
		numCarsAvailable++;
		pthread_mutex_unlock(&mutex);
		leftPeople--;
	}

	pthread_mutex_lock(&mutex);
	totalWaitingTime += 53;
	pthread_mutex_unlock(&mutex);

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

			for (int i = 0; i < numOfPeopleRiding; i++) {
				totalWaitTime += 52;
			}
			
			// TODO: need to implement thread_run function

			int *passengers = (int *)malloc(sizeof(int));
			passengers[0] = numOfPeopleRiding;

			pthread_create(&tid, NULL, run_car, (void *)passengers);
			pthread_join(tid, NULL);
			// number of people waiting decreases on the car takes some people on a ride
			peopleInWaitingArea -= numOfPeopleRiding;
			}
	}
}


int * getHourMinuteSec(int timestep){

	int hours;
	int minutes;
	int seconds;
	static int hoursMinsSecs[3];

  hours = timestep / 60; 
  minutes = timestep % 60; 
  seconds = 0;
        
  hoursMinsSecs[0] = hours;
  hoursMinsSecs[1] = minutes;
  hoursMinsSecs[2] = seconds;
        
  return hoursMinsSecs;

}

void getTimeString(int timestep){
	int * hoursMinsSecs;
	char buffer [20];
	
	hoursMinsSecs = getHourMinuteSec(timestep);

	timeInfo.tm_sec = hoursMinsSecs[2];
	timeInfo.tm_min = hoursMinsSecs[1];
	timeInfo.tm_hour = hoursMinsSecs[0];

	strftime(buffer,10, "%H:%M:%S", &timeInfo);
	for(int i =0; i<8;i++){
		timeString[i] = buffer[i];
	}
	timeString[8] = '\0';
}


void writeToFile(int timestep, int num_rejected, int num_arrivals, int num_waiting){
	
	FILE *output_file = fopen("output.txt", "w");
	
	int * hoursMinsSecs;
	
	hoursMinsSecs = getHourMinuteSec(timestep);
	getTimeString;
	
	fprintf(output_file, "%d arrive %d reject %d wait-line %d at %s\n", timestep, num_arrivals, num_rejected, num_waiting, timeString);
	fclose(output_file);

}


// int main(int argc, char *argv[]) {
	
// 	waitingArea = (int *)malloc(MAXWAITPEOPLE);
	
// 	for(int timestep = 0; timestep < 600; timestep++){ // 600 minutes = 6000ms == 6 seconds (1 min = 10 ms) -> (1 sec = 0.167 ms)

int main(int argc, char *argv[]) {
	
	int hours, minutes, seconds;
	int hoursMinutesSecs[3];
	int meanArrival;	
	char timestamp[8];
	int num_passengers;
	int num_rejected = 0;
	int num_arrivals = 0;
	int num_waiting = 0;
	
	totalWaitingTime = 0; 
	totalArrived = 0;
	totalRejected = 0;
	totalWaited = 0;

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
	FILE *output_file = fopen("output.txt", "w");
	//initialize waiting space
	initiateWaitingSpace();
	numCarsAvailable = CARNUM;

	

	// 600 minutes = 6000ms == 6 seconds (1 min = 10 ms) -> (1 sec = 0.167 ms)
	for(int timestep = 0; timestep < 600; timestep++) { 

		num_rejected = 0;
		num_arrivals = getArrivals(timestep);
		//printf("num_arrivals: %d", num_arrivals);
		num_waiting += num_arrivals;
		
		if(num_waiting > MAXWAITPEOPLE){
			num_rejected = num_waiting - MAXWAITPEOPLE;
			num_waiting = MAXWAITPEOPLE;
		}
		
		totalRejected += num_rejected;
		totalArrived += num_arrivals;
		totalWaited += num_waiting;

		//printf("TimeString: %s  ", timeString );
		//printf("Num Waiting: %d  ", num_waiting);

		//printf("Hello, World!");
		//write to file
		// writeToFile(timestep, num_rejected, num_arrivals, num_waiting);
		fprintf(output_file, "%d arrive %d reject %d wait-line %d at %s\n", timestep, num_arrivals, num_rejected, num_waiting, timeString);
		
		if(numCarsAvailable != 0){
			
			if(num_waiting > MAXPERCAR){
				num_passengers = MAXPERCAR;
			}else{
				num_passengers = num_waiting;
				peopleInWaitingArea = num_waiting;
			}
			
			// update waiting time
		//	printf("update total waiting time\n ");
			totalWaitingTime += (float) (7 * num_passengers);

			// Send car with thread
			explorerThread();
			
			//update num_waiting
			num_waiting -= num_passengers;
		}
		
	}

	printf("Total Number Arrived: %d. ", totalArrived);
	printf("Total Number Rejected: %d. ", totalRejected);
	printf("Total Number in Line: %d. ", totalWaited);
	printf("Total Waiting Time: %.2lf ", totalWaitingTime);
	printf("Average Waiting Time: %.2lf Minutes.\n", ( totalWaitingTime / (float) totalWaited) / 60);

    
}







