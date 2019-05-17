#ifndef ENGINE_H
#define ENGINE_H

#include <wiringPi.h>
#include <pthread.h>
#include "constants.h"
#include "servo.h"

pthread_t t_Engine;

void *EngineThread(void *value){
	servoInit(motorPin1);
	servoInit(motorPin2);
	servoInit(servoPin_ST);

	printf("Engine ready\n");
	while (run) {
		if (steering > 10) steering = 10;
		if (steering <-10) steering =-10;
		
		if (Spin_Current() > Spin_Target)	throttle=throttle-ACCELERATION;
		if (Spin_Current() < Spin_Target) 	throttle=throttle+ACCELERATION;

		if (throttle > 0) 					gear= 1;
		if (throttle > THROTTLE_MAX) 		throttle = THROTTLE_MAX;
		if (throttle < 0) 					gear=-1;
		if (throttle < -THROTTLE_MAX) 		throttle = -THROTTLE_MAX;
		
		
		if(gear>0){
			pwmWrite(motorPin1,0);
			pwmWrite(motorPin2,abs(throttle));
		//printf("turn Forward...\n");
		}
		if (gear<0){
			pwmWrite(motorPin1,abs(throttle));
			pwmWrite(motorPin2,0);
		//printf("turn Back...\n");
			Sound[2].loop = 1;
		}
		if (gear == 0 ){
			pwmWrite(motorPin1,0);
			pwmWrite(motorPin2,0);
			Blinker[6].dura = 1;
			Blinker[6].freq = 0;
		}
		servoWriteMS(servoPin_ST,map(steering,10,-10,SERVO_MIN_ST,SERVO_MAX_ST));
	}
	pwmWrite(motorPin1,0);
	pwmWrite(motorPin2,0);
	printf("Motor off\n");	
}

int init_Engine(void){
	return pthread_create(&t_Engine, NULL, EngineThread, NULL);
}

#endif 
