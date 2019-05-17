#ifndef PS3_CONTROL_H
#define PS3_CONTROL_H

//	JoyStick  ///////////////////////////////////////////////////////////////////////
#include <linux/joystick.h>

const char *device = "/dev/input/js0";
int js;
struct js_event event;
size_t axis;

int read_event(int fd, struct js_event *event) {
    ssize_t bytes;
    bytes = read(fd, event, sizeof(*event));
    if (bytes == sizeof(*event)) return 0;
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

int AnalogControl(int axis, int value) {
	switch (axis) {
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
					Blinker[3].dura = 2;
					Blinker[3].freq = 2;
				}
				if (steering == -10 ) {
					Blinker[2].dura = 2;
					Blinker[2].freq = 2;
					Blinker[4].dura = 2;
					Blinker[4].freq = 2;					
				}
		break;
		case 4 :	//R3 Up/Down
		break;
		case 5 :	//R2 Pull
			if (encoder_mode) {
			    Spin_Target = (map(value,-32767,32767,0,(int)SPIN_MAX));
			}else{
			    throttle = (map(value, -32767,32767,0,(int)THROTTLE_MAX));
			}

		break;
	}
	return 0;
}

int DigitalControl (int button, int value) {
//  PRESS							RELEASE
/*
	X		Hupe					-
	O    	Scheinwerfer (60s)		-
	Dreieck	Laser Beam on			Laser Beam off			
	Quadrat	encoder_mode on/off
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
	if (value==1) {
		switch (button) {
			case 0 :		//	X
				Sound[1].loop = 2;
			break;
			case 1	:		//	O
					//Scheinwerfer
				Blinker[5].dura = 60;
				Blinker[5].freq = 0;
			break;
			case 2 :		//Dreieck
				turret1 = 3;
			break;
			case 3 :		//quadrat
				encoder_mode_Switch(-1);
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
				run = false;
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
				DigitalControl(event.number,event.value);
			break;
			case JS_EVENT_AXIS:
				AnalogControl(event.number,event.value);
			break;
		}
	}
	printf("StickThread end\n");
	return NULL;
}

int init_Joystick(void) {
	js = open(device, O_RDONLY);
	while (js == -1) {
	    	js = open(device, O_RDONLY);
        	Blinker[5].dura = 2;
		Blinker[5].freq = 20;
		delay(2000);
		printf("Warte auf Joystick\n");
	};
	printf("Joystick ready \n");

	pthread_t t_Joystick;
		if(pthread_create(&t_Joystick, NULL, StickThread, NULL)) {
			printf("Error creating thread t_Joystick\n");
			return 1;
		}	
}

#endif
