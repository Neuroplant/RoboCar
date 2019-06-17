#common functions
#ifndef COMMON_H
#define COMMON_H

long map(long value,long fromLow,long fromHigh,long toLow,long toHigh){
    return (toHigh-toLow)*(value-fromLow) / (fromHigh-fromLow) + toLow;}

int pulsln(int pin,bool level, int timeout) {
  unsigned int count = 0;

  while (digitalRead(pin) == level)
  {
    if (count++ == timeout)
      return 0 ;
  }

  while (digitalRead(pin) != level)
  {
    if (count++ == timeout)
      return 0 ;
  }

  int timerStart = micros();
  while (digitalRead(pin) == level)
  {
    if (count++ == timeout)
      return 0 ;
  }

  return micros() - timerStart ;
}
#endif