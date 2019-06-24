#ifndef SERVO_H
#define SERVO_H

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <pca9685.h>	
#include "common.h"
#include "constants.h"

int servoInit(int pin){        		//initialization function for servo PMW pins
	pinMode(pin,OUTPUT);
	printf("Servo Pin %i OK\n",pin);
	return 1;
}

void servoWriteMS(int pin, int ms){     //specific the unit for pulse(5-25ms) with specific duration output by servo pin: 0.1ms
    if(ms > SERVO_MAX_MS) {
        printf("Pin: %i ms: %i too big\n",pin,ms);
        ms = SERVO_MAX_MS;
    };
    if(ms < SERVO_MIN_MS) {
        printf("Pin: %i ms: %i too small\n",pin,ms);
        ms = SERVO_MIN_MS;
    };
    pwmWrite(pin,map(ms,0,200,0,PWM_MAX));
	delay(10);
}

int init_Servo(int devID,int pinBase) {
	if(wiringPiI2CSetup(devID) == -1){ 
    printf("setup wiringPi I2C failed !");
    }
	int fd = pca9685Setup(pinBase, devID, HERTZ);
	if (fd < 0) {
		printf("setup PCA9685 failed !");
	}
	pca9685PWMReset(fd);
	pinMode(DEV_ID0_enable,OUTPUT);
	digitalWrite(DEV_ID0_enable,LOW);
	for (int i=0;i<=16;i++) servoInit(i+pinBase);
	return 1;
}
#endif
