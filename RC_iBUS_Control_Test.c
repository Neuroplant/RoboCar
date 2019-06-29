//Test RC iBUS Control
#include <wiringPi.h>
#include <wiringPiI2C.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "constants.h"
#include "common.h"
#include "RC_iBUS_Control.h"

bool run = true;//, EncoderMode =true;




void setup() {
	if(wiringPiSetup() == -1){ 
        	printf("setup wiringPi faiservo !");
        };
	init_RC_iBUS_Control();
}

void loop() {
	for (int i=0;i<Anz_RC_Channels;i++) {
		printf(" %i(%i):%i",i+1,(int)RCPin[i],(int)RC_Channel[i]);
	}
	printf("\n");
}

void main() {
	setup();
	while(run) loop();
}
	
	
