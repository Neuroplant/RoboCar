#ifndef SONAR_H
#define SONAR_H

//Ultrasonic Obstacle avoidance 
#include <sys/time.h>
struct timespec Time0, Time1; 
long StartTime, EndTime, OutTime; 


int pulseIn(int pin, int level, int timeout) {
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

long PulseLen (int inpin, int level, int timeout) { 
	long val;
	clock_gettime(CLOCK_REALTIME, &Time0);
	OutTime = Time0.tv_nsec;
	while (digitalRead(inpin) != level) { 
		clock_gettime(CLOCK_REALTIME, &Time1); 
		StartTime  = Time1.tv_nsec; 
		if (StartTime-OutTime >= timeout) {val = 13201; break;}
	} 
	clock_gettime(CLOCK_REALTIME, &Time0);
	OutTime = Time0.tv_nsec;
	while (digitalRead(inpin) == level) { 
		clock_gettime(CLOCK_REALTIME, &Time1); 
		EndTime = Time1.tv_nsec; 
		if (EndTime-OutTime >= timeout) {val = 13200; break;}
	} 
	return (EndTime-StartTime); 
}
 
float getSonar() { 
	digitalWrite(trigPin,HIGH); 
	delayMicroseconds(10); 
	digitalWrite(trigPin,LOW); 
	long pingTime = pulseIn(echoPin,HIGH,13200);   //read plus time of echoPin
	//pingTime = PulseLen(echoPin,HIGH,13200); //have to take a look, why my function does not work
	float distance = (float)pingTime * 340.0 / 2.0 / 10000.0; // the sound speed is 340m/s,and calculate distance
	return distance;
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
#endif
