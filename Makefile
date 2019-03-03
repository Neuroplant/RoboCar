LDFLAGS = -lwiringPi -lm -lpthread -lwiringPiPca9685
Remote: RemoteCar.c
	gcc RemoteCar.c -o Remote $(LDFLAGS)
	
