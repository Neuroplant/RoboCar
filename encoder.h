#ifndef ENCODER_H
#define ENCODER_H

#include <wiringPi.h>
#include <pthread.h>
#include "constants.h"

// 	AB-Phase-Encoder ////////////////////////////////////////////////////
#define Teeth		11	//number of teeth on the encoder wheel
#define SPIN_MAX	100 // max 260/min
int PhaseCount, SpinDirection;
void PhaseCounter(void){
	PhaseCount++;
	if (digitalRead(phaseBPin) == HIGH) {
		SpinDirection = 1;
	}else{
		SpinDirection = -1;
	}
}
float Spin_Current (void){
	float rpmin;
	PhaseCount = 0;
	delay(100);
	//rpmin = ((PhaseCount/(float)Teeth)*600*SpinDirection);
	return PhaseCount*SpinDirection;
}
int init_Encoder(void) {
	pinMode(phaseAPin,INPUT);
	pinMode(phaseBPin,INPUT);
	wiringPiISR (phaseAPin, INT_EDGE_FALLING, &PhaseCounter);
	return 0;
}

#endif /* ENCODER_H */
