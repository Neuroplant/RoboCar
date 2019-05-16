/*
 * Author: Neuroplant
 *
 * Description:
 * Reads joystick(Sixaxis3) events and controls Car.
 *
 * Compile:
*  make Line
 *
 * Run:
 * sudo ./Line 
 */
 
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

#include <pthread.h>
#include <string.h>
#include <stdbool.h>

#include "constants.h"

bool run			= true;
float Spin_Target 	= 0;
int steering = 0, throttle = 0, gear = 0;

long map(long value,long fromLow,long fromHigh,long toLow,long toHigh){
    return (toHigh-toLow)*(value-fromLow) / (fromHigh-fromLow) + toLow;}

#include "servo.h"

#include "sound.h"

#include "blinker.h"

#include "encoder.h"

#include "engine.h"

#include "line.h"

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
//engine
	init_Engine();
//Sonar
	init_Sonar();
//Line detect
	init_LineDetect();
	
//Main-Loop Section
	printf("\n All Threads up: LineCar starting \n");
	while (run) {
		//Steuerung via LineDetect
		
		if (LineDetect() == 0) { gear = 1; 	steering = 0;	Spin_Target = SPIN_MAX/2;}
		if (LineDetect() == 1) { gear = 1; 	steering = 10;	Spin_Target = SPIN_MAX/3;}
		if (LineDetect() == 2) { gear = 1; 	steering = -10;	Spin_Target = SPIN_MAX/3;}
		if (LineDetect() == 3) { gear = -1;	steering = 0;	Spin_Target = SPIN_MAX/5;}

		//if (getSonar() < 10) gear=0; //Emergency Break
		
		//OUTPUT

		lcdPosition(lcdhd,0,0);
		lcdPrintf(lcdhd,"TR %4i US %4f\n", throttle, getSonar());
		lcdPrintf(lcdhd,"US%5.2f/%5.2f",Spin_Current(),Spin_Target);

		system("clear"); 
		printf("Throttle %i Distance %fcm\n", throttle, getSonar());
		printf("LineDetect: %i Steering: %i\n",LineDetect(),steering);
		printf("Turns per Secound: %5.2f/%5.2f \n",Spin_Current(),Spin_Target);
	}
		
//End Section

    //close(js);
	run=false;
	throttle = 0;
	printf("\n Wait for threads to close\n");

	delay(2000);

	printf("OK\n");		       
	return 0;
}
