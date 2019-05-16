//Ultrasonic Obstacle avoidance 
#include <sys/time.h>
struct timespec Time0, Time1; 
long StartTime, EndTime, OutTime; 

long PulseLen (int inpin) { 
	long val;
	clock_gettime(CLOCK_REALTIME, &Time0);
	OutTime = Time0.tv_nsec;
	while (digitalRead(inpin) == LOW) { 
		clock_gettime(CLOCK_REALTIME, &Time1); 
		StartTime  = Time1.tv_nsec; 
		if (StartTime-OutTime >= 13200) {val = 13201; break;}
	} 
	clock_gettime(CLOCK_REALTIME, &Time0);
	OutTime = Time0.tv_nsec;
	while (digitalRead(inpin) == HIGH) { 
		clock_gettime(CLOCK_REALTIME, &Time1); 
		EndTime = Time1.tv_nsec; 
		if (EndTime-OutTime >= 13200) {val = 13200; break;}
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

int init_Sonar(void) {
	pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);
	pinMode(servoPin_US,OUTPUT);   
	return 0;
}
