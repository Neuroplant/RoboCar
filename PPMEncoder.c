/*PPM encoder*/
#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include "constants.h"

unsigned int currentChannel = 0;
unsigned int previousTick;
unsigned int deltaTime;
unsigned int channels[ppmChannelsNumber];

void ppmEdge(void) {
	int level = digitalRead(ppmInputPin);
	unsigned int tick = millis();
	if (level == LOW) {
		deltaTime = tick - previousTick;
		previousTick = tick;
		
		if (deltaTime >= ppmSyncLength) {
			currentChannel = 0;
			printf("\n");
			for (int i = 0; i < ppmChannelsNumber; i++)
				printf("%4.f ", channels[i]);
		}
	}else {
		if (currentChannel < ppmChannelsNumber)
			channels[currentChannel++] = deltaTime;
	}
}

void setup() {
	wiringPiSetup ();
	pinMode (ppmInputPin, INPUT);
	previousTick = millis();
	if ( wiringPiISR (ppmInputPin, INT_EDGE_BOTH, &ppmEdge) < 0 ) {
		printf("CheckDir failed!");
	}
}

int main(){
	setup();
	while(1)
		sleep(10);
	return 0;
}
