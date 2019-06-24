// Gimbal für Robocar

#ifndef GIMBAL_H
#define GIMBAL_H

#include <pthread.h>

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include "gyro.h"




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
	
	init_Gyro(DEVGY1,&GyroHd);
    printf("%i ",set_gyro_x(GyroHd));
    printf("%i ",set_gyro_y(GyroHd));
    printf("%i \n",set_gyro_z(GyroHd));
	pthread_t t_Gimbal;
	if(pthread_create(&t_Gimbal, NULL, GimbalThread, NULL)) {
		printf("Error creating thread t_Gimbal\n");
		return 1;
	}
}
	
	
#endif
