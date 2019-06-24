#ifndef RC_PWM_CONTROL_H
#define RC_PWM_CONTROL_H

#include <wiringPi.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include "common.h"
#include "constants.h"



//#define Anz_RC_Channels	10
//const int RCPin[Anz_RC_Channels] = {30, 21, 22, 23, 24, 25, 26, 27, 28, 29};
extern unsigned int RC_Channel[Anz_RC_Channels];
pthread_t t_RC_Control;


void *RC_PWM_Thread(void *value) {
	int last[Anz_RC_Channels];
	printf("RC_PWM_Thread start \n");
	while(run) {
		for (int i=0;i<Anz_RC_Channels;i++) 
			RC_Channel[i] = pulsln(RCPin[i],HIGH,1000000); 
		
		for (int i=0;i<Anz_RC_Channels;i++) {
			if (RC_Channel[i] !=last[i])
				Control();
			last[i] = RC_Channel[i];
		}
	}
	printf("RC_PWM_Thread end\n");
	return NULL;
}


/////////////////////////////
int init_RC_PWM() {
	for (int i=0;i<Anz_RC_Channels;i++) { 
		RC_Channel[i] = 0;
		pinMode (RCPin[i],INPUT);
		pullUpDnControl(RCPin[i],PUD_DOWN);
	}
	if(pthread_create(&t_RC_Control, NULL, RC_PWM_Thread, NULL)) {
	   	printf("Error creating thread t_RC_PWM_Control \n");
	   	return 0;
	}else{
		return 1;
	}
}


#endif
