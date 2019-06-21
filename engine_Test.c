//Test RC_PWM_Control
#include <wiringPi.h>
#include <wiringPiI2C.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "constants.h"
#include "common.h"

bool run = true;
float Spin_Target 	= 0;
int steering = 0, throttle = 0, gear = 1;
#include "engine.h"
#include "encoder.h"

void setup() {
	if(wiringPiSetup() == -1){ 
        	printf("setup wiringPi faiservo !");
        };
	if(wiringPiI2CSetup(DEVGY1) == -1){ 
       	printf("setup wiringPi I2C faiservo !");
	};
	init_Encoder();
	init_Engine();
	delay(500);
}

void loop() {
		
		for (int i= -10; i<=10; i++) {
			steering=i;
			printf(" Lenkung : %i\n",i);
			delay(300);
		}
		for (int i= 10; i>=-10; i--) {
			steering=i;
			printf(" Lenkung : %i\n",i);
			delay(300);
		}
		steering=0;
		
		gear=1;
		EncoderMode=true;		
		for (int i= -SPIN_MAX; i<=SPIN_MAX; i+=50) {
			Spin_Target=i;
			delay(300);
			printf("1: Target : %i(%i)  Throttle: %i\n",Spin_Target,(int)Spin_Current(),throttle);
			delay(300);
			printf("2: Target : %i(%i)  Throttle: %i\n",Spin_Target,(int)Spin_Current(),throttle);
			delay(300);
		}
		for (int i= SPIN_MAX; i>=-SPIN_MAX; i-=50) {
			Spin_Target=i;
			delay(300);
			printf("1: Target : %i(%i)  Throttle: %i\n",Spin_Target,(int)Spin_Current(),throttle);
			delay(300);
			printf("2: Target : %i(%i)  Throttle: %i\n",Spin_Target,(int)Spin_Current(),throttle);
			delay(300);
		}
		
		EncoderMode=false;
		gear=1;
		for (int i=0; i<= THROTTLE_MAX,i++) {
			printf(" Throttle: %i\n",throttle,(int)Spin_Current());
			delay(300);
		}
		for (int i=THROTTLE_MAX; i>= 0,i--) {
			printf(" Throttle: %i\n",throttle,(int)Spin_Current());
			delay(300);
		}
		gear=-1;
		for (int i=0; i<= THROTTLE_MAX,i++) {
			printf(" Throttle: %i\n",throttle,(int)Spin_Current());
			delay(300);
		}
		for (int i=THROTTLE_MAX; i>= 0,i--) {
			printf(" Throttle: %i\n",throttle,(int)Spin_Current());
			delay(300);
		}
		gear=0;
			delay(100);
			printf("Stopp! %i\n",(int)Spin_Current());
			delay(300);		
}

void main() {
	setup();
	while(run) loop();
}
	
	
