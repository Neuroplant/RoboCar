//Read 10 PWM Channels

#include <wiringPi.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "common.h"
#define PPMPin	29
#define Anz_PWM_Channels	8

int PWM_Channel[Anz_PWM_Channels];

int pulsln(int pin,bool level, int timeout) {
  unsigned int numloops = 0;

  while (digitalRead(pin) == level)
  {
    if (numloops++ == timeout)
      return 0 ;
  }

  while (digitalRead(pin) != level)
  {
    if (numloops++ == timeout)
      return 0 ;
  }

  int timerStart = micros();
  while (digitalRead(pin) == level)
  {
    if (numloops++ == timeout)
      return 0 ;
  }

  return micros() - timerStart ;
}

void setup() {
	wiringPiSetup ();
	for (int i=0;i<Anz_PWM_Channels;i++) 
		PWM_Channel[i]=0; 
	pinMode (PPMPin, INPUT);
	pullUpDnControl(PPMPin,PUD_DOWN);	
}

void loop() {
	while(pulseln(PPMPin, HIGH,1000000) < 11000){}
	for(int x=0; x<Anz_PWM_Channels; x++) 
		PWM_Channel[x]=pulseln(PPMPin, HIGH,1000000);
	for(int x=0; x<Anz_PWM_Channels; x++) {
		printf(" %i:%i ",x,PWM_Channel[x]);
		PWM_Channel[x]=0; 
	}
	printf("\n"); 
}


void main () {
	setup();
	while (1) loop();
}
