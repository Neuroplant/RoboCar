#ifndef LINE_H
#define LINE_H

/* Line detector
int init_LineDetect(void)
int LineDetect(void)
*/

#include <wiringPi.h>
pthread_t t_Line_Control;


void *Line_Thread(void *value) {
	int last[Anz_RC_Channels];
	printf("Line_Thread start \n");
	while(run) {
		if ((digitalRead(LinePinL)==HIGH) && (digitalRead(LinePinR)==HIGH)){
			steering = 0;
			if (EncoderMode) {
				Spin_Target = SPIN_MAX/2;
			}else{
				throttle = THROTTLE_MAX/2;
				gear=1;
			}
		}
		if ((digitalRead(LinePinL)==LOW) && (digitalRead(LinePinR)==HIGH)) {
			steering = 10;
			if (EncoderMode) {
				Spin_Target = SPIN_MAX/3;
			}else{
				throttle = THROTTLE_MAX/3;
				gear=1;
			}
		}
		if ((digitalRead(LinePinL)==HIGH) && (digitalRead(LinePinR)==LOW)) { 
			steering = -10;
			if (EncoderMode) {
				Spin_Target = SPIN_MAX/3;
			}else{
				throttle = THROTTLE_MAX/3;
				gear=1;
			}
		}
		if ((digitalRead(LinePinL)==LOW) && (digitalRead(LinePinR)==LOW)) { 
			steering = 0;
			if (EncoderMode) {
				Spin_Target = SPIN_MAX/5;
			}else{
				throttle = THROTTLE_MAX/5;
				gear=-1;
			}
		}
	}
	printf("Line_Thread end\n");
	return NULL;
}

int init_LineDetect(void) {
	pinMode(LinePinL, INPUT);
	pullUpDnControl (LinePinL, PUD_DOWN);
	pinMode(LinePinR, INPUT);
	pullUpDnControl (LinePinR, PUD_DOWN);
	if(pthread_create(&t_Line_Control, NULL, Line_Thread, NULL)) {
	   	printf("Error creating thread t_Blinker %i\n",i);
	   	return 1;
		}
	return 0;
}
#endif

