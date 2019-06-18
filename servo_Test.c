//Servo Test
#include <wiringPi.h>
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
        printf("setup wiringPi faiservo !");
	};
//	wiringPi I2C
	if(wiringPiI2CSetup(DEV_ID0) == -1){ 
       	printf("setup wiringPi I2C faiservo !");
       	}
// wiringPi PCA9685 (Servo Driver)	
	int fd = pca9685Setup(PIN_BASE0, DEV_ID0, HERTZ);
	if (fd < 0) {
		printf("Error in setup\n");
	}
	pinMode(DEV_ID0_enable,OUTPUT);
	digitalWrite(DEV_ID0_enable,LOW);
	for (int i=0;i<10;i++) servoInit(i+PIN_BASE0);
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
