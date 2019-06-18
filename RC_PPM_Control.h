#IFNDEF RC_PPM_CONTOL_H
#define RC_PPM_CONTOL_H
/*PPM encoder*/
#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#define PPMPin	29
#define Anz_PWM_Channels	8
unsigned int channel[ppmChannelsNumber];
pthread_t t_RCControl;

void Control() {
//RC-Channel 1	(analog)
	steering = map(RC_Channel[0][1],RCmin,RCmax,10,-10);
//RC-Channel 2	(analog)

//RC-Channel 3	(analog)
	if (EncoderMode) {
		Spin_Target = (map(RC_Channel[2][1],RCmin,RCmax,0,(int)SPIN_MAX));
	}else{
		throttle = (map(RC_Channel[2][1], RCmin,RCmax,0,(int)THROTTLE_MAX));
	}
//RC-Channel 4	(analog)

//RC-Channel 5	(analog rotary encoder)
	ACCELERATION	= (map(RC_Channel[4][1], RCmin,RCmax,0,(int)THROTTLE_MAX));
//RC-Channel 6	(analog rotary encoder)

//RC-Channel 7	(2-way switch)
	if (RC_Channel[6][1] >= RCmid) {
		Blinker[5].dura = 1;
		Blinker[5].freq = 0;
	}
//RC-Channel 8	(2-way switch)
	if (RC_Channel[7][1] >= RCmid) {
		EncoderMode 	= true;
	}else {
		EncoderMode 	= false;
	}			
//RC-Channel 9	(3-way switch)
	if (RC_Channel[8][1] > RCmid+100) 	gear = 1;
	if ((RC_Channel[8][1] >= RCmid-100)&&(RC_Channel[8][1] <= RCmid+100)) 	gear = 0;
	if (RC_Channel[8][1] < RCmid-100) 	gear = -1;
//RC-Channel 10	(2-way switch)
	if (RC_Channel[9][1] >= RCmid) {
		run = true;
	}else {
		run = false;
	}
}

void *RCThread (void *value) {
	int last[Anz_RC_Channels];
	printf("RCThread start\n");
	while (run) {
		while(pulsln(PPMPin, HIGH,1000000) < 10000){} 		//wait
		for(int i=0; i<Anz_PWM_Channels; i++)
			PWM_Channel[i]=pulsln(PPMPin, HIGH,1000000);
		
		for(int i=0; i<Anz_PWM_Channels; i++) {
			if (RC_Channel[i] !=last[i]) 
				Control();
			last[i] = RC_Channel[i];
		}

	}
	printf("RCThread end\n");
	return NULL;
}

int init_RC_PPM_Control() {
	for (int i=0;i<Anz_PWM_Channels;i++) 
		PWM_Channel[i]=0; 
	pinMode (PPMPin, INPUT);
	pullUpDnControl(PPMPin,PUD_DOWN);	
	
	if(pthread_create(&t_RCControl, NULL, RCThread, NULL)) {
		printf("Error creating thread t_RCControl\n");
		return 0;
	}else{
		return 1;
	}
}


#endif
