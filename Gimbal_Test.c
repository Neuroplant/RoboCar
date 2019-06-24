//Test RC_PWM_Control
#include <wiringPi.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "constants.h"
#include "common.h"
#include "servo.h"

bool run = true;

#include "Gimbal.h"

void setup() {
	if(wiringPiSetup() == -1){ 
        	printf("setup wiringPi faiservo !");
        };
	if(wiringPiI2CSetup(DEVGY1) == -1){ 
       	printf("setup wiringPi I2C faiservo !");
	};
	init_Servo(DEV_ID0,PIN_BASE0);
	init_Gimbal();
	delay(500);
}

void loop() {
		printf("Gyro: x:%i y: z:%i \n",gyroskop_xout(GyHD),gyroskop_yout(GyHD),gyroskop_zout(GyHD));
		
		//printf("Accel:x:%i y: z:%i \n",accel_xout(GyHD),accel_yout(GyHD),accel_zout(GyHD));
		//printf("Spin: x:%i y: z:%i \n",get_x_rotation(GyHD),get_y_rotation(GyHD),get_z_rotation(GyHD));
}

void main() {
	setup();
	while(run) loop();
}
	
	
