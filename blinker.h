#ifndef BLINKER_H
#define BLINKER_H
#include <pthread.h>
#include <wiringPiI2C.h>
#include <pca9685.h>	//PWM
#include "servo.h"

struct s_Blinker {
	int pin;
	int dura;
	float freq;
};
struct s_Blinker Blinker[10];
pthread_t t_Blinker[10];


void *BlinkerThread (void *arg) {
	long idNr = (long)arg;
	int cycles,i;
	printf("Blinker %i ready\n", idNr);
	pinMode(Blinker[i].pin,OUTPUT);
//	for (int i=0; i <= PWM_MAX; i=i+10) pwmWrite(Blinker[idNr].pin,i);
//	for (int i=PWM_MAX; i >= 0; i=i-20) pwmWrite(Blinker[idNr].pin,i);
	
	while (run) {
		if (Blinker[idNr].dura != 0) {
			cycles = (Blinker[idNr].freq * Blinker[idNr].dura);
			if (Blinker[idNr].freq == 0) {
				pwmWrite(Blinker[idNr].pin,PWM_MAX);
	    		delay(Blinker[idNr].dura * 1000);
	    		pwmWrite(Blinker[idNr].pin,0);
			} else {
				for (i=0;i<cycles;i++) {
					pwmWrite(Blinker[idNr].pin,PWM_MAX);
					delay(500 / Blinker[idNr].freq);
					pwmWrite(Blinker[idNr].pin,0);
					delay(500 / Blinker[idNr].freq);
				}
			}
		}
		Blinker[idNr].dura = 0;
	}
	printf("Blinker %i off\n", idNr);
	return NULL;
}

int init_Blinker (void) {
	Blinker[0].pin = laserPin; 
	Blinker[1].pin = blinkrechtsPin;
	Blinker[2].pin = blinklinksPin;
	Blinker[3].pin = blinkrechtsvPin;
	Blinker[4].pin = blinklinksvPin;
	Blinker[5].pin = frontlightPin;
	Blinker[6].pin = rearlightPin;
	int i=0;
	for (i=0;i<=6;i++) {
		
		if(pthread_create(&t_Blinker[i], NULL, BlinkerThread, (void*)i)) {
	   	printf("Error creating thread t_Blinker %i\n",i);
	   	return 0;
		}
	}
	return 1;
}
#endif /* BLINKER_H */
