#include <wiringPi.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "common.h"
#include "constants.h"
#include "sonar.h"

void setup() {
	wiringPiSetup ();
	init_Sonar();
	for (int i=0;i<Anz_PWM_Channels;i++) 
		PWM_Channel[i]=0; 
	pinMode (PPMPin, INPUT);
	pullUpDnControl(PPMPin,PUD_DOWN);	
}

void loop() {
	printf(" Distance: %fcm \n",getSonar());
	printf("\n"); 
}


void main () {
	setup();
	while (1) loop();
}