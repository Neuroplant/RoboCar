# RoboCar
JJRC Q60 mit Raspberrysteuerung
Als Zwischenschritt erstmal "RemoteCar" in dem die Steuerung nicht anhand von Sensoren etc erfolgt, sondern mit einem PS3-Controller.

# RemoteCar
Alle Ausgänge der Steuerung erfolgen über ein 16 Channel PWM Board via I²C.
Nur die beiden Eingänge für den Drehzahl-Encoder sind direkt mit dem RPI verbunden.
Dadurch soll ein bischen Ordnung geschaffen werden und die Hardware flexibel auch mit einem Arduino gesteuert werden können.

# Install


	Prerequisites:
	WiringPi Libary 	www.wiringpi.com
	pca9685 Libary 	https://github.com/Reinbert/pca9685
	
	make remote
  	
	sudo ./RemoteCar [encoder]

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

