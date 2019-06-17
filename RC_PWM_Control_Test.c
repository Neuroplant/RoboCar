//Test RC_PWM_Control
#include <wiringPi.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "constants.h"
#include "common.h"
#include "RC_PWM_Control.h"

bool run = true;

void setup() {
	if(wiringPiSetup() == -1){ 
        printf("setup wiringPi faiservo !");
        return 1; 
	};
	init_RC_PWM();
}

void loop() {
	for (int i=0;i<Anz_RC_Channels;i++) {
		printf(" %i(%i):%i",i+1,RC_Channel[i][0],RC_Channel[i][1]);
	}
	printf("\n");
}

void main() {
	setup();
	while(run) loop();
}
	
	