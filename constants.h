

#define DEV_ID0		0x40
#define PIN_BASE0 	64
#define PWM_MAX 	4095
#define HERTZ 		50

// Pin definitions
#define servoPin_ST	15 + PIN_BASE0  //Lenkung (Steering)    out PWM
#define motorPin2	14 + PIN_BASE0	//Motor Rückwärts       out PWM
#define motorPin1	13 + PIN_BASE0	//Motor Vorwärts        out PWM
//#define enablePin	12 + PIN_BASE0  //Motor Geschwindigkeit out PWM

#define servoPin_CX	11 + PIN_BASE0  //Camera X              out PWM
#define	servoPin_CY	10 + PIN_BASE0  //Camera Y              out PWM
#define servoPin_US  9 + PIN_BASE0   	//Ultraschall-Servo     out PWM

//#define	xxx	8 + PIN_BASE0 

//#define	xxx	7 + PIN_BASE0 
#define laserPin	6 + PIN_BASE0  	//div LEDs              out digital
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


#define ACCELERATION	32		//acceleration per cycle 

#define	SERVO_MIN_MS	4
#define SERVO_MAX_MS	26

#define	SERVO_MIN_ST	10
#define SERVO_MAX_ST	18

#define	SERVO_MIN_CX	5
#define SERVO_MAX_CX	26
#define	SERVO_MIN_CY	5
#define SERVO_MAX_CY	15

#define THROTTLE_MAX    PWM_MAX         //defines the scale and also the acceleration
#define BRAKE           30          	//Bremskraft
#define SPIN_MAX	4920 		//max 6100


//Ultraschall
#define trigPin         4   	//Ultraschall-Trigger   out digital
#define echoPin         5   	//Ultraschall-Echo      in  digital

#define	SERVO_MIN_US	5
#define SERVO_MAX_US	27

//Linefollower
#define linePinR 2
#define linePinL 3