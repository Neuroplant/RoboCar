LDFLAGS = -lwiringPi -lm -lpthread -lwiringPiPca9685 -lwiringPiDev
Remote: RemoteCar.c
	gcc RemoteCar.c -o Remote $(LDFLAGS)
Line:	LineFollowCar.c
	gcc LineFollowCar.c -o Line $(LDFLAGS)
