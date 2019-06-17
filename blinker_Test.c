//Test RC_PWM_Control
#include <wiringPi.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "constants.h"
#include "common.h"
#include "blinker.h"

bool run = true;

void setup() {
	if(wiringPiSetup() == -1){ 
        printf("setup wiringPi faiservo !");
        return 1; 
	};
	//	wiringPi I2C
	if(wiringPiI2CSetup(DEV_ID0) == -1){ 
       	printf("setup wiringPi I2C faiservo !");
       	return 1; 
	};
// wiringPi PCA9685 (Servo Driver)	
	int fd = pca9685Setup(PIN_BASE0, DEV_ID0, HERTZ);
	if (fd < 0) {
		printf("Error in setup\n");
		return fd;
	}
	pinMode(DEV_ID0_enable,OUTPUT);
	digitalWrite(DEV_ID0_enable,LOW);
	init_Blinker();
}

void loop() {
	for (int i=0;i<A7;i++) {
		Blinker[i].freq = 	2; //Hz
		Blinker[i].dura	=	2; //Secounds
		printf(" Blinker Nr %i \n",i);
		delay(2000)
	printf("\n");
}

void main() {
	setup();
	while(run) loop();
}
	
	