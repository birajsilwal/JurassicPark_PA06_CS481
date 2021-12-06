#include "random437.h"
#include <pthread.h>
#include <sys/types.h>
#include <getopt.h>
#include <time.h>

int * getHourMinuteSec(int timestep);
int getMeanArrival(int hour);

int MAXPERCAR;
int CARNUM;
int numCarsAvailable;
const int MAXWAITPEOPLE = 800;
const int LOADING_TIME = 7;
int waitingArea[MAXWAITPEOPLE];

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



int main(int argc, char *argv[]) {

	int carnum;
	int maxpercar;
	int opt;

	// handling user command line argument using getopt
  while((opt = getopt(argc, argv, "N:M:")) != -1) {
    switch(opt){
      case 'N':
        carnum = atoi(optarg);
        break;
      case 'M':
        maxpercar = atoi(optarg);
        break;
      default:
        printf("Incorrect Flags\n");
        return(-1);
    }
  }
	
	pthread_t tid;
	int hours, minutes, seconds;
	int hoursMinutesSecs[3];
	int meanArrival;	
	char timestamp[8];
	int num_passengers;
	
	numCarsAvailable = CARNUM;
	
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
		
		
		
		if(numCarsAvailable != 0){
			
			if(num_waiting > MAXPERCAR){
				num_passengers = MAXPERCAR;
			}else{
				num_passengers = num_waiting;
			}
			
			// Send car with thread
			pthread_create(&tid, NULL, run_car, num_passengers;
			pthread_join(tid, NULL);
			
			//update num_waiting
			num_waiting -= num_passengers;
			
		}
		
	}

    
}



void* runCar(int numPassengers){
	numCarsAvailable--;
	//do something 
	//factor in load and ride time
	numCarAvailable++;
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
	
	hoursMinsSecs = getHourMinuteSec(timestep);
	
	fprintf(output_file, "%d arrive %d reject %d wait-line %d at %d:%d:%d\n", timestep, num_arrivals, num_rejected, num_waiting, hoursMinSecs[0], hoursMinsSecs[1], hoursMinsSecs[2]);
	
	fclose(output_file);

}






