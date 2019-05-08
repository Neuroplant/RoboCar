// 	AB-Phase-Encoder ////////////////////////////////////////////////////
#define Teeth		11	//number of teeth on the encoder wheel
#define MAX_SPIN	240 // max 260/min
static volatile int PhaseCount, SpinDirection;
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
	rpmin = ((PhaseCount/(float)Teeth)*600*SpinDirection);
	return rpmin;
}
void init_Encoder(void) {
	pinMode(phaseAPin,INPUT);
	pinMode(phaseBPin,INPUT);
	wiringPiISR (phaseAPin, INT_EDGE_FALLING, &PhaseCounter);
}