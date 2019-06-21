LDFLAGS = -lwiringPi -lm -lpthread -lwiringPiPca9685 -lwiringPiDev
Remote: RemoteCar.c
	gcc RemoteCar.c -o Remote $(LDFLAGS)
Line:	LineFollowCar.c
	gcc LineFollowCar.c -o Line $(LDFLAGS)
RCPPM:	RC_PPM_Control_Test.c
	gcc RC_PPM_Control_Test.c -o Test $(LDFLAGS)
RCPWM:	RC_PWM_Control_Test.c
	gcc RC_PWM_Control_Test.c -o Test $(LDFLAGS)
RCIBUS:	RC_iBUS_Control_Test.c
	gcc RC_iBUS_Control_Test.c -o Test $(LDFLAGS)
Encoder:	Encoder_Test.c
	gcc Encoder_Test.c -o Test $(LDFLAGS)
Blinker:	blinker_Test.c
	gcc blinker_Test.c -o Test $(LDFLAGS)
Servo:	servo_Test.c
	gcc servo_Test.c -o Test $(LDFLAGS)
Sonar:	sonar_Test.c
	gcc sonar_Test.c -o Test $(LDFLAGS)
Gyro:	gyro_Test.c
	gcc gyro_Test.c -o Test $(LDFLAGS)
Gimbal:	Gimbal_Test.c
	gcc Gimbal_Test.c -o Test $(LDFLAGS)
Engine:	engine_Test.c
	gcc engine_Test.c -o Test $(LDFLAGS)
