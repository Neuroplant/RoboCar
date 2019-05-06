/*
 * Author: Neuroplant
 *
 * Description:
 * control Car via LineFollower + Ultrasonich sensor
 *
 * Compile:
*  make Line
 *
 * Run:
 * sudo ./Line [m]
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
#include <linux/joystick.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>

#include "constants.h"

bool run		= true;

float Spin_Target 	= 0;


long map(long value,long fromLow,long fromHigh,long toLow,long toHigh){
    return (toHigh-toLow)*(value-fromLow) / (fromHigh-fromLow) + toLow;}


struct s_Sound {
	int loop;
	char name[20];	
};
struct s_Sound Sound[5];
pthread_t t_Sound[5];
bool SoundLock = false;

struct s_Blinker {
	int pin;
	int dura;
	float freq;
};
struct s_Blinker Blinker[10];
pthread_t t_Blinker[10];

void servoWriteMS(int pin, int ms);


//Car Funktions////////////////////////////////////////////////////////
// 	steering 	-	-10..10	-	left/right
//	throttle	-	0..PWM_MAX	-	motor power
//	gear		-	1/0/-1	-	forward/brake/reverse

int steering = 0, throttle = 0, gear = 1;

void *MotorThread(void *value);

//Sound/////////////////////////////////////////////////////////////////
//	sound[]		-	0..5	-	selects soundfile
//	sound[].Loop	-	0..	-	repeat soundfile



void *SoundThread(void *arg) {
	long idNr = (long)arg;
	int i;
	char soundfile[100];
	strcpy (soundfile, "omxplayer --no-keys -o local /home/pi/RoboCar/Sounds/");
	strcat(soundfile,Sound[idNr].name);
	printf("Sound %i: %s ready\n",idNr,Sound[idNr].name);
	while (run) {
		if (!SoundLock  && Sound[idNr].loop>0) {
			SoundLock=true;
			while (Sound[idNr].loop>0) {
				system(soundfile);
				Sound[idNr].loop--;
			}
			SoundLock=false;
			
		}
		if (SoundLock) Sound[idNr].loop=0;
	}
	printf("Sound %i end\n",i,Sound[idNr].name);
	return NULL;
}
int init_Sound (void) {
	int i=0;
	strcpy(Sound[0].name, "0-idle.mp3"); 
	strcpy(Sound[1].name, "1-Horn.mp3"); 
	strcpy(Sound[2].name, "2-BackupBeep.mp3"); 
	strcpy(Sound[3].name, "3-EngineStart.mp3"); 
	strcpy(Sound[4].name, "4-Laser.mp3"); 

	
	
	for (i=0;i<5;i++) {
		if(pthread_create(&t_Sound[i], NULL, SoundThread, (void*)i)) {
	   	printf("Error creating thread t_Sound %i\n",i);

	   	return 1;
		}
	}
	return 0;
}

//Blinker////////////////////////////////////////////////


void *BlinkerThread (void *arg) {
	long idNr = (long)arg;
	int cycles,i;
	printf("Blinker %i ready\n", idNr);
	for (int i=0; i <= PWM_MAX; i=i+10) pwmWrite(Blinker[idNr].pin,i);
	for (int i=PWM_MAX; i >= 0; i=i-20) pwmWrite(Blinker[idNr].pin,i);
	
	while (run) {
		if (Blinker[idNr].dura != 0) {
			cycles = (Blinker[idNr].freq * Blinker[idNr].dura);
			if (Blinker[idNr].freq == 0) {
				pwmWrite(Blinker[idNr].pin,PWM_MAX);
	    		delay(Blinker[idNr].dura * 1000);
	    		pwmWrite(Blinker[idNr].pin,0);
			} else {
				for (i=0;i<cycles;i++) {
					pwmWrite(Blinker[idNr].pin,PWM_MAX);
					delay(500 / Blinker[idNr].freq);
					pwmWrite(Blinker[idNr].pin,0);
					delay(500 / Blinker[idNr].freq);
				}
			}
		}
		Blinker[idNr].dura = 0;
	}
	printf("Blinker %i off\n", idNr);
	return NULL;
}

int init_Blinker (void) {
	Blinker[0].pin = laserPin; 
	Blinker[1].pin = blinkrechtsPin;
	Blinker[2].pin = blinklinksPin;
	Blinker[3].pin = blinkrechtsvPin;
	Blinker[4].pin = blinklinksvPin;
	Blinker[5].pin = frontlightPin;
	Blinker[6].pin = rearlightPin;

	
	int i=0;
	for (i=0;i<=6;i++) {
		pinMode(Blinker[i].pin,OUTPUT);
		if(pthread_create(&t_Blinker[i], NULL, BlinkerThread, (void*)i)) {
	   	printf("Error creating thread t_Blinker %i\n",i);
	   	return 1;
		}
	}
	return 0;
}

// 	Servo//////////////////////////////////////////////////////
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


float getSonar();

void *MCPThread (void *value) {
	printf("MCPThread start\n");
	while (run) {
		if ((digitalRead(linePinR)==HIGH)&&(digitalRead(linePinL)==LOW)) // links streifen verlassen und rechts auf treifen 	-> turn right
		{
			steering = 10;
			Blinker[1].dura = 2;
			Blinker[1].freq = 2;
			Blinker[3].dura = 2;
			Blinker[3].freq = 2;
		}
		if ((digitalRead(linePinR)==LOW)&&(digitalRead(linePinL)==HIGH))// rechts Streifen verlassen und links aus Streifen	-> turn left
		{
			steering = -10;
			Blinker[2].dura = 2;
			Blinker[2].freq = 2;
			Blinker[4].dura = 2;
			Blinker[4].freq = 2;					
		}
		if ((digitalRead(linePinR)==LOW)&&(digitalRead(linePinL)==LOW))// beide auf streifen 								-> go straight
		{
			gear = 1;
			Spin_Target = 2*(SPIN_MAX/3);
		}
		if ((digitalRead(linePinR)==HIGH)&&(digitalRead(linePinL)==HIGH))// beide Streifen verlassen							-> go backward
		{
			gear = -1;
			Spin_Target = SPIN_MAX/2;
		}
		
		if (getSonar() <= 20) //Hinderniss 										-> Stop, Alarm
		{
			gear = 0;
			Blinker[1].dura = 2;
			Blinker[1].freq = 2;
			Blinker[3].dura = 2;
			Blinker[3].freq = 2;
			Blinker[2].dura = 2;
			Blinker[2].freq = 2;
			Blinker[4].dura = 2;
			Blinker[4].freq = 2;
			Sound[1].loop=1;
		}
	
	}
	printf("MCPThread end\n");
	return NULL;
}

//Ultrasonic Obstacle avoidance
struct timespec Time1;
long StartTime, EndTime;   
long PulseLen (int inpin) {
	while (digitalRead(inpin) == LOW) {
		clock_gettime(CLOCK_REALTIME, &Time1);
		StartTime  = Time1.tv_nsec;
	}
	while (digitalRead(inpin) == HIGH) {
		clock_gettime(CLOCK_REALTIME, &Time1);
		EndTime = Time1.tv_nsec;
		if (EndTime-StartTime >= 13200) return 13200;
	}
	return (EndTime-StartTime);
}

float getSonar() {
	digitalWrite(trigPin,HIGH);
	delayMicroseconds(10);
	digitalWrite(trigPin,LOW);
	
	return ((float)PulseLen(echoPin) * 340.0 / 2.0 / 10000.0);
}
float getSonarP(int angle) {
	servoWriteMS(servoPin_US,angle);
	delay(100);
	return getSonar();
}
#include "motor.h"

int main (int argc, char *argv[]) {/////////////////////////////////////////////////////////////////////////////////////////
	int i;
	if (argc == 0) {
	    int encoder_mode=FALSE;
	}

//Setup

// wiringPi
	if(wiringPiSetup() == -1){ 
        printf("setup wiringPi faiservo !");
        return 1; 
	};
//I2C
	if(wiringPiI2CSetup(DEV_ID0) == -1){ 
       	printf("setup wiringPi I2C faiservo !");
       	return 1; 
	};
//ServoDriver
	int SDHandle = pca9685Setup(PIN_BASE0, DEV_ID0, HERTZ);
	if (SDHandle < 0) {
		printf("Error in setup\n");
		return SDHandle;
	}
	pinMode(DEV_ID0_enable,OUTPUT);
	digitalWrite(DEV_ID0_enable,LOW);
	
//LCD
	pcf8574Setup(PIN_BASE1,DEV_ID1);// initialize PCF8574 
	for(i=0;i<8;i++){ 
		pinMode(PIN_BASE1+i,OUTPUT); // set PCF8574 port to output mode 
	}
	digitalWrite(LED,HIGH); // turn on LCD backlight 
	digitalWrite(RW,LOW); // allow writing to LCD 
	int lcdhd = lcdInit(2,16,4,RS,EN,D4,D5,D6,D7,0,0,0,0);// initialize LCD and return “handle” used to handle LCD 
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
	//servoInit(servoPin_CX);	// X
	//servoInit(servoPin_CY);	// Y
	//pthread_t t_Turret;
        //if(pthread_create(&t_Turret, NULL, TurretThread, NULL)) {
	//		printf("Error creating thread t_Turret\n");
	//	return 1;
	//}
//Motor
	pthread_t t_Motor;
	if(pthread_create(&t_Motor, NULL, MotorThread, NULL)) {
		printf("Error creating thread t_Motor\n");
		return 1;
	}	
//Joystick init
	//js = open(device, O_RDONLY);
	/*while (js == -1) {
	    	js = open(device, O_RDONLY);
        	Blinker[5].dura = 2;
		Blinker[5].freq = 20;
		delay(2000);
		printf("Warte auf Joystick\n");
	};
	printf("Joystick ready \n");*/
//Joystick
	pthread_t t_Joystick;
		if(pthread_create(&t_Joystick, NULL, MCPThread, NULL)) {
			printf("Error creating thread t_Joystick\n");
			return 1;
		}	

//Ultraschall
	pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);
	pinMode(servoPin_US,OUTPUT);
//LineFollower
	pinMode(linePinR,INPUT);
	pinMode(linePinL,INPUT);
	
//Main-Loop Section
	printf("\n All Threads up: RemoteCar starting \n");

	while (run) {
		
		if (steering > 10) steering = 10;
		if (steering <-10) steering =-10;
		
		if ((Spin_Current() > Spin_Target)) throttle=throttle-ACCELERATION;
		if ((Spin_Current() < Spin_Target)) throttle=throttle+ACCELERATION;

		if (throttle > THROTTLE_MAX) throttle = THROTTLE_MAX;
		if (throttle < 0) gear=-1;
		if (throttle < -THROTTLE_MAX) throttle = -THROTTLE_MAX;
		
		if (getSonar() < 20) gear=0;
		
		
	 
		system("clear"); 
		lcdPosition(lcdhd,0,0);
		printf("Throttle %i Distance %fcm\n", throttle, getSonar());
		lcdPrintf(lcdhd,"TR %4i US %4f\n", throttle, getSonar());
		for (i=0;i<5;i++) {
			printf("Blinker: %i Pin: %i Frequenz: %2.3f Dauer: %i \n",i,Blinker[i].pin,Blinker[i].freq,Blinker[i].dura);
		 }
		for (i=0;i<5;i++) {
			printf("SoundNr.: %i Loop: %i \n",i,Sound[i].loop);
		 }
		printf("Turns per Secound: %5.2f/%5.2f \n",Spin_Current(),Spin_Target);
		lcdPrintf(lcdhd,"US%5.2f/%5.2f",Spin_Current(),Spin_Target);
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
//	pthread_join(t_Turret,NULL);
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
