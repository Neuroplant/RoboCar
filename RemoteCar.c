/*
 * Author: Neuroplant
 *
 * Description:
 * Reads joystick(Sixaxis3) events and controls Car.
 *
 * Compile:
*  make Remote
 *
 * Run:
 * sudo ./Remote 
 */
 
 //Car Funktions////////////////////////////////////////////////////////
// 	steering 	-	-10..10	-	left/right
//	throttle	-	0..PWM_MAX	-	motor power
//	gear		-	1/0/-1	-	forward/brake/reverse
 
 //Sound/////////////////////////////////////////////////////////////////
//	sound[]		-	0..5	-	selects soundfile
//	sound[].Loop	-	0..	-	repeat soundfile



// Turret/////////////////////////////////////////////////
// turr1X	-	10..-10	-	x-axis 
// turr1Y	-	10..-10	-	y-axis
// turret1	-	0/1	-	laser off/on

 
 // Include Section
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <pca9685.h>	//PWM
#include <pcf8574.h>	//for LCD
#include <lcd.h>	
#include <fcntl.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>

#include "constants.h"

bool run			= true;
float Spin_Target 	= 0;
int steering = 0, throttle = 0, gear = 1;

long map(long value,long fromLow,long fromHigh,long toLow,long toHigh){
    return (toHigh-toLow)*(value-fromLow) / (fromHigh-fromLow) + toLow;}

int servoInit(int pin){        		//initialization function for servo PMW pins
	pinMode(pin,OUTPUT);
	printf("Servo Pin %i OK\n",pin);
	return 0;
}

void servoWriteMS(int pin, int ms){     //specific the unit for pulse(5-25ms) with specific duration output by servo pin: 0.1ms
    if(ms > SERVO_MAX_MS) {
        printf("Pin: %i ms: %i too big\n",pin,ms);
        ms = SERVO_MAX_MS;
    };
    if(ms < SERVO_MIN_MS) {
        printf("Pin: %i ms: %i too small\n",pin,ms);
        ms = SERVO_MIN_MS;
    };
    pwmWrite(pin,map(ms,0,200,0,PWM_MAX));
	delay(10);
}

#include "sound.h"

#include "blinker.h"

#include "engine.h"

//#include "turret.h"

#include "encoder.h"
bool encoder_mode = true;

//#include "line.h"


//#include "ps3_control.h"
#include "RCControl.h"

#include "sonar.h"

int main (int argc, char *argv[]) {/////////////////////////////////////////////////////////////////////////////////////////
	int i;
	

//Setup

// 	wiringPi
	if(wiringPiSetup() == -1){ 
        printf("setup wiringPi faiservo !");
        return 1; 
	};
//	wiringPi I2C
	if(wiringPiI2CSetup(DEV_ID0) == -1){ 
       	printf("setup wiringPi I2C faiservo !");
       	return 1; 
	};
// wiringPi PCA9685 (Servo Driver)	
	int fd = pca9685Setup(PIN_BASE0, DEV_ID0, HERTZ);
	if (fd < 0) {
		printf("Error in setup\n");
		return fd;
	}
	pinMode(DEV_ID0_enable,OUTPUT);
	digitalWrite(DEV_ID0_enable,LOW);
//	wiringPi PCF8574 (for LCD)
	pcf8574Setup(PIN_BASE1,DEV_ID1);// initialize PCF8574 
	for(i=0;i<8;i++){ 
		pinMode(PIN_BASE1+i,OUTPUT); // set PCF8574 port to output mode 
	}
	digitalWrite(LED,HIGH); // turn on LCD backlight 
	digitalWrite(RW,LOW); // allow writing to LCD 
//  wiringPi LCD
	int lcdhd = lcdInit(2,16,4,RS,EN,D4,D5,D6,D7,0,0,0,0);
	if(lcdhd < 0) {
		printf("lcdInit failed !"); 
		return lcdhd; 
	} 

// AB - Encoder
	init_Encoder();
//Sound
	init_Sound();
//Blinker
	init_Blinker();
//Turret
//	init_Turret();
//Motor
	pthread_t t_Motor;
	if(pthread_create(&t_Motor, NULL, MotorThread, NULL)) {
		printf("Error creating thread t_Motor\n");
		return 1;
	}	
	
//Joystick	
//	init_Joystick();
	
//RCControl
	init_RCControl();

//Sonar
	pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);
	pinMode(servoPin_US,OUTPUT);
//Line detect
//	init_LineDetect();
	
//Main-Loop Section
	printf("\n All Threads up: RemoteCar starting \n");
	while (run) {
		//Steuerung via joystick
		
		//if (getSonar() < 10) gear=0;
		
		//OUTPUT

		lcdPosition(lcdhd,0,0);
		lcdPrintf(lcdhd,"TR %4i US %4f\n", throttle, getSonar());
		lcdPrintf(lcdhd,"US%5.2f/%5.2f",Spin_Current(),Spin_Target);

		system("clear"); 
		printf("Throttle %i Distance %fcm\n", throttle, getSonar());
		for (i=0;i<5;i++) {
			printf("Blinker: %i Pin: %i Frequenz: %2.3f Dauer: %i \n",i,Blinker[i].pin,Blinker[i].freq,Blinker[i].dura);
		 }
		for (i=0;i<5;i++) {
			printf("SoundNr.: %i Loop: %i \n",i,Sound[i].loop);
		 }
		printf("Turns per Secound: %5.2f/%5.2f \n",Spin_Current(),Spin_Target);
	}
		
//End Section

    close(js);
	run=false;
	throttle = 0;
	printf("\n Wait for threads to close\n");
	pthread_join(t_Sound[0],NULL);
	printf(".");
	pthread_join(t_Sound[1],NULL);
	printf(".");
	pthread_join(t_Sound[2],NULL);
	printf(".");
	pthread_join(t_Sound[3],NULL);
	printf(".");
	pthread_join(t_Sound[4],NULL);
	printf(".");
	pthread_join(t_Motor,NULL);
	printf(".");
	pthread_join(t_Turret,NULL);
	printf(".");
	pthread_join(t_Joystick,NULL);
	printf(".");
	pthread_join(t_Blinker[0],NULL);
	printf(".");
	pthread_join(t_Blinker[1],NULL);
	printf(".");
	pthread_join(t_Blinker[2],NULL);
	printf(".");
	pthread_join(t_Blinker[3],NULL);
	printf(".");
	pthread_join(t_Blinker[4],NULL);
	printf(".");
	pthread_join(t_Blinker[5],NULL);
	printf(".");
	pthread_join(t_Blinker[6],NULL);
	printf(".");

	printf("OK\n");		       
	return 0;
}
