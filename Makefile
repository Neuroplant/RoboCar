LDFLAGS = -lwiringPi -lm -lpthread -lwiringPiPca9685 -lwiringPiDev
Remote: RemoteCar.c
	gcc RemoteCar.c -o Remote $(LDFLAGS)
Line:	LineFollowCar.c
	gcc LineFollowCar.c -o Line $(LDFLAGS)
PPM:	PPM_Test_beta.c
	gcc PPM_Test_beta.c -o Test $(LDFLAGS)
RCPWM:	RC_PWM_Test.c
	gcc RC_PWM_Test.c -o Test $(LDFLAGS)
Encoder:	Encoder_Test.c
	gcc Encoder_Test.c -o Test $(LDFLAGS)
Blinker:	blinker_Test.c
	gcc blinker_Test.c -o Test $(LDFLAGS)
Servo:	servo_Test.c
	gcc servo_Test.c -o Test $(LDFLAGS)
Sonar:	sonar_Test.c
	gcc sonar_Test.c -o Test $(LDFLAGS)