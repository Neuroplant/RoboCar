//Test RC_PWM_Control
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <pca9685.h>	


#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "constants.h"
#include "common.h"

bool run = true;
float Spin_Target 	= 0;
int steering = 0, throttle = 0, gear = 1;
#include "encoder.h"
#include "engine.h"

void setup() {
	if(wiringPiSetup() == -1){ 
        	printf("setup wiringPi failed !");
        };
	if(wiringPiI2CSetup(DEV_ID0) == -1){ 
       	printf("setup wiringPi I2C failed !");
	};
	if (int PWMhd = pca9685Setup(PIN_BASE0, DEV_ID0, HERTZ)<0){
		printf("setup pca9685 failed !");
	}
	pca9685PWMReset(PWMhd);
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
		for (int i= -SPIN_MAX; i<=SPIN_MAX; i+=10) {
			Spin_Target=i;
			delay(300);
			printf("1: Target : %i(%i)  Throttle: %i\n",Spin_Target,(int)Spin_Current(),throttle);
			delay(300);
			printf("2: Target : %i(%i)  Throttle: %i\n",Spin_Target,(int)Spin_Current(),throttle);
			delay(300);
		}
		for (int i= SPIN_MAX; i>=-SPIN_MAX; i-=10) {
			Spin_Target=i;
			delay(300);
			printf("1: Target : %i(%i)  Throttle: %i\n",Spin_Target,(int)Spin_Current(),throttle);
			delay(300);
			printf("2: Target : %i(%i)  Throttle: %i\n",Spin_Target,(int)Spin_Current(),throttle);
			delay(300);
		}
		
		EncoderMode=false;
		gear=1;
		for (int i=0; i<= THROTTLE_MAX;i+=ACCELERATION) {
			throttle = i;
			printf(" Throttle: %i\n",throttle,(int)Spin_Current());
			delay(300);
		}
		for (int i=THROTTLE_MAX; i>= 0;i-=ACCELERATION) {
			throttle = i;
			printf(" Throttle: %i\n",throttle,(int)Spin_Current());
			delay(300);
		}
		gear=-1;
		for (int i=0; i<= THROTTLE_MAX;i+=ACCELERATION) {
			throttle = i;
			printf(" Throttle: %i\n",throttle,(int)Spin_Current());
			delay(300);
		}
		for (int i=THROTTLE_MAX; i>= 0;i-=ACCELERATION) {
			throttle = i;
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
	
	
