#IFNDEF RCCONTOL_H
#define RCCONTOL_H
/*PPM encoder*/
#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>

unsigned int currentChannel = 0;
unsigned int previousTick;
unsigned int deltaTime;
unsigned int channel[ppmChannelsNumber];

void ppmEdge(void) {
	int level = digitalRead(ppmInputPin);
	unsigned int tick = millis();
	if (level == LOW) {
		deltaTime = tick - previousTick;
		previousTick = tick;
		
		if (deltaTime >= ppmSyncLength) {
			currentChannel = 0;
//			printf("\n");
//			for (int i = 0; i < ppmChannelsNumber; i++)
//				printf("%4.f ", channel[i]);
		}
	}else {
		if (currentChannel < ppmChannelsNumber)
			channel[currentChannel++] = deltaTime;
	}
}


void *RCThread (void *value) {

	printf("RCThread start\n");

	while (run) {
		steering = map(channel[1],RCmin,RCmax,10,-10);
		
		if (encoder_mode) {
			Spin_Target = (map(channel[2],RCmin,RCmax,0,(int)SPIN_MAX));
		}else{
			throttle = (map(channel[2], RCmin,RCmax,0,(int)THROTTLE_MAX));
		}
		if (channel[5] >= RCmid) {encoder_mode 	= true;}
		else {encoder_mode 	= false;}		
		if (channel[6] > RCmid) 	gear = 1;
		if (channel[6] = RCmid) 	gear = 0;
		if (channel[6] < RCmid) 	gear = -1;
		if (channel[7] >= RCmid) {	run	 = true;}
		else {run = false;}
		ACCELERATION	= (map(channel[8], RCmin,RCmax,0,(int)THROTTLE_MAX))
		if (channel[9] >= RCmid) {
			Blinker[5].dura = 1;
			Blinker[5].freq = 0;
		}	
	}
	printf("RCThread end\n");
	return NULL;
}

void init_RCControl() {
	pinMode (ppmInputPin, INPUT);
	pullUpDnControl (ppmInputPin, PUD_DOWN);
	previousTick = millis();
	wiringPiSR(ppmInputPin,INT_EDGE_BOTH, *ppmEdge();
	pthread_t t_RCControl;
		if(pthread_create(&t_RCControl, NULL, RCThread, NULL)) {

			printf("Error creating thread t_RCControl\n");

			return 1;

		}	
}


#endif
