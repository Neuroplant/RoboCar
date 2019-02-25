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

#define motorPin1	    21	//Motor Vorwärts        out digital
#define motorPin2	    22  //Motor Rückwärts       out digital
#define enablePin	    23  //Motor Geschwindigkeit out PWM
#define servoPin_CX	    11  //Camera X              out PWM
#define	servoPin_CY	    10  //Camera Y              out PWM
#define servoPin_ST	    16  //Lenkung (Steering)    out PWM
#define servoPin_US     6   //Ultraschall-Servo     out PWM
#define trigPin         4   //Ultraschall-Trigger   out digital
#define echoPin         5   //Ultraschall-Echo      in  digital
#define laserPin	    25  //div LEDs              out digital
#define blinkrechtsPin  28  //div LEDs              out digital
#define blinklinksPin   29  //div LEDs              out digital
#define frontlightPin   27  //div LEDs              out digital
#define rearlightPin    26  //div LEDs              out digital

#define OFFSET_CX 0
#define OFFSET_CY 0

#define OFFSET_ST 0

#define OFFSET_US 0

#define	SERVO_MIN_MS	4
#define SERVO_MAX_MS	26

#define	SERVO_MIN_ST	9+OFFSET_ST
#define SERVO_MAX_ST	18+OFFSET_ST

#define	SERVO_MIN_US	7+OFFSET_ST
#define SERVO_MAX_US	23+OFFSET_ST

#define	SERVO_MIN_CX	5+OFFSET_CX
#define SERVO_MAX_CX	26+OFFSET_CX
#define	SERVO_MIN_CY	5+OFFSET_CY
#define SERVO_MAX_CY	15+OFFSET_CY

#define SPEED_MAX       100         //  maxmax 128
#define BRAKE           30          // Bremskraft


#define MAX_DISTANCE 	220         //  define the maximum measured distance
#define timeOut MAX_DISTANCE*60     //  calculate timeout according to the maximum measured distance
#define MIN_DISTANCE	40	        //  cm bis Kollision unvermeidlich

//sound
   
int soundNr = 3, soundLoop = 1;
    
    
int run      = 0;
int steering = 0, speed = 0, gear = 1;
int Speed_current = 0;

int turr1X	 = 0, turr1Y = -10, turret1 = 0; 

pthread_t t_blink[100];
int blink_t_counter = 0;

const char *device;
int js;
struct js_event event;
size_t axis;

float distance = 0;

void motor(int ADC);

long map(long value,long fromLow,long fromHigh,long toLow,long toHigh){
    return (toHigh-toLow)*(value-fromLow) / (fromHigh-fromLow) + toLow;
}

//multi Threading

void *blinkThread (void *input) {
    int *parameter = ((int*)input);
	int i;
	int pin 	= parameter[0];
	int dura	= parameter[1];
	int freq	= parameter[2];
	int cycles = (freq * dura);
//	printf(" Pin %i \n Frequenz %iHz \n Dauer %is\n",pin,freq,dura);
	if (freq == 0) {    		//Dauerlicht
	    digitalWrite(pin,HIGH);
	    delay(dura * 1000);
	    digitalWrite(pin,LOW);
	} else {                    	//Blinken
		for (i=0;i<cycles;i++) {
			digitalWrite(pin,HIGH);
			//printf("LED %i on...\n", pin);
			delay(500 / freq);
			digitalWrite(pin,LOW);
			//printf("LED %i off..\n", pin);
			delay(500 / freq);
		}
	}
	return NULL;
}
//Thread starter
int blink (int pin,int dura,int freq) {
	int *LEDArgs = (int*)malloc(3*sizeof(int));
	LEDArgs[0] = pin;
	LEDArgs[1] = dura;      //Dauer in Sekunden
	LEDArgs[2] = freq;     //Frequenz in Hz
	//pthread_join(t_blink[blink_t_counter],NULL);
	if(pthread_create(&t_blink[blink_t_counter], NULL, blinkThread, (void*)LEDArgs)) {
	   	printf("Error creating thread t_blink\n");
	   	return 1;
	}
	blink_t_counter++;
	if (blink_t_counter==99) blink_t_counter=0;
	return 0;
}

// Sound

void *SoundThread(void *value) {
    char soundfile[100] ;
	while (run==0) {
		
    if (soundLoop <= 0) soundNr = 0;
	strcpy (soundfile, "omxplayer --no-keys -o local /home/pi/RoboCar/Sounds/");
    switch (soundNr) {
        case 0 :    //engine running
            
        break;
        case 1 :    //Horn
            strcat(soundfile, "1-Horn.mp3");
        break;
        case 2 :    //RückwärtsBeep
            strcat(soundfile, "2-BackupBeep.mp3");
        break;
        case 3 :    //Engine start
            strcat(soundfile, "3-EngineStart.mp3");
        break;
        case 4 :    //Laser 
            strcat(soundfile, "4-Laser.mp3");
        break;
        case 5 :    //???
            strcat(soundfile, "XXX.mp3");
        break;
       default :    //engine running
            strcat(soundfile, "0-idle.mp3");
        break;
    }
//PLAYER HERE    

	strcat(soundfile, "&");
	if (soundLoop > 0) printf("%i. %s\n",soundLoop,soundfile);
	// system(soundfile);
    
    if (soundLoop > 0 ) soundLoop--;
	}
    printf("Soundausgabe end\n");
    return NULL;
}

// Servo
void servoInit(int pin){        		//initialization function for servo PMW pins
	pinMode(pin,OUTPUT);
    softPwmCreate(pin,  0, 200);
//	printf("Pin %i OK\n",pin);
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

// Sonar
int pulseIn(int pin, int level, int timeout) { //function pulseIn: obtain pulse time of a pin
   struct timeval tn, t0, t1;
   long micros;
   gettimeofday(&t0, NULL);
   micros = 0;
   while (digitalRead(pin) != level)
   {
      gettimeofday(&tn, NULL);
      if (tn.tv_sec > t0.tv_sec) micros = 1000000L; else micros = 0;
      micros += (tn.tv_usec - t0.tv_usec);
      if (micros > timeout) return 0;
   }
   gettimeofday(&t1, NULL);
   while (digitalRead(pin) == level)
   {
      gettimeofday(&tn, NULL);
      if (tn.tv_sec > t0.tv_sec) micros = 1000000L; else micros = 0;
      micros = micros + (tn.tv_usec - t0.tv_usec);
      if (micros > timeout) return 0;
   }
   if (tn.tv_sec > t1.tv_sec) micros = 1000000L; else micros = 0;
   micros = micros + (tn.tv_usec - t1.tv_usec);
   return micros;
}

float getSonar(){   // get the measurement results of ultrasonic module,with unit: cm
    long pingTime;
    float distance;
    digitalWrite(trigPin,HIGH); //trigPin send 10us high level 
    delayMicroseconds(10);
    digitalWrite(trigPin,LOW);
    pingTime = pulseIn(echoPin,HIGH,timeOut);   //read plus time of echoPin
    distance = (float)pingTime * 340.0 / 2.0 / 10000.0; // the sound speed is 340m/s,and calculate distance
	if (distance == 0) {
		distance = MAX_DISTANCE;
	}
    return distance;
}

float getSonarP(int angle) {
	float distance;
	servoWriteMS(servoPin_US,angle);
	delay(10);
	distance = getSonar();
	printf(" %3.2f cm an Position %i \n",distance,angle);
	return distance;
}

int freeDirection() {
	int i, free;
	float rSum=0, lSum=0;
	if (getSonarP((SERVO_MIN_US+SERVO_MAX_US)/2) >= MIN_DISTANCE) {
		free = 0;
		printf(" vorne %i \n--> Weg frei ",free);
	}else {
		motor(0); //Stopp während der messung
		for (i=SERVO_MIN_US;i<(SERVO_MIN_US+SERVO_MAX_US)/2;i++) {
			rSum=rSum+getSonarP(i);
		};
		printf(" Rechts : %6f ",rSum);
		for (i=SERVO_MAX_US;i>(SERVO_MIN_US+SERVO_MAX_US)/2;i--) {
		lSum=lSum+getSonarP(i);
		};
		
		printf(" Links : %6f\n",lSum);
		if (rSum>lSum) {
			free = 1;
			printf("--> Rechts frei \n");
		};
		if (rSum<lSum) {
			free = 2;
			printf("--> Link s frei \n");
		};
		if (lSum+rSum < (SERVO_MAX_US-SERVO_MIN_US)* MIN_DISTANCE) {
			free = 3;
			printf("--> Sackgasse   \n");
				blink(blinkrechtsPin,10,2);
				blink(blinklinksPin,10,2);
		};
	};
	printf("free %i\n",free);
	return free;
}

// Motor

void motor(int value){
	if(gear>0){
		digitalWrite(motorPin1,HIGH);
		digitalWrite(motorPin2,LOW);
		//printf("turn Forward...\n");
	}
	else if (gear<=0){
		digitalWrite(motorPin1,LOW);
		digitalWrite(motorPin2,HIGH);
		//printf("turn Back...\n");
		soundNr = 2;
		soundLoop = 1;
	}
	/*else {
	    digitalWrite(motorPin1,LOW); 
		digitalWrite(motorPin2,LOW);
		printf("Motor Stop...\n");
	*/
	
	if (gear == 0 ){
		softPwmWrite(enablePin,BRAKE);
		blink(rearlightPin,1,0);
	}else{
		softPwmWrite(enablePin,abs(value));
	}
}


// Joystick

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
					blink(blinkrechtsPin,2,2);
				}
				if (steering == -10 ) {
					blink(blinklinksPin,3,2);
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
	Up		move Turret1 up			Turret1 Stop
	Down	move Turret1 down		Turret1 Stop
	Left	move Turret1 left		Turret1 Stop
	Right	move Turret1 right		Turret1 Stop
	L1		Rückwärts               Vorwärts
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
					    blink(frontlightPin,60,0);
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
						run = 1;
					break;

				default :	//noch ohne Funktion
					printf("Button Nr.%i pressed\n", button);
					break;
			}
	}
//  RELEASE
    else {
	    switch (button) {
				case 0 :		//	X

                    break;
				case 1	:		//	O

					break;
				case 2 :	//Dreieck
    					turret1 = 9;
					break;
				case 4	:	//L1
                    gear = 1;
					break;
			//Turret1 Control
				case 13 :	//UP
					    turret1 =  0;
					break;
				case 14 :	//DOWN
					    turret1 =  0;
					break;
				case 15 :	//LEFT
					    turret1 =  0;
					break;
				case 16 :	//RIGHT
					    turret1 =  0;
					break;
				
				case 5 :    //R1
                    gear = 1;
                    speed = 0;
				    break;
				case 10 :	//PS

					break;

				default :	//noch ohne Funktion
					printf("Button Nr.%i released\n", button);
					break;
			}
	}
	return 0;
}

void *StickThread (void *value) {
		while ((read_event(js, &event) == 0)&&(run == 0)) {
        switch (event.type){
            case JS_EVENT_BUTTON:
                ButtonControl(event.number,event.value);
                break;
            case JS_EVENT_AXIS:
                StickControl(event.number,event.value);
                break;
            default:
                /* Ignore init events. */
                break;
        };
    }
	printf("StickThread end\n");
	run=2;
	return NULL;
}

void *TurretThread (void *value) {
    while (run==0) {
        switch (turret1) {
            case 0 :
            // dont move
            break;
            case 1 :
            // center
            turr1X	 = 0, turr1Y = -10;
            break;
            case 3 :
            // Laser on
            soundNr = 4;
            soundLoop = 1;
            digitalWrite(laserPin,HIGH);
            break;
            case 9 :
            // laser off
            digitalWrite(laserPin,LOW);
            break;
            case 8 :
            //up
            turr1Y++;
            delay(100);
            break;
            case 2 :
            //down
            turr1Y--;
            delay(100);
            break;
            case 4 :
            //left
            turr1X--;            
            delay(100);
            break;
            case 6 :
            //right
            turr1X++;
            delay(100);
            break;
            default :
            // dont move;
            break;
        }
    }
    printf("Turret off\n");
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
	pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);
	
// Output
	//OnOff
	pinMode(laserPin,OUTPUT);
	pinMode(blinkrechtsPin,OUTPUT);
    pinMode(blinklinksPin,OUTPUT);
    pinMode(frontlightPin,OUTPUT);  
    pinMode(rearlightPin,OUTPUT);
	
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

//Joystick init
	js = open(device, O_RDONLY);
	while (js == -1) {
	    js = open(device, O_RDONLY);
        blink(frontlightPin,2,20);
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
}

int SubmitMotor( int steeringInput, int speedInput) {
/* WARTUNG
//Motor
	while (Speed_current > speedInput) {    // Rückwärts
	    Speed_current--;
		motor(Speed_current);
		delay(10);
	}
	while (Speed_current < speedInput) {    // Vorwärts
	    Speed_current++;
		motor(Speed_current);
		delay(10);
	}
	if (Speed_current != speedInput) {
	    printf("\nSomething went wrong with the motor Speed_current %i != speedInput %i\n",Speed_current, speedInput);
	}
WARTUNG*/
motor(speedInput);
	
//Lenkung	
	servoWriteMS(servoPin_ST,map(steeringInput,10,-10,SERVO_MIN_ST,SERVO_MAX_ST));
	return 0;
}

int SubmitTurr1( int x, int y) {
		servoWriteMS(servoPin_CX,map(x,-10,10,SERVO_MIN_CX,SERVO_MAX_CX));
		servoWriteMS(servoPin_CY,map(y,-10,10,SERVO_MIN_CY,SERVO_MAX_CY));
		return 0;
}

int main (int argc, char *argv[]) {
	if (argc > 1) {
		device = argv[1];
	} else {
		device = "/dev/input/js0";
	};

//Setup	
	Setup();
	
//Main-Loop Section
		while (run != 2) {
			if (steering > 10) 	steering = 10;
			if (steering <-10) 	steering =-10;
			if (speed > SPEED_MAX) 	speed = SPEED_MAX;
			if (speed <-SPEED_MAX) 	speed =-SPEED_MAX;
			SubmitMotor ( steering, speed);
		//	printf("Steuer: %i Speed %i", steering,speed);
			if (turr1X > 10) 	turr1X = 10;
			if (turr1X <-10) 	turr1X =-10;
			if (turr1Y > 10) 	turr1Y = 10;
			if (turr1Y <-10) 	turr1Y =-10;	
			SubmitTurr1 ( turr1X, turr1Y);
		//	printf(" Turm X: %i Turm Y %i \n",turr1X,turr1Y);
		}
		
//End Section

    close(js);
	motor(0);
return 0;
}
