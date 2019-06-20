//common functions
#ifndef COMMON_H
#define COMMON_H
#include <wiringPi.h>
long map(long value,long fromLow,long fromHigh,long toLow,long toHigh){
    return (toHigh-toLow)*(value-fromLow) / (fromHigh-fromLow) + toLow;
}
int	ByteToWord(char ByteHigh,char ByteLow) {
	return ((ByteHigh*0xFF)+ByteLow);
}
int readWord(int handle,int Reg) {
    return  ByteToWord(wiringPiI2CReadReg8(handle,Reg),wiringPiI2CReadReg8(handle,Reg+1));
}	

int pulsln(int pin,bool level, int timeout) {
  int killtime = micros();
  while (digitalRead(pin) == level)
  {
    if (micros()-killtime >= timeout)
      return 0 ;
  }
  while (digitalRead(pin) != level)
  {
    if (micros()-killtime >= timeout)
      return 0 ;
  }
  int timerStart = micros();
 while (digitalRead(pin) == level)
  {
    if (micros()-killtime >= timeout)
      return 0 ;
  }
  return micros() - timerStart ;
}
#endif
