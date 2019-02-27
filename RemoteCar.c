/*
 * Author: Neuroplant
 *
 * Description:
 * Reads joystick(Sixaxis3) events and controls Car.
 *
 * Compile:
gcc RemoteCar.c -o Remote -lwiringPi -lm -lpthread
 *
 * Run:
 * ./Remote [/dev/input/jsX]
 */
 
 // Include Section
#include <wiringPi.h>
#include <softPwm.h>
#include <fcntl.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/joystick.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>

// Pin definitions
#define motorPin1	21	//Motor Vorwärts        out digital
#define motorPin2	22  	//Motor Rückwärts       out digital
#define enablePin	23  	//Motor Geschwindigkeit out PWM
#define servoPin_CX	11  	//Camera X              out PWM
#define	servoPin_CY	10  	//Camera Y              out PWM
#define servoPin_ST	16  	//Lenkung (Steering)    out PWM
#define servoPin_US     6   	//Ultraschall-Servo     out PWM
#define trigPin         4   	//Ultraschall-Trigger   out digital
#define echoPin         5   	//Ultraschall-Echo      in  digital
#define laserPin	25  	//div LEDs              out digital
#define blinkrechtsPin  28  	//div LEDs              out digital
#define blinklinksPin   29  	//div LEDs              out digital
#define frontlightPin   27  	//div LEDs              out digital
#define rearlightPin    26  	//div LEDs              out digital

#define OFFSET_CX 0
#define OFFSET_CY 0
#define OFFSET_ST 0
#define OFFSET_US 0

#define	SERVO_MIN_MS	4
#define SERVO_MAX_MS	26

#define	SERVO_MIN_ST	10+OFFSET_ST
#define SERVO_MAX_ST	18+OFFSET_ST

#define	SERVO_MIN_US	7+OFFSET_ST
#define SERVO_MAX_US	23+OFFSET_ST

#define	SERVO_MIN_CX	5+OFFSET_CX
#define SERVO_MAX_CX	26+OFFSET_CX
#define	SERVO_MIN_CY	5+OFFSET_CY
#define SERVO_MAX_CY	15+OFFSET_CY

#define SPEED_MAX       100         	//  maxmax 128
#define BRAKE           30          	// Bremskraft

#define MAX_DISTANCE 	220         	//  define the maximum measured distance
#define timeOut MAX_DISTANCE*60     	//  calculate timeout according to the maximum measured distance
#define MIN_DISTANCE	40		//  cm bis Kollision unvermeidlich

int run      = 1;

struct s_Sound {
	int loop = 0;
	char name[20];	
}
struct s_Sound Sound[5];
pthread_t t_Sound[5]

struct s_Blinker {
	int pin;
	int dura;
	float freq;
};
struct s_Blinker Blinker[10];
pthread_t t_Blinker[10];

void servoWriteMS(int pin, int ms);

long map(long value,long fromLow,long fromHigh,long toLow,long toHigh){
    return (toHigh-toLow)*(value-fromLow) / (fromHigh-fromLow) + toLow;}


//Car Funktions////////////////////////////////////////////////////////
// 	steering 	-	-10..10	-	left/right
//	speed		-	0..100	-	motor power
//	gear		-	1/0/-1	-	forward/brake/reverse

int steering = 0, speed = 0, gear = 1;
void *MotorThread(void *value){
	printf("Motor on\n");
	while (run) {
		if(gear>0){
			digitalWrite(motorPin1,HIGH);
			digitalWrite(motorPin2,LOW);
		//printf("turn Forward...\n");
		}
		else{
			if (gear<=0){
				digitalWrite(motorPin1,LOW);
				digitalWrite(motorPin2,HIGH);
		//printf("turn Back...\n");
				soundNr = 2;
				soundLoop = 1;
			}
		}
		if (gear == 0 ){
			softPwmWrite(enablePin,BRAKE);
			Blinker[4].dura = 1;
			Blinker[4].freq = 0;
		}else{
			softPwmWrite(enablePin,abs(speed));
		}
		servoWriteMS(servoPin_ST,map(steering,10,-10,SERVO_MIN_ST,SERVO_MAX_ST));
	}
	digitalWrite(motorPin1,LOW);
	digitalWrite(motorPin2,LOW);
	printf("Motor off\n");	
}

//Sound/////////////////////////////////////////////////////////////////
//	soundNr		-	0..5	-	selects soundfile
//	soundLoop	-	0..	-	repeat soundfile



void *SoundThread(void *value) {
	long idNr = (long)value, i, x;
	char soundfile[100];
	strcpy (soundfile, "omxplayer --no-keys -o local /home/pi/RoboCar/Sounds/");
	strcat(soundfile,Sound[idNr].name);
	printf("Sound %i: %c ready",i,Sound[idNr].name);
	while (run) {
		x = Sound[idNr].loop;
		for (i=0;i<x;i++) {
			system(soundfile);
			Sound[idNr].loop=0;
		}
	}
	printf("Sound %i: %c ready",i,Sound[idNr].name);
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
}

// Turret/////////////////////////////////////////////////
// turr1X	-	10..-10	-	x-axis 
// turr1Y	-	10..-10	-	y-axis
// turret1	-	0/1	-	lader off/on

int turr1X	= 0, turr1Y 	= -10, turret1 = 	0; 
void *TurretThread (void *value) {
	printf("Turret1 on\n");
	while (run) {
		switch (turret1) {
			case 0 : 	// dont move
			break;
			case 1 : // center
				turr1X	 = 0;
				turr1Y = -10;
			break;
			case 3 :	// Laser on
				soundNr = 4;
				soundLoop = 1;
				digitalWrite(laserPin,HIGH);
			break;
			case 9 :	// laser off
				digitalWrite(laserPin,LOW);
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
		servoWriteMS(servoPin_CX,map(turr1X,-10,10,SERVO_MIN_CX,SERVO_MAX_CX));
		servoWriteMS(servoPin_CY,map(turr1Y,-10,10,SERVO_MIN_CY,SERVO_MAX_CY));
	}
	printf("Turret1 off\n");
	return NULL;
}

//Blinker////////////////////////////////////////////////


void *BlinkerThread (void *arg) {
	long idNr = (long)arg;
	int cycles,i;
	printf("Blinker %i on\n", idNr);
	while (run) {
		if (Blinker[idNr].dura != 0) {
			cycles = (Blinker[idNr].freq * Blinker[idNr].dura);
			if (Blinker[idNr].freq == 0) {
				digitalWrite(Blinker[idNr].pin,HIGH);
	    			delay(Blinker[idNr].dura * 1000);
	    			digitalWrite(Blinker[idNr].pin,LOW);
			} else {
				for (i=0;i<cycles;i++) {
					digitalWrite(Blinker[idNr].pin,HIGH);
					delay(500 / Blinker[idNr].freq);
					digitalWrite(Blinker[idNr].pin,LOW);
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
	Blinker[3].pin = frontlightPin;
	Blinker[4].pin = rearlightPin;
	
	int i=0;
	for (i=0;i<=4;i++) {
		pinMode(Blinker[i].pin,OUTPUT);
		if(pthread_create(&t_Blinker[i], NULL, BlinkerThread, (void*)i)) {
	   	printf("Error creating thread t_Blinker %i\n",i);
	   	return 1;
		}
	}
}

// 	Servo//////////////////////////////////////////////////////
void servoInit(int pin){        		//initialization function for servo PMW pins
	pinMode(pin,OUTPUT);
	softPwmCreate(pin,  0, 200);
	printf("Pin %i OK\n",pin);
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
    softPwmWrite(pin,ms);
	delay(10);
}

// 	Sonar////////////////////////////////////////////////////////////////

struct timespec Time1, Time2;

void StartStopTimer (void) {
	if (digitalRead(echoPin)==HIGH) {
		clock_gettime(CLOCK_REALTIME, &Time1);
		StartTime = rTime.tv_nsec;
	}else{
		clock_gettime(CLOCK_REALTIME, &Time2);
		EndTime = rTime.tv_nsec;
	}
}
		
float getSonar(void) {
	// send trigger signal
	digitalWrite(trigPin,HIGH);
    	delayMicroseconds(10);
	digitalWrite(trigPin,LOW);
    	delayMicroseconds(5);
	
	float puls = Time1.tv_nsec - Time2.tv_nsec;
	return (puls * 340.0 / 2.0 / 10000.0);
}
	
float getSonarP(int angle) {
	float distance;
	servoWriteMS(servoPin_US,angle);
	delay(10);
	distance = getSonar();
	printf(" %3.2f cm an Position %i \n",distance,angle);
	return distance;
}


// 	Inettigence?
int freeDirection() {
	int i, free;
	float rSum=0, lSum=0;
	if (getSonarP((SERVO_MIN_US+SERVO_MAX_US)/2) >= MIN_DISTANCE) {
		free = 0;
		printf(" vorne %i \n--> Weg frei ",free);
	}else {
		gear=0; //Stopp während der messung
		delay(100);
		for (i=SERVO_MIN_US;i<(SERVO_MIN_US+SERVO_MAX_US)/2;i++) {
			rSum=rSum+getSonarP(i);
		}
		printf(" Rechts : %6f ",rSum);
		for (i=SERVO_MAX_US;i>(SERVO_MIN_US+SERVO_MAX_US)/2;i--) {
			lSum=lSum+getSonarP(i);
		}
		printf(" Links : %6f\n",lSum);
		if (rSum>lSum) {
			free = 1; 
			printf("--> Rechts frei \n");
		}
		if (rSum<=lSum) {
			free = 2;
			printf("--> Link s frei \n");
		}
		if (lSum+rSum < (SERVO_MAX_US-SERVO_MIN_US)* MIN_DISTANCE) {
			free = 3;
			printf("--> Sackgasse   \n");
			Blinker[1].dura = 5;
			Blinker[1].freq = 2;
			Blinker[2].dura = 5;
			Blinker[2].freq = 2;
		};
	};
	printf("free %i\n",free);
	return free;
}

//	JoyStick
const char *device;
int js;
struct js_event event;
size_t axis;

int read_event(int fd, struct js_event *event) {
    ssize_t bytes;
    bytes = read(fd, event, sizeof(*event));
    if (bytes == sizeof(*event))
        return 0;
    /* Error, could not read full event. */
    return -1;
}

size_t get_axis_count(int fd) {
    __u8 axes;
    if (ioctl(fd, JSIOCGAXES, &axes) == -1)
        return 0;
    return axes;
}

size_t get_button_count(int fd) {
    __u8 buttons;
    if (ioctl(fd, JSIOCGBUTTONS, &buttons) == -1)
        return 0;
    return buttons;
}


int StickControl(int stick, int value) {
	switch (stick) {
		case 0 :	//L3 Left/Right
		break;
		case 1 :	//L3 Up/Down
				
		break;
		case 2 :	//L2 Pull
		//	speed=(map(value,-32767,32767,-SPEED_MAX,0));		//Rückwärts
		break;
		case 3 :	//R3 Left/Right
				steering=(map(value,32767,-32767,10,-10));
					//Blinker
				if (steering == 10 ) {
					Blinker[1].dura = 2;
					Blinker[1].freq = 2;
				}
				if (steering == -10 ) {
					Blinker[2].dura = 2;
					Blinker[2].freq = 2;
					
				}
		break;
		case 4 :	//R3 Up/Down
		break;
		case 5 :	//R2 Pull
			speed=(map(value,-32767,32767,0,SPEED_MAX));		//Vorwärts
		break;
	}
	return 0;
}

int ButtonControl (int button, int value) {
//  PRESS							RELEASE
/*
	X		Hupe					-
	O    	Scheinwerfer (60s)		-
	Dreieck	Laser Beam on			Laser Beam off			
	Quadrat	-
	Up	move Turret1 up			Turret1 Stop
	Down	move Turret1 down		Turret1 Stop
	Left	move Turret1 left		Turret1 Stop
	Right	move Turret1 right		Turret1 Stop
	L1	Rückwärts			Vorwärts
	L2		
	L3		-
	R1		Brake                   Brake off
	R2		-
	R3		-
	Select	-
	Option	-
	PS		Quit
*/
	if (event.value==1) {
		switch (button) {
			case 0 :		//	X
				soundNr = 1;
				soundLoop = 2;
			break;
			case 1	:		//	O
					//Scheinwerfer
				Blinker[3].dura = 60;
				Blinker[3].freq = 0;
			break;
			case 2 :	//Dreieck
				turret1 = 3;
				break;
			case 4	:	//L1
				gear = -1;
			break;
					
			//Turret1 Control
			case 13 :	//UP
			   	turret1 = 8;
			break;
			case 14 :	//DOWN
    				turret1 = 2;
			break;
			case 15 :	//LEFT
			    	turret1 = 4;
			break;
			case 16 :	//RIGHT
    				turret1 = 6;
			break;
			case 5 :    //R1
                    		gear = 0;
			break;
			case 6 :	//L2
			break;
			case 10 :	//PS
				run = 0;
			break;
			default :	//noch ohne Funktion
			printf("Button Nr.%i pressed\n", button);
			}
	} else {
		switch (button) {
			case 0 :		//	X
			break;
			case 1	:		//	O
			break;
			case 2 :		//Dreieck
    				turret1 = 9;
			break;
			case 4	:		//L1
				gear = 1;
			break;
			//Turret1 Control
			case 13 :		//UP
				turret1 =  0;
			break;
			case 14 :		//DOWN
				turret1 =  0;
			break;
			case 15 :		//LEFT
				turret1 =  0;
			break;
			case 16 :		//RIGHT
				turret1 =  0;
			break;
			case 5 :    		//R1
				gear = 1;
				speed = 0;
			break;
			case 10 :	//PS
			break;
			default :	//noch ohne Funktion
			printf("Button Nr.%i released\n", button);
		}
		
	}
	return 0;
}

void *StickThread (void *value) {
	printf("StickThread start\n");
	while ((read_event(js, &event) == 0)&&(run)) {
		switch (event.type){
			case JS_EVENT_BUTTON:
				ButtonControl(event.number,event.value);
			break;
			case JS_EVENT_AXIS:
				StickControl(event.number,event.value);
			break;
		}
	}
	printf("StickThread end\n");
	return NULL;
}


	
// Setup
int Setup () {

// wiringPi
	if(wiringPiSetup() == -1){ 
        printf("setup wiringPi faiservo !");
        return 1; 
    };
// Input
	
	//Ultraschall

	
// Output
	//OnOff
	
	//Servos
	servoInit(servoPin_CX);	//Camera X
	servoInit(servoPin_CY);	//Camera Y
	servoInit(servoPin_ST);	//Lenkung
	servoInit(servoPin_US);	//Ultraschall
	
	//Motor
	pinMode(enablePin,OUTPUT);
	pinMode(motorPin1,OUTPUT);
	pinMode(motorPin2,OUTPUT);
	softPwmCreate(enablePin,0,SPEED_MAX);

//Sound
	init_Sound();
//Blinker
	init_Blinker();
	
//Joystick init
	js = open(device, O_RDONLY);
	while (js == -1) {
	    	js = open(device, O_RDONLY);
        	Blinker[3].dura = 2;
		Blinker[3].freq = 20;
		delay(2000);
		printf("Warte auf Joystick\n");

	};
	printf("Joystick ok\n");
	
//Joystick
	pthread_t t_Joystick;
		if(pthread_create(&t_Joystick, NULL, StickThread, NULL)) {
			printf("Error creating thread t_Joystick\n");
			return 1;
		}
		
//Sound 
	pthread_t t_playSound;
	if(pthread_create(&t_playSound, NULL, SoundThread, NULL)) {
		printf("Error creating thread t_playSound\n");
		return 1;
	}

//Turret
	pthread_t t_Turret;
        if(pthread_create(&t_Turret, NULL, TurretThread, NULL)) {
		printf("Error creating thread t_Turret\n");
		return 1;
	}

	return 0;
//Motor
	pthread_t t_Motor;
	if(pthread_create(&t_Motor, NULL, MotorThread, NULL)) {
		printf("Error creating thread t_Motor\n");
		return 1;
	}
		
//Sonar
	pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);
	wiringPiISR (echoPin, INT_EDGE_BOTH, &StartStopTimer) ;
}

int main (int argc, char *argv[]) {
	if (argc > 1) {
		device = argv[1];
	} else {
		device = "/dev/input/js0";
		
	}	//Setup
	Setup();
	
		//Main-Loop Section
	while (run) {
		if (steering > 10) steering = 10;
		if (steering <-10) steering =-10;
		if (speed > SPEED_MAX) speed = SPEED_MAX;
		if (speed <0) speed =0;
		//SubmitMotor ( steering, speed);
		if (turr1X > 10) turr1X = 10;
		if (turr1X <-10) turr1X =-10;
		if (turr1Y > 10) turr1Y = 10;
		if (turr1Y <-10) turr1Y =-10;
		//SubmitTurr1 ( turr1X, turr1Y);
		clrscr();
		printf("Turm1 %i,%i, Speed %i Lenkrad $i\n", turr1X, turr1Y, speed, steering);
		for (i=0;i<5;i++) {
			printf("Blinker: %i Pin: %i Frequenz: %2.3f Dauer: %i \n",i,Blinker[i].pin,Blinker[i].freq,Blinker[i].dura);
		}
		for (i=0;i<5;i++) {
			printf("SoundNr.: %i Loop: %i \n",i,Sound[i].loop);
		}
	}
		
//End Section

    	close(js);
	speed = 0;
	printf("\n Wait for threads to close...\n");
	delay(1000);		       
	return 0;
}
