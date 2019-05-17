#ifndef SOUND_H
#define SOUND_H

#include <stdlib.h>

struct s_Sound {
	int loop;
	char name[20];	
};
struct s_Sound Sound[5];
pthread_t t_Sound[5];
bool SoundLock = false;

void *SoundThread(void *arg) {
	long idNr = (long)arg;
	int i;
	char soundfile[100];
	strcpy (soundfile, "omxplayer --no-keys -o local /home/pi/RoboCar/Sounds/");
	strcat(soundfile,Sound[idNr].name);
	printf("Sound %i: %s ready\n",idNr,Sound[idNr].name);
	while (run) {
		if (!SoundLock  && Sound[idNr].loop>0) {
			SoundLock=true;
			while (Sound[idNr].loop>0) {
				system(soundfile);
				Sound[idNr].loop--;
			}
			SoundLock=false;
			
		}
		if (SoundLock) Sound[idNr].loop=0;
	}
	printf("Sound %i end\n",i,Sound[idNr].name);
	return NULL;
}
int init_Sound (void) {
	int i=0;
	strcpy(Sound[0].name, "0-idle.mp3"); 
	strcpy(Sound[1].name, "1-Horn.mp3"); 
	strcpy(Sound[2].name, "2-BackupBeep.mp3"); 
	strcpy(Sound[3].name, "3-EngineStart.mp3"); 
	strcpy(Sound[4].name, "4-Laser.mp3"); 

	
	
	for (i=0;i<5;i++) {
		if(pthread_create(&t_Sound[i], NULL, SoundThread, (void*)i)) {
	   	printf("Error creating thread t_Sound %i\n",i);

	   	return 1;
		}
	}
	return 0;
}

#endif
