//Ultrasonic Obstacle avoidance 
#include <sys/time.h>
struct timespec Time1; 
long StartTime, EndTime; 

long PulseLen (int inpin) { 
	while (digitalRead(inpin) == LOW) { 
		clock_gettime(CLOCK_REALTIME, &Time1); 
		StartTime  = Time1.tv_nsec; 
		if (EndTime-StartTime >= 13200) {return 13201; break}
	} 
	while (digitalRead(inpin) == HIGH) { 
		clock_gettime(CLOCK_REALTIME, &Time1); 
		EndTime = Time1.tv_nsec; 
		if (EndTime-StartTime >= 13200) {return 13200; break}
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