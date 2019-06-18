# RoboCar
JJRC Q60 mit Raspberrysteuerung
Als Zwischenschritt erstmal "RemoteCar" in dem die Steuerung nicht anhand von Sensoren etc erfolgt, sondern mit einem PS3-Controller.

# RemoteCar/LineFollowCar
Ich habe mir mal die Mühe gemacht und etwas aufgeräumt.
Jede einzelne Funktion/HW-Komponente findet sich jetzt in einer eigenen *.H Datei und es gibt (fast) zu jeder auch eine *_Test.c die einerseits die Natürlich die Hardware testet, als auch helfen soll die Verwendung der *.h Datei zu verstehen.

# Install


	Prerequisites:
	WiringPi Libary 	www.wiringpi.com
	pca9685 Libary 	https://github.com/Reinbert/pca9685
	
	make remote
  	sudo ./RemoteCar 

# gyro.h
Libary for using an MPU6050 Gyroskop

	int init_Gyro(int Addr,int *gyrohd)
	int gyroskop_xout(int *gyrohd)
	int gyroskop_yout(int *gyrohd)
	int gyroskop_zout(int *gyrohd)
	int beschleunigung_xout(int *gyrohd)
	int beschleunigung_yout(int *gyrohd)
	int beschleunigung_zout(int *gyrohd)
	int get_x_rotation(int *gyrohd)
	int get_y_rotation(int *gyrohd)

# line.h
Libary for using Linedetector

	int init_LineDetect(void)
	int LineDetect(void)
	
	needs:
	#define	linePinR [pin of right detector]
	#define	linePinL [pin of left detector]
	

	
# Encoder
Im Encoder-Modus gibt die Stellung des Stick die Drehzahl vor und das Programm versucht diese zu erreichen, indem es die Spannung des Motors nachregelt.
Hierduch sollen auch langsame Fahrten und ein Tempomat-Effect ermöglicht werden.

# Tastenbelegung

	PRESS							RELEASE
	X			Hupe					-
	O    		Scheinwerfer (60s)		-
	Dreieck		Laser Beam on		Laser Beam off			
	Quadrat		encoder_mode on/off
	Up		move Turret1 up		Turret1 Stop
	Down		move Turret1 down	Turret1 Stop
	Left		move Turret1 left	Turret1 Stop
	Right		move Turret1 right	Turret1 Stop
	L1		Rückwärts		Vorwärts
	L2			-		
	L3			-
	R1			Brake               Brake off
	R2			-
	R3			-
	Select		-
	Option		-
	PS			Quit

