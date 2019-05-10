// Gyroskop
/*
int init_Gyro(int Addr,int *gyrohd)
int gyroskop_xout(int *gyrohd)
int gyroskop_yout(int *gyrohd)
int gyroskop_zout(int *gyrohd)
int beschleunigung_xout(int *gyrohd)
int beschleunigung_yout(int *gyrohd)
int beschleunigung_zout(int *gyrohd)
int get_x_rotation(int *gyrohd)
int get_y_rotation(int *gyrohd)
*/
#include <math.h>
#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>


int readWord(int handle,int Register) {
	int HighByte = wiringPiI2CReadReg8(handle,Reg);
	int LowByte = wiringPiI2CReadReg8(handle,Reg+1);
	return	((dataH*255)+dataL);
}
 
int readWord_2c(int handle, int Reg) {
	int val = readWord(handle,Reg)
    if (val >= 32768) {
		return -((65535 - val) + 1);
	}else {
		return val;
	}
}

int dist(int a,int b) {
    return sqrt((a*a)+(b*b));
}
 
int get_y_rotation(int *gyrohd) {
	int x = gyroskop_xout(int *gyrohd)/16384.0;
	int y = gyroskop_yout(int *gyrohd)/16384.0;
	int z = gyroskop_zout(int *gyrohd)/16384.0;
    int radians = atan2(x, dist(y,z));
    return -(radians*180/3.1415926535);
}
 
int get_x_rotation(int *gyrohd) {
	int x = gyroskop_xout(int *gyrohd)/16384.0;
	int y = gyroskop_yout(int *gyrohd)/16384.0;
	int z = gyroskop_zout(int *gyrohd)/16384.0;
    int radians = atan2(y, dist(x,z));
    return (radians*180/3.1415926535);
}
 
int init_Gyro(int Addr,int *gyrohd) {
	gyrohd=wiringPiI2CSetup(Addr);
	if (gyrohd == -1) return gyrohd;
	wiringPiI2CWriteReg8 (gyrohd, 107, 0);
	wiringPiI2CWriteReg8 (gyrohd, 28, 0);
	wiringPiI2CWriteReg8 (gyrohd, 27, 0);
	return gyrohd
}
 
int gyroskop_xout(int *gyrohd) {
	return readWord_2c(gyrohd,0x43);
}
int gyroskop_yout(int *gyrohd) {
	return readWord_2c(gyrohd,0x45);
}
int gyroskop_zout(int *gyrohd) {
	return readWord_2c(gyrohd,0x47);
}
 
int beschleunigung_xout(int *gyrohd) {
	return readWord_2c(gyrohd,0x3b);
}
int beschleunigung_yout(int *gyrohd) {
	return readWord_2c(gyrohd,0x3d);
}
int beschleunigung_zout(int *gyrohd) {
	return readWord_2c(0x3f);
}
 
