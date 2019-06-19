//Read 10 PWM Channels
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

pthread_t t_RC_Control;
int RC_Channel[Anz_RC_Channels][2];


void *RC_PWM_Thread(void *value) {
	int last[Anz_RC_Channels];
	printf("RC_PWM_Thread start \n");
	while(run) {
		for (int i=0;i<Anz_RC_Channels;i++) 
			RC_Channel[i][1] = pulsln(RC_Channel[i][0],HIGH,1000000); 
		
		for (int i=0;i<Anz_RC_Channels;i++) {
			if (RC_Channel[i][1] !=last[i])
				Control();
			last[i] = RC_Channel[i][1];
		}
	}
	printf("RC_PWM_Thread end\n");
	return NULL;
}


/////////////////////////////
int init_RC_PWM() {
	for (int i=0;i<Anz_RC_Channels;i++) {
		RC_Channel[i][0] = RCPin[i]; 
		RC_Channel[i][1] = 0;
		pinMode (RC_Channel[i][0], INPUT);
		pullUpDnControl(RC_Channel[i][0],PUD_DOWN);
	}
	if(pthread_create(&t_RC_Control, NULL, RC_PWM_Thread, NULL)) {
	   	printf("Error creating thread t_RC_PWM_Control \n");
	   	return 0;
	}else{
		return 1;
	}
}


#endif
