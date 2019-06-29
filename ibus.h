#ifndef IBUS_H
#define IBUS_H
int open_serial(void){
	
  /*
   * Oeffnet seriellen Port
   * Gibt das Filehandle zurueck oder -1 bei Fehler
   *
   * RS232-Parameter:
   * 19200 bps, 8 Datenbits, 1 Stoppbit, no parity, no handshake
   */

	int ibusfd;                    /* Filedeskriptor */
	struct termios options;    /* Schnittstellenoptionen */

   /* Port oeffnen - read/write, kein "controlling tty", Status von DCD ignorieren */
	ibusfd = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);
	if (ibusfd >= 0){
	   /* get the current options */
	   fcntl(ibusfd, F_SETFL, 0);
	   if (tcgetattr(ibusfd, &options) != 0) return(-1);
		memset(&options, 0, sizeof(options)); /* Structur loeschen, ggf. vorher sichern
                                          und bei Programmende wieder restaurieren */
     /* Baudrate setzen */
     cfsetispeed(&options, B115200);
     cfsetospeed(&options, B115200);

     /* setze Optionen */
     options.c_cflag &= ~PARENB;         /* kein Paritybit */
     options.c_cflag &= ~CSTOPB;         /* 1 Stoppbit */
     options.c_cflag &= ~CSIZE;          /* 8 Datenbits */
     options.c_cflag |= CS8;

     /* 19200 bps, 8 Datenbits, CD-Signal ignorieren, Lesen erlauben */
     options.c_cflag |= (CLOCAL | CREAD);

     /* Kein Echo, keine Steuerzeichen, keine Interrupts */
     options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
     options.c_iflag = IGNPAR;           /* Parity-Fehler ignorieren */
     options.c_oflag &= ~OPOST;          /* setze "raw" Input */
     options.c_cc[VMIN]  = 0;            /* warten auf min. 0 Zeichen */
     options.c_cc[VTIME] = 10;           /* Timeout 1 Sekunde */
     tcflush(ibusfd,TCIOFLUSH);              /* Puffer leeren */
    if (tcsetattr(ibusfd, TCSAFLUSH, &options) != 0) return(-1);
	}
  return(ibusfd);
}

  
char Data[101];   /* Eingabepuffer für die komplette Eingabe */
int anz;         /* gelesene Zeichen */
char c;          /* Eingabepuffer fuer 1 Byte */
int  i;          /* Zeichenposition bzw Index */
//   ...
void ReadData(void) {
	i = 0;
	while (c != 0xFF && i < 100 && anz >= 0){
		anz = read(ibusfd, (void*)&c, 1);
		if (anz > 0){
			if (c != 0xFF)
				Data[i++] = c;
		}
	}
	if (anz < 0)
		perror("Read failed!");
	else if (i == 0)
		perror("No data!");    /* im Normalbetrieb loeschen!            */
	else {
		Data[i] = '\0';        /* Stringterminator */
		printf("%i Bytes: %s", i, Data);
	}
}

#endif
