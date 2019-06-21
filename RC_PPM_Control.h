#IFNDEF RC_PPM_CONTOL_H
#define RC_PPM_CONTOL_H

#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include "common.h"
#include "constants.h"
extern unsigned int RC_Channel[Anz_RC_Channels];
pthread_t t_RCControl;

void *RC_PPM_Thread (void *value) {
	int last[Anz_RC_Channels];
	printf("RC_PPM_Thread start\n");
	while (run) {
		while(pulsln(PPMPin, LOW,1000000) < 10000){} 		//wait
		for(int i=0; i<Anz_RC_Channels; i++)
			RC_Channel[i]=pulsln(PPMPin, LOW,1000000);
		
		for(int i=0; i<Anz_RC_Channels; i++) {
			if (RC_Channel[i] !=last[i]) 
				Control();
			last[i] = RC_Channel[i];
		}

	}
	printf("RC_PPM_Thread end\n");
	return NULL;
}

int init_RC_PPM_Control() {
	for (int i=0;i<Anz_RC_Channels;i++) 
		RC_Channel[i]=0; 
	pinMode (PPMPin, INPUT);
	pullUpDnControl(PPMPin,PUD_DOWN);	
	
	if(pthread_create(&t_RCControl, NULL, RC_PPM_Thread, NULL)) {
		printf("Error creating thread t_RCControl\n");
		return 0;
	}else{
		return 1;
	}
}


#endif
