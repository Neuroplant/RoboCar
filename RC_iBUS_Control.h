#ifndef RC_IBUS_CONTOL_H
#define RC_IBUS_CONTOL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include "common.h"
#include "constants.h"
#include "ibus.h"

#ifndef RCCONTROL_H
extern unsigned int RC_Channel[Anz_RC_Channels];
void Control(void) {
	for (int i=0; i<Anz_RC_Channels; i++) {
		printf(" i%: i% ",i, RC_Channel[i]);
	}
	printf("\n");
}
#endif
pthread_t t_RCControl;


void *RC_iBUS_Thread (void *value) {
	int last[Anz_RC_Channels];
	printf("RC_PPM_Thread start\n");
	while (1) {
		ReadData();
		int j=0;
		for(int i=2;i<100;i=i+2){ 
			if (j > Anz_RC_Channels) break;
			RC_Channel[j++]=ByteToWord(Data[i+1],Data[i]);
		}		
		for(int i=0; i<Anz_RC_Channels; i++) {
			if (RC_Channel[i] !=last[i]) 
				Control();
			last[i] = RC_Channel[i];
		}
	}
	printf("RC_PPM_Thread end\n");
	return NULL;
}

int init_RC_iBUS_Control() {
	for (int i=0;i<Anz_RC_Channels;i++) 
		RC_Channel[i]=0; 
	open_serial();
	if(pthread_create(&t_RCControl, NULL, RC_iBUS_Thread, NULL)) {
		printf("Error creating thread t_RCControl\n");
		return 0;
	}else{
		return 1;
	}
}


#endif
