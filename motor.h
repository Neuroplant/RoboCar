void *MotorThread(void *value){
	pinMode(motorPin1,OUTPUT);
	pinMode(motorPin2,OUTPUT);

	printf("Motor ready\n");
	while (run) {
				if (steering > 10) steering = 10;
		if (steering <-10) steering =-10;
		
		if (Spin_Current() > Spin_Target)	throttle=throttle-ACCELERATION;
		if (Spin_Current() < Spin_Target) 	throttle=throttle+ACCELERATION;

		if (throttle > THROTTLE_MAX) 		throttle = THROTTLE_MAX;
		if (throttle < 0) 					gear=-1;
		if (throttle < -THROTTLE_MAX) 		throttle = -THROTTLE_MAX;
		
		
		if(gear>0){
			pwmWrite(motorPin1,0);
			pwmWrite(motorPin2,abs(throttle));
		//printf("turn Forward...\n");
		}
		if (gear<0){
			pwmWrite(motorPin1,abs(throttle));
			pwmWrite(motorPin2,0);
		//printf("turn Back...\n");
			Sound[2].loop = 1;
		}
		if (gear == 0 ){
			pwmWrite(motorPin1,0);
			pwmWrite(motorPin2,0);
			Blinker[6].dura = 1;
			Blinker[6].freq = 0;
		}
		servoWriteMS(servoPin_ST,map(steering,10,-10,SERVO_MIN_ST,SERVO_MAX_ST));
	}
	pwmWrite(motorPin1,0);
	pwmWrite(motorPin2,0);
	printf("Motor off\n");	
}
