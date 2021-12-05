#include "random437.h"
#include <pthread.h>
#include <sys/types.h>
#include <getopt.h>
#include <time.h>

int * getHourMinuteSec(long startTime, long currentTime);
int getMeanArrival(int hour);

int MAXPERCAR;
int CARNUM;
const int MAXWAITPEOPLE = 800;

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


int * getHourMinuteSec(long startTime, long currentTime) {
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
        
        hoursMinsSecs = {hours, minutes, seconds};
        
        return hoursMinsSecs;

}


int getMeanArrival(int hour){
	int meanArrival;
	
	if(hour >= 9 && hour < 11){
		meanArrival = 25;
	}else if(hour >= 11 && hour < 14){
		meanArrival = 45;
	}else if(hour >= 14 && hour < 16){
		meanArrival = 35;
	}else{
		meanArrival = 25;
	}
	
	return meanArrival;
}
