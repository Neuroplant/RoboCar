//Read 10 PWM Channels

#include <wiringPi.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define Anz_PWM_Channels	10

int PWM_Channel[Anz_PWM_Channels][2];

int pulsln(int pin,bool level, int timeout) {
	#define MAX_LOOPS 1000000
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
	for (int i=0;i<Anz_PWM_Channels;i++) {
		PWM_Channel[i][0] = 30-i; 
		pinMode (PWM_Channel[i][0], INPUT);
		pullUpDnControl(PWM_Channel[i][0],PUD_DOWN);
	}
}

void loop() {
	for (int i=0;i<Anz_PWM_Channels;i++) {
		PWM_Channel[i][1] = pulsln(PWM_Channel[i][0],HIGH,1000000); 
		printf(" %i(%i):%i",i+1,PWM_Channel[i][0],PWM_Channel[i][1]);
	}
	printf("\n");
			
}


void main () {
	setup();
	while (1) loop();
}