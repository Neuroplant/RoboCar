//Servo Test
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <pca9685.h>	

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "constants.h"
#include "common.h"
#include "servo.h"
bool run = true;
void setup() {
// 	wiringPi
	if(wiringPiSetup() == -1){ 
        printf("setup wiringPi failed !");
	};
	
	init_Servo(DEV_ID0,PIN_BASE0);
}
void loop() {
	for (int i=0;i<10;i++) {
		printf(" Servo Nr.%i \n",i);
		for (int ms= SERVO_MIN_MS; ms <= SERVO_MAX_MS; ms++) {
			servoWriteMS(i+PIN_BASE0, ms);
			delay(200);
			printf(".");
		}
		for (int ms= SERVO_MAX_MS; ms >= SERVO_MIN_MS; ms--) {
			servoWriteMS(i+PIN_BASE0, ms);
			delay(200);
			printf(".");
		}
		servoWriteMS(i+PIN_BASE0, (SERVO_MAX_MS+SERVO_MIN_MS)/2);
		printf("\n");
	}
	
}
void main() {
	setup();
	while(run) loop();
}
