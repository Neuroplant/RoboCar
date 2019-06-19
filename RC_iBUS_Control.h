#IFNDEF RC_IBUS_CONTOL_H
#define RC_IBUS_CONTOL_H
/*PPM encoder*/
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
#include "iBUS.h"

unsigned int RC_Channel[Anz_RC_Channels][2];
pthread_t t_RCControl;


void *RC_iBUS_Thread (void *value) {
	int last[Anz_RC_Channels];
	printf("RC_PPM_Thread start\n");
	while (run) {
		ReadData();
		int j=0;
		for(int i=2;i<100;i=i+2){ 
			if (j > Anz_RC_Channels) break;
			RC_Channel[j++][1]=ByteToWord(Data[i+1],Data[i]);
		}		
		for(int i=0; i<Anz_RC_Channels; i++) {
			if (RC_Channel[i][1] !=last[i]) 
				Control();
			last[i] = RC_Channel[i][1];
		}
	}
	printf("RC_PPM_Thread end\n");
	return NULL;
}

int init_RC_PPM_Control() {
	for (int i=0;i<Anz_RC_Channels;i++) 
		RC_Channel[i][1]=0; 
	open_serial();
	if(pthread_create(&t_RCControl, NULL, RC_iBUS_Thread, NULL)) {
		printf("Error creating thread t_RCControl\n");
		return 0;
	}else{
		return 1;
	}
}


#endif
