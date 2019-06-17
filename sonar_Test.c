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
}

void loop() {
	printf(" Distance: %fcm \n",getSonar());
	printf("\n"); 
}


void main () {
	setup();
	while (1) loop();
}
