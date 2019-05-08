//	JoyStick  ///////////////////////////////////////////////////////////////////////
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