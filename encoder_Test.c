// Encoder Test
#include <wiringPi.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "constants.h"
#include "common.h"
#include "encoder.h"

bool run = true;

void setup() {
	if(wiringPiSetup() == -1){ 
        printf("setup wiringPi faiservo !");
        return 1; 
	};
	init_Encoder();
}

void loop() {
	printf(" Direction: %i Spinn: %i \n",SpinDirection,Spin_Current());
}

void main() {
	setup();
	while(run) loop();
}
	