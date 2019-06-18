#ifndef TURRET_H
#define TURRET_H

#include <pthread.h>
#include "constants.h"
#include "servo.h"
#include "blinker.h"


int turr1X	= 0, turr1Y 	= -10, turret1 = 	0; 
void *TurretThread (void *value) {
	printf("Turret1 ready\n");
	while (run) {
		switch (turret1) {
			case 0 : 	// dont move
			break;
			case 1 : // center
				turr1X	 = 0;
				turr1Y = -10;
			break;
			case 3 :	// Laser on
				Sound[4].loop = 1;
				Blinker[0].dura = 10;
				Blinker[0].freq = 0;
			break;
			case 9 :	// laser off
				Blinker[0].dura = 0;
				Blinker[0].freq = 0;
			break;
			case 8 :	//up
				turr1Y++;
				delay(50);
			break;
			case 2 :	//down
				turr1Y--;
				delay(50);
			break;
			case 4 :	//left
				turr1X--;
				delay(100);
			break;
			case 6 :	//right
				turr1X++;
				delay(100);
			break;
			default :	// dont move;
			break;
		}
		if (turr1X > 10) turr1X = 10;
		if (turr1X <-10) turr1X =-10;
		if (turr1Y > 10) turr1Y = 10;
		if (turr1Y <-10) turr1Y =-10;
		
		servoWriteMS(servoPin_CX,map(turr1X,-10,10,SERVO_MIN_CX,SERVO_MAX_CX));
		servoWriteMS(servoPin_CY,map(turr1Y,-10,10,SERVO_MIN_CY,SERVO_MAX_CY));
	}
	servoWriteMS(servoPin_CX,map(0,-10,10,SERVO_MIN_CX,SERVO_MAX_CX));
	servoWriteMS(servoPin_CY,map(-10,-10,10,SERVO_MIN_CY,SERVO_MAX_CY));
	printf("Turret1 off\n");
	return NULL;
}

int init_Turret() {
	servoInit(servoPin_CX);	// X
	servoInit(servoPin_CY);	// Y
	pthread_t t_Turret;
        if(pthread_create(&t_Turret, NULL, TurretThread, NULL)) {
		printf("Error creating thread t_Turret\n");
		return 0;
	}
	return 1;
}

#endif
