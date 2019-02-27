LDFLAGS = -lwiringPi -lm -lpthread
Remote: RemoteCar.c
	gcc RemoteCar.c -o Remote $(LDFLAGS)
	
