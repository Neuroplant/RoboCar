//Read 10 PWM Channels

#include <wiringPi.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "common.h"
#include "constants.h"
#include "RC_PPM_Control.h"

int PWM_Channel[Anz_RC_Channels];


void setup() {
	wiringPiSetup ();
	for (int i=0;i<Anz_RC_Channels;i++) 
		PWM_Channel[i]=0; 
	pinMode (PPMPin, INPUT);
	pullUpDnControl(PPMPin,PUD_DOWN);	
}

void loop() {
	while(pulsln(PPMPin, HIGH,1000000) < 10000){} 		//wait
	for(int x=0; x<Anz_RC_Channels; x++) 			//read
		PWM_Channel[x]=pulsln(PPMPin, HIGH,1000000);
	
	for(int x=0; x<Anz_RC_Channels; x++) {			//output
		printf(" %i:%i ",x,PWM_Channel[x]);
		PWM_Channel[x]=0; 
	}
	printf("\n"); 
}


void main () {
	setup();
	while (1) loop();
}
