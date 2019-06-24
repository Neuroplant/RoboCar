#ifndef ENCODER_H
#define ENCODER_H

#include "constants.h"
#include "common.h"
bool EncoderMode = true;

int PhaseCount, SpinDirection;
void PhaseCounter(void){
	PhaseCount++;
	if (digitalRead(phaseBPin) == HIGH) {
		SpinDirection = 1;
	}else{
		SpinDirection = -1;
	}
}
int Spin_Current (void){
	float rpmin;
	PhaseCount = 0;
	delay(100);
	//rpmin = ((PhaseCount/(float)Teeth)*600*SpinDirection);
	return PhaseCount*SpinDirection;
}
int init_Encoder(void) {
	pinMode(phaseAPin,INPUT);
	pullUpDnControl (phaseAPin, PUD_DOWN);
	pinMode(phaseBPin,INPUT);
	pullUpDnControl (phaseBPin, PUD_DOWN);
	wiringPiISR (phaseAPin, INT_EDGE_FALLING, &PhaseCounter);
	return 1;
}

#endif /* ENCODER_H */
