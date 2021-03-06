#ifndef CONSTANTS_H
#define CONSTANTS_H

#define DEV_ID0		0x40
#define PIN_BASE0 	64
#define PWM_MAX 	4095
#define HERTZ 		50

// Pin definitions
#define servoPin_ST	15 + PIN_BASE0  //Lenkung (Steering)    out PWM
#define motorPin2	14 + PIN_BASE0	//Motor Rückwärts       out PWM
#define motorPin1	13 + PIN_BASE0	//Motor Vorwärts        out PWM
//#define enablePin	12 + PIN_BASE0  //Motor Geschwindigkeit out PWM

#define servoPin_GX	11 + PIN_BASE0  //Gimbal X              out PWM
#define	servoPin_GY	10 + PIN_BASE0  //Gimbal Y              out PWM
#define servoPin_GZ  9 + PIN_BASE0  //Gimbal Z							out PWM	
#define	servoPin_US	8 + PIN_BASE0  //Ultraschall-Servo     out PWM

//#define	xxx	7 + PIN_BASE0      
#define laserPin	6 + PIN_BASE0  	      //div LEDs              out digital
#define blinklinksvPin  5 + PIN_BASE0 	//div LEDs              out digital
#define blinkrechtsvPin 4 + PIN_BASE0  	//div LEDs              out digital

#define frontlightPin   3 + PIN_BASE0  	//div LEDs              out digital
#define rearlightPin    2 + PIN_BASE0  	//div LEDs              out digital
#define blinkrechtsPin  1 + PIN_BASE0  	//div LEDs              out digital
#define blinklinksPin   0 + PIN_BASE0  	//div LEDs              out digital

#define DEV_ID0_enable	7
#define phaseAPin	13 		//Encoder Phase A	in digital
#define phaseBPin	12 		//Encoder Phase B	in digital

#define DEV_ID1		0x27
#define PIN_BASE1 	128
#define RS PIN_BASE1+0 
#define RW PIN_BASE1+1 
#define EN PIN_BASE1+2 
#define LED PIN_BASE1+3 
#define D4 PIN_BASE1+4 
#define D5 PIN_BASE1+5 
#define D6 PIN_BASE1+6 
#define D7 PIN_BASE1+7 

// 	AB-Phase-Encoder 
#define Teeth		11	//number of teeth on the encoder wheel
#define SPIN_MAX	100 // max 260/min


#define	SERVO_MIN_MS	5
#define SERVO_MAX_MS	25

#define	SERVO_MIN_ST	10
#define SERVO_MAX_ST	18

#define	SERVO_MIN_CX	5
#define SERVO_MAX_CX	26
#define	SERVO_MIN_CY	5
#define SERVO_MAX_CY	15

#define THROTTLE_MAX    PWM_MAX         //defines the scale and also the acceleration
int ACCELERATION = THROTTLE_MAX/20;		//acceleration per cycle 
#define BRAKE           30          	//Bremskraft


//Ultraschall
#define trigPin         4   	//Ultraschall-Trigger   out digital
#define echoPin         5   	//Ultraschall-Echo      in  digital


#define	SERVO_MIN_US	5
#define SERVO_MAX_US	25

//Linefollower
#define LinePinR 2
#define LinePinL 3

//RC Contol
#define Anz_RC_Channels	10
const int RCPin[Anz_RC_Channels] = {30, 21, 22, 23, 31, 25, 26, 27, 28, 29};

#define PPMPin	          29    // PPM input Pin
#define Anz_RC_Channels  10     // Number of PPM channels (8)
#define RCmax             2000
#define RCmin             1000
#define RCmid          (RCmax+RCmin)/2

//Gyro/Gimbal
#define DEVGY1  0x68 // 0x69 for AdressPin high

#define OFFSETGX_MS 0  
#define OFFSETGY_MS 0   
#define OFFSETGZ_MS 0    

#define SERVOGX_MIN_MS 5+OFFSETGX_MS        
#define SERVOGX_MAX_MS 25+OFFSETGX_MS       
#define SERVOGY_MIN_MS 5+OFFSETGY_MS        
#define SERVOGY_MAX_MS 25+OFFSETGY_MS       
#define SERVOGZ_MIN_MS 5+OFFSETGZ_MS        
#define SERVOGZ_MAX_MS 25+OFFSETGZ_MS  

#endif /* CONSTANTS_H */
