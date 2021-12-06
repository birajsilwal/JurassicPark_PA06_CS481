#include "random437.h"
#include <pthread.h>
#include <sys/types.h>
#include <time.h>

int * getHourMinuteSec(int timestep);
int getMeanArrival(int hour);

int MAXPERCAR;
int CARNUM;
int numCarsAvailable;
const int MAXWAITPEOPLE = 800;
int waitingSpace[800];
int totalWaitingTime;
int totalArrived ;
int totalRejected;
int totalWaited;

pthread_mutex_t mutex;


void getTimeString(int currentTime){
	
	sec = 0;
	min = currentTime % 60;
	hr = 9 + currentTime/60;
	
	timeInfo.tm_sec = sec;
	timeInfo.tm_min = min;
	timeInfo.tm_hour = hr;

	strftime(buffer,10, "%H:%M:%S", &timeInfo);
	for(int i =0; i<8;i++){
		timeString[i] = buffer[i];
	}
	timeString[8] = '\0';
}


int main(int argc, char *argv[]){

	// Take Input Args	
	MAXPERCAR = argv[2];
	CARNUM = argv[4];
	
	pthread_t tid;
	int hours, minutes, seconds;
	int hoursMinutesSecs[3];
	int meanArrival;	
	char timestamp[8];
	int num_passengers;
	
	numCarsAvailable = CARNUM;
	totalWaitingTime = 0; 
	totalArrived = 0;
	totalRejected = 0;
	totalWaited = 0;


	//initialize waiting space
	for(int i  = 0; i < MAXWAITPEOPLE; i++){
		waitingSpace[i] = -1;
	}


	for(int timestep = 0; timestep < 600; timestep++){ // 600 minutes = 6000ms == 6 seconds (1 min = 10 ms) -> (1 sec = 0.167 ms)

		num_rejected = 0;
		num_arrivals = getMeanArrival(hour);
		num_waiting += num_arrivals;
		
		if(num_waiting > MAXWAITPEOPLE){
			num_rejected = num_waiting - MAXWAITPEOPLE;
			num_waiting = MAXWAITPEOPLE;
		}
		
		totalRejected += num_rejected;
		totalArrived += num_arrivals;
		totalWaited += num_waiting;

		//write to file
		writeToFile(timestep, num_rejected, num_arrivals, num_waiting);
		
		
		
		if(numCarsAvailable != 0){
			
			if(num_waiting > MAXPERCAR){
				num_passengers = MAXPERCAR;
			}else{
				num_passengers = num_waiting;
			}
			
			// update waiting time
			totalWaitingTime += (7 * num_passengers);

			// Send car with thread
			pthread_create(&tid, NULL, run_car, num_passengers);
			pthread_join(tid, NULL);
			
			//update num_waiting
			num_waiting -= num_passengers;
			
		}
		
	}

	printf("Total Number Arrived: %d", totalArrived);
	printf("Total Number Rejected: %d", totalRejected);
	printf("Total Number in Line: %d", totalWaited);
	printf("Average Waiting Time: %d Minutes", (totalWaitingTime / totalWaited) / 60);

    
}


void* run_car(int num_passengers){

	for(int i =0; i< MAXWAITPEOPLE; i++){
		pthread_mutex_lock(&mutex);
		numCarsAvailable--;
		if(leftPeople > 0){
			waitingSpace[i] = waitingSpace[i + num_passengers]; 			
		} else {
			waitingSpace[i] = -1;			
		}
		numCarAvailable++;
		pthread_mutex_unlock(&mutex);
	}

}


int getMeanArrival(int timestep){
	int meanArrival;
	
	if(timestep < 119){
		meanArrival = 25;
	}else if(timestep >= 120 && timestep < 299){
		meanArrival = 45;
	}else if(timestep >= 300 && timestep < 419){
		meanArrival = 35;
	}else{
		meanArrival = 25;
	}
	
	return poissonRandom(meanArrival);
}

int * getHourMinuteSec(int timestep){

	int hours;
	int minutes;
	int seconds;
	int hoursMinsSecs[3];

        hours = timestep / 60; 
        minutes = timestep % 60; 
        seconds = 0;
        
        hoursMinsSecs = {hours, minutes seconds};
        
        return hoursMinsSecs;

}


void writeToFile(int timestep, int num_rejected, int num_arrivals, int num_waiting){
	
	FILE *output_file = fopen("output.txt", "w");
	
	hoursMinsSecs = getHourMinuteSec(timestep);
	
	fprintf(output_file, "%d arrive %d reject %d wait-line %d at %d:%d:%d\n", timestep, num_arrivals, num_rejected, num_waiting, hoursMinSecs[0], hoursMinsSecs[1], hoursMinsSecs[2]);
	
	fclose(output_file);

}






