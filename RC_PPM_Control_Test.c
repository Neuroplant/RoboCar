//Test RC_PWM_Control
#include <wiringPi.h>
#include <wiringPiI2C.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "constants.h"
#include "common.h"

/*int steering=0, Spin_Target=0, throttle=0, gear;
struct s_Blinker {
	int pin;
	int dura;
	float freq;
};
struct s_Blinker Blinker[10]; 
*/


bool run = true;//, EncoderMode =true;

unsigned int RC_Channel[Anz_RC_Channels];
int Control(void) {
		printf("\n");
}
#include "RC_PPM_Control.h"


void setup() {
	if(wiringPiSetup() == -1){ 
        	printf("setup wiringPi faiservo !");
        };
	init_RC_PWM();
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
	
	
