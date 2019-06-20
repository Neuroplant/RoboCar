#ifndef RCCONTROL_H
#define RCCONTROL_H
#include "constants.h"
#include "commom.h"
unsigned int RC_Channel[Anz_RC_Channels][2];

void Control() {
//RC-Channel 1	(analog Stick)
	steering = map(RC_Channel[0][1],RCmin,RCmax,10,-10);
//RC-Channel 2	(analog Stick)

//RC-Channel 3	(analog Stick)
	if (EncoderMode) {
		Spin_Target = (map(RC_Channel[2][1],RCmin,RCmax,0,(int)SPIN_MAX));
	}else{
		throttle = (map(RC_Channel[2][1], RCmin,RCmax,0,(int)THROTTLE_MAX));
	}
//RC-Channel 4	(analog Stick)

//RC-Channel 5	(analog rotary encoder)
	ACCELERATION	= (map(RC_Channel[4][1], RCmin,RCmax,0,(int)THROTTLE_MAX));
//RC-Channel 6	(analog rotary encoder)

//RC-Channel 7	(2-way switch)
if (Anz_RC_Channels>=7) {
	if (RC_Channel[6][1] >= RCmid) {
		Blinker[5].dura = 1;
		Blinker[5].freq = 0;
	}
}
//RC-Channel 8	(2-way switch)
if (Anz_RC_Channels>=8) {
	if (RC_Channel[7][1] >= RCmid) {
		EncoderMode 	= true;
	}else {
		EncoderMode 	= false;
	}			
}
//RC-Channel 9	(3-way switch)
if (Anz_RC_Channels>=9) {
	if (RC_Channel[8][1] > RCmid+100) 	gear = 1;
	if ((RC_Channel[8][1] >= RCmid-100)&&(RC_Channel[8][1] <= RCmid+100)) 	gear = 0;
	if (RC_Channel[8][1] < RCmid-100) 	gear = -1;
}
//RC-Channel 10	(2-way switch)
if (Anz_RC_Channels>=10) {
	if (RC_Channel[9][1] >= RCmid) {
		run = true;
	}else {
		run = false;
	}
}
}

#endif
