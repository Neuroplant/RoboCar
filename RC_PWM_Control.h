//Read 10 PWM Channels
#ifndef RC_PWM_CONTROL_H
#define RC_PWM_CONTROL_H

#include <wiringPi.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "constants.h"
#include "common.h"

//#define Anz_RC_Channels	10
//const int RCPin[Anz_RC_Channels] = {30, 21, 22, 23, 24, 25, 26, 27, 28, 29};

pthread_t t_RC_PWM_Control;
int RC_Channel[Anz_RC_Channels][2];



void *RC_PWM_Thread(void *value) {
	int last[Anz_RC_Channels];
	printf("RC_PWM_Thread start \n");
	while(run) {
		for (int i=0;i<Anz_RC_Channels;i++) {
			RC_Channel[i][1] = pulsln(RC_Channel[i][0],HIGH,1000000); 
			if (RC_Channel[i][1] !=last[i]) {
				last[i] = RC_Channel[i][1];
				Control();
			}
		}
	}
	printf("RC_PWM_Thread end\n");
	return NULL;
}
////////////////////////////
void Control() {
//RC-Channel 1	(analog)
	steering = map(RC_Channel[0][1],RCmin,RCmax,10,-10);
//RC-Channel 2	(analog)

//RC-Channel 3	(analog)
	if (encoder_mode) {
		Spin_Target = (map(RC_Channel[2][1],RCmin,RCmax,0,(int)SPIN_MAX));
	}else{
		throttle = (map(RC_Channel[2][1], RCmin,RCmax,0,(int)THROTTLE_MAX));
	}
//RC-Channel 4	(analog)

//RC-Channel 5	(analog rotary encoder)
	ACCELERATION	= (map(RC_Channel[4][1], RCmin,RCmax,0,(int)THROTTLE_MAX));
//RC-Channel 6	(analog rotary encoder)

//RC-Channel 7	(2-way switch)
	if (RC_Channel[6][1] >= RCmid) {
		Blinker[5].dura = 1;
		Blinker[5].freq = 0;
	}
//RC-Channel 8	(2-way switch)
	if (RC_Channel[7][1] >= RCmid) {
		encoder_mode 	= true;
	}else {
		encoder_mode 	= false;
	}			
//RC-Channel 9	(3-way switch)
	if (RC_Channel[8][1] > RCmid+100) 	gear = 1;
	if ((RC_Channel[8][1] >= RCmid-100)&&(RC_Channel[8][1] <= RCmid+100)) 	gear = 0;
	if (RC_Channel[8][1] < RCmid-100) 	gear = -1;
//RC-Channel 10	(2-way switch)
	if (RC_Channel[9][1] >= RCmid) {
		run	 = false;
	}else {
		run = true;
	}
}
/////////////////////////////////
int init_RC_PWM() {
	for (int i=0;i<Anz_RC_Channels;i++) {
		RC_Channel[i][0] = RCPin[i]; 
		RC_Channel[i][1] = 0;
		pinMode (RC_Channel[i][0], INPUT);
		pullUpDnControl(RC_Channel[i][0],PUD_DOWN);
	}
	if(pthread_create(&t_RC_PWM_Control, NULL, RC_PWM_Thread, NULL)) {
	   	printf("Error creating thread t_RC_PWM_Control \n");
	   	return 1;
	}
}


#endif
