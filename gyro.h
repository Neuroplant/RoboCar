// Gyroskop
// V0.1 04.06.2019 Neuroplant
/*
int init_Gyro(int Addr,int *gyrohd)	initalizise MPU6050
int set_gyro_x(int *gyrohd)	//Set current position/movement as 0
int set_gyro_y(int *gyrohd)	//Set current position/movement as 0
int set_gyro_z(int *gyrohd)	//Set current position/movement as 0

int gyroskop_xout(int *gyrohd)	// unit °/s
int gyroskop_yout(int *gyrohd)
int gyroskop_zout(int *gyrohd)

int accel_xout(int *gyrohd)	// unit g
int accel_yout(int *gyrohd)
int accel_zout(int *gyrohd)

int get_x_rotation(int *gyrohd)
int get_y_rotation(int *gyrohd)
int get_z_rotation(int *gyrohd)

*/
#include <math.h>
#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#define FS_SEL	3
#define GyroSens 131072/(1000*(FS_SEL+1))
#define GyroMax 250*(FS_SEL+1)
/*
FS_SEL 		FullScale Range	LSB Sensitivity
0		± 250 °/s	131 LSB/°/s
1		± 500 °/s	65.5 LSB/°/s
2		± 1000 °/s	32.8 LSB/°/s
3		± 2000 °/s	16.4 LSB/°/s	<-
*/

#define AFS_SEL	3
#define AccelSens 16384/(AFS_SEL+1);
#define AccelMax pow(2,AFS_SEL+1)
/*
AFS_SEL 	FullScale Range	LSB Sensitivity
0		±2g		16384 LSB/g
1		±4g		8192 LSB/g
2		±8g		4096 LSB/g
3		±16g		2048 LSB/g	<-
*/

#define DLPF_CFG	2
/*
DLPF_CFG 	Accelerometer 			Gyroscope 
	Bandwidth(Hz) 	Delay(ms)	Fs (kH)	Bandwidth(Hz)	Delay(ms)	Fs(kHz)
0	260		0		1	256		0.98		8
1	184		2.0		1	188		1.9		1
2	94		3.0		1	98		2.8		1	<-
3	44		4.9		1	42		4.8		1
4	21		8.5		1	20		8.3		1
5	10		13.8		1	10		13.4		1
6	5		19.0		1	5		18.6		1
7		RESERVED				RESERVED
*/

#define CLKSEL	2
/*
CLKSEL 	Clock Source
0	Internal 8MHz oscillator
1	PLL with X axis gyroscope reference
2	PLL with Y axis gyroscope reference						<-
3	PLL with Z axis gyroscope reference
4	PLL with external 32.768kHz reference
5	PLL with external 19.2MHz reference
6	Reserved
7	Stops the clock and keeps the timing generator in reset
*/

int AX_err = 0;
int AY_err = 0;
int AZ_err = 0;
int GX_err = 0;
int GY_err = 0;
int GZ_err = 0;


int readWord(int handle,int Reg) {
	int HighByte = wiringPiI2CReadReg8(handle,Reg);
	int LowByte = wiringPiI2CReadReg8(handle,Reg+1);
	return	((HighByte*256)+LowByte);
}
 
int readWord_2c(int handle, int Reg, int err) {
	int val = (readWord(handle,Reg))-err;
	if (val >= 32768) {
		return -(65535 - val) ;
	}else {
		return val;
	}
}

int dist(int a,int b) {
    return sqrt((a*a)+(b*b));
}
 

int init_Gyro(int Addr,int *gyrohd);


float temperatur_out(int gyrohd) {
	return (((readWord_2c(gyrohd,0x41,0))/340)+36.53);
}
 
float gyroskop_xout(int gyrohd) {
	return (readWord_2c(gyrohd,0x43,GX_err)/GyroSens);
}
float gyroskop_yout(int gyrohd) {
	return (readWord_2c(gyrohd,0x45,GY_err)/GyroSens);
}
float gyroskop_zout(int gyrohd) {
	return (readWord_2c(gyrohd,0x47,GZ_err)/GyroSens);
}
 
float accel_xout(int gyrohd) {
	return (readWord_2c(gyrohd,0x3b,AX_err)/AccelSens);
}
float accel_yout(int gyrohd) {
	return (readWord_2c(gyrohd,0x3d,AY_err)/AccelSens);
}
float accel_zout(int gyrohd) {
	return (readWord_2c(gyrohd,0x3f,AZ_err)/AccelSens);
}
 
int get_y_rotation(int gyrohd) {
	int x,y,z, radians;
	 x = gyroskop_xout(gyrohd)/16384.0;
	 y = gyroskop_yout(gyrohd)/16384.0;
	 z = gyroskop_zout(gyrohd)/16384.0;
     radians = atan2(x, dist(y,z));
    return -(radians*180/3.1415926535);
}
 
int get_x_rotation(int gyrohd) {
	int x,y,z, radians;
	 x = gyroskop_xout(gyrohd)/16384.0;
	 y = gyroskop_yout(gyrohd)/16384.0;
	 z = gyroskop_zout(gyrohd)/16384.0;
     radians = atan2(y, dist(x,z));
    return (radians*180/3.1415926535);
}
 
int get_z_rotation(int gyrohd) {
	int x,y,z, radians;
	 x = gyroskop_xout(gyrohd)/16384.0;
	 y = gyroskop_yout(gyrohd)/16384.0;
	 z = gyroskop_zout(gyrohd)/16384.0;
     radians = atan2(z, dist(x,y));
    return (radians*180/3.1415926535);
}

int set_gyro_y(int gyrohd) {
	for (int i = 0; i< 100; i++) {
		GY_err +=readWord_2c(gyrohd,0x45,0);
	}
	GY_err = (int) GY_err/100;
	return GY_err;
}
int set_gyro_x(int gyrohd) {
	for (int i = 0; i< 100; i++) {
		GX_err += readWord_2c(gyrohd,0x43,0);
	}
	GX_err = (int) GX_err/100;
	return GX_err;
}
int set_gyro_z(int gyrohd) {
	for (int i = 0; i< 100; i++) {
		GZ_err += readWord_2c(gyrohd,0x47,0);
	}
	GZ_err = (int) GZ_err/100;
	return GZ_err;
}
int set_accel_z(int gyrohd) {
	for (int i = 0; i< 100; i++) {
		AZ_err += readWord_2c(gyrohd,0x3f,0);
	}
	AZ_err = (int) AZ_err/100;
	return AZ_err;
}
int set_accel_y(int gyrohd) {
	for (int i = 0; i< 100; i++) {
		AY_err += readWord_2c(gyrohd,0x3d,0);
	}
	AY_err = (int) AY_err/100;
	return AY_err;
}
int set_accel_x(int gyrohd) {
	for (int i = 0; i< 100; i++) {
		AX_err += readWord_2c(gyrohd,0x3b,0);
	}
	AX_err = (int) AX_err/100;
	return AX_err;
}
int set_accel(int gyrohd) {
	set_accel_x(gyrohd);
	set_accel_y(gyrohd);
	set_accel_z(gyrohd);
	return 1;
}
int set_gyro(int gyrohd) {
	set_gyro_x(gyrohd);
	set_gyro_y(gyrohd);
	set_gyro_z(gyrohd);
	return 1;
}
int init_Gyro(int Addr,int *gyrohd) {
	*gyrohd=(int)wiringPiI2CSetup(Addr);
	if ((int)gyrohd == -1) return (int)gyrohd;
	wiringPiI2CWriteReg8 (*gyrohd, 107, CLKSEL);	// clock source:	PLL with Y axis gyroscope reference
	wiringPiI2CWriteReg8 (*gyrohd, 28, AFS_SEL); // full scale range of the accelerometer:	+- 16g
	wiringPiI2CWriteReg8 (*gyrohd, 27, FS_SEL); // full scale range of the gyroscope:		+- 2000°/s
	wiringPiI2CWriteReg8 (*gyrohd, 26, DLPF_CFG);  // LowPass filter:	 Accel: 94Hz 3.0ms 	Gyro: 98Hz 2.8ms 1kHz
1	return (int)gyrohd;
}