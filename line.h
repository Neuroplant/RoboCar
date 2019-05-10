/* Line detector
int init_LineDetect(void)
int LineDetect(void)
*/

#include <wiringPi.h>

int init_LineDetect(void) {
	pinMode(LinePinL, INPUT);
	pinMode(LinePinR, INPUT);
	return 0;
}

int LineDetect (void) {
	int val;
		if ((digitalRead(LinePinL)==LOW) && (digitalRead(LinePinR)==LOW)) val = 0;
		if ((digitalRead(LinePinL)==LOW) && (digitalRead(LinePinR)==HIGH)) val = 1;
		if ((digitalRead(LinePinL)==HIGH) && (digitalRead(LinePinR)==LOW)) val = 2;
		if ((digitalRead(LinePinL)==HIGH) && (digitalRead(LinePinR)==HIGH)) val = 3;
	return val;
}
