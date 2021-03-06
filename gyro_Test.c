//Test RC_PWM_Control
#include <wiringPi.h>
#include <wiringPiI2C.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "constants.h"
#include "common.h"

bool run = true;
int GyHD;

#include "gyro.h"

void setup() {
	if(wiringPiSetup() == -1){ 
        	printf("setup wiringPi faiservo !");
        };
	if(wiringPiI2CSetup(DEVGY1) == -1){ 
       	printf("setup wiringPi I2C faiservo !");
	};
	init_Gyro(DEVGY1,GyHD);
	set_gyro(GyHD);
	set_accel(GyHD);
}

void loop() {
		printf("Gyro: x:%i y: z:%i \n",gyroskop_xout(GyHD),gyroskop_yout(GyHD),gyroskop_zout(GyHD));
		printf("Accel:x:%i y: z:%i \n",accel_xout(GyHD),accel_yout(GyHD),accel_zout(GyHD));
		printf("Spin: x:%i y: z:%i \n",get_x_rotation(GyHD),get_y_rotation(GyHD),get_z_rotation(GyHD));
}

void main() {
	setup();
	while(run) loop();
}
	
	
