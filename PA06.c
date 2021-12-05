#include "random437.h"
#include <pthread.h>
#include <sys/types.h>
#include <time.h>

int * getHourMinuteSec(long startTime, long currentTime);
int getMeanArrival(int hour);

int MAXPERCAR;
int CARNUM;
const int MAXWAITPEOPLE = 800;



int main(int argc, char *argv[]){

	// Take Input Args	
	MAXPERCAR = argv[2];
	CARNUM = argv[4];
	
	struct
	
	time_t startTime;
	pthread_t tid;
	int hours, minutes, seconds;
	int hoursMinutesSecs[3];
	int meanArrival;
	int 
	
	time(&startTime);
	
	while(startTime - currentTime <= 6){ // 600 minutes = 6000ms == 6 seconds (1 min = 10 ms) -> (1 sec = 0.167 ms)
		time(&currentTime);
		hoursMinutesSecs = getHourMinuteSec(startTime, currentTime);
		hours = hoursMinutesSecs[0];
		minutes = hoursMinutesSecs[1];
		seconds = hoursMinutesSecs[2];
		
		meanArrival = getMeanArrival(hour);
		
		
		
	}

    
}


int * getHourMinuteSec(long startTime, long currentTime){
	float time_passed;
	long totalMinutes;
	int hours;
	int minutes;
	int seconds;
	int hoursMinsSecs[3];
	
	time_passed = startTime - currentTime;
	totalMinutes = (time_passed * 1000) / 10; //convert to milliseconds and then convert to minutes
        hours = totalMinutes / 60; 
        minutes = totalMinutes % 60; 
        seconds = 0;
        
        hoursMinsSecs = {hours, minutes seconds};
        
        return hoursMinsSecs;

}


int genArrivals(int timestep){
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
