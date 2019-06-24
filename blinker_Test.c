//Test Blinker

#include <wiringPi.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "constants.h"
#include "common.h"
#include "servo.h"
#include "blinker.h"

bool run = true;


#include "blinker.h"


int setup() {
	if(wiringPiSetup() == -1){ 
        printf("setup wiringPi faiservo !");
        return 1; 
	};
	
	init_Servo(DEV_ID0,PIN_BASE0);
	init_Blinker();
}

void loop() {
	for (int i=0;i<7;i++) {
		Blinker[i].freq = 	2; //Hz
		Blinker[i].dura	=	2; //Secounds
		printf(" Blinker Nr %i \n",i);
		delay(2000);
	}
	printf("\n");
}

void main() {
	setup();
	while(run) loop();
}
	
	
