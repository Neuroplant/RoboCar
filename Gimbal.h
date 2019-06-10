// Gimbal für Robocar

#ifndef GIMBAL_H
#define GIMBAL_H

#include <wiringPi.h>
#include "gyro.h"
#include <wiringPiI2C.h>

#define DEVGY1  0x68 // 0x69 for AdressPin high

#define OFFSETGX_MS 0  
#define OFFSETGY_MS 0   
#define OFFSETGZ_MS 0    

#define SERVOGX_MIN_MS 5+OFFSETGX_MS        
#define SERVOGX_MAX_MS 25+OFFSETGX_MS       
#define SERVOGY_MIN_MS 5+OFFSETGY_MS        
#define SERVOGY_MAX_MS 25+OFFSETGY_MS       
#define SERVOGZ_MIN_MS 5+OFFSETGZ_MS        
#define SERVOGZ_MAX_MS 25+OFFSETGZ_MS  

int GyroHd;
int16_t ax, ay, az;     //store acceleration data
//int16_t gx, gy, gz;     //store gyroscope data

void *GimbalThread (void *value) {
	printf("Gimbal ready\n");
	
	while (run) {
		ax = accel_xout(GyroHd);
		ay = accel_yout(GyroHd);
		az = accel_zout(GyroHd);
		servoWriteMS(servoPin_GX,map(ax,-AccelMax  ,AccelMax  ,SERVOGX_MIN_MS,SERVOGX_MAX_MS));
		servoWriteMS(servoPin_GY,map(ay,-AccelMax  ,AccelMax  ,SERVOGY_MIN_MS,SERVOGY_MAX_MS));
		servoWriteMS(servoPin_GZ,map(az,-AccelMax  ,AccelMax  ,SERVOGZ_MIN_MS,SERVOGZ_MAX_MS));
	}
}

int init_Gimbal() {
	if(wiringPiI2CSetup(DEVGY1) == -1){ 
       	printf("setup wiringPi I2C faiservo !");
	};
	init_Gyro(DEVGY1,&GyroHd);
    printf("%i ",set_gyro_x(GyroHd));
    printf("%i ",set_gyro_y(GyroHd));
    printf("%i \n",set_gyro_z(GyroHd));
	pinMode(servoPin_GX,OUTPUT);
	pinMode(servoPin_GY,OUTPUT);
	pinMode(servoPin_GZ,OUTPUT);
	pthread_t t_Gimbal;
	if(pthread_create(&t_Gimbal, NULL, GimbalThread, NULL)) {
		printf("Error creating thread t_Gimbal\n");
		return 1;
	}
}
	
	
#endif
