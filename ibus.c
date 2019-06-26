#include <stdint.h>

/*
  The data is serial data, 115200, 8N1.
  Messages arrive every 7 milliseconds, and are read constantly until a
  few tenths of a second after the transmitter is switched off.

  Packet format:
  20 40 CH0 CH1 CH2 CH3 CH4 CH5 CH6 CH7 CH8 CH9 CH10 CH11 CH12 CH13 SUM
  Channels are stored in little endian byte order.  Unused channels read
  5DC (first byte DC, second byte 05).

  Channel 0: Right horizontal: 3E8 -> 7D0
  Channel 1: Right vertical:   3E8 -> 7CF
  Channel 2: Left vertical:    3E8 -> 7D0
  Channel 3: Left horizontal:  3E8 -> 7CD
  Channel 4: Left pot:         3E8 -> 7D0
  Channel 5: Right pot:        3E8 -> 7D0

  The checksum starts at 0xFFFF, then subtract each byte except the 
  checksum bytes.
*/

int init_UART (void) {
	//-------------------------
	//----- SETUP USART 0 -----
	//-------------------------
	//At bootup, pins 8 and 10 are already set to UART0_TXD, UART0_RXD (ie the alt0 function) respectively
	int filestream = -1;
	
	//OPEN THE UART
	//The flags (defined in fcntl.h):
	//	Access modes (use 1 of these):
	//		O_RDONLY - Open for reading only.
	//		O_RDWR - Open for reading and writing.
	//		O_WRONLY - Open for writing only.
	//
	//	O_NDELAY / O_NONBLOCK (same function) - Enables nonblocking mode. When set read requests on the file can return immediately with a failure status
	//											if there is no input immediately available (instead of blocking). Likewise, write requests can also return
	//											immediately with a failure status if the output can't be written immediately.
	//
	//	O_NOCTTY - When set and path identifies a terminal device, open() shall not cause the terminal device to become the controlling terminal for the process.
	filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
	if (filestream == -1) {
		//ERROR - CAN'T OPEN SERIAL PORT
		printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
	}
		
	//CONFIGURE THE UART
	//The flags (defined in /usr/include/termios.h - see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html):
	//	Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
	//	CSIZE:- CS5, CS6, CS7, CS8
	//	CLOCAL - Ignore modem status lines
	//	CREAD - Enable receiver
	//	IGNPAR = Ignore characters with parity errors
	//	ICRNL - Map CR to NL on input (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
	//	PARENB - Parity enable
	//	PARODD - Odd parity (else even)
	struct termios options;
	tcgetattr(filestream, &options);
	options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;		//<Set baud rate
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(filestream, TCIFLUSH);
	tcsetattr(filestream, TCSANOW, &options);
	return filestream;
}


struct ibus_state {
  uint_fast8_t state;
  uint_fast16_t checksum;
  uint_fast8_t datal;
  uint_fast8_t channel_count;
};

void ibus_init(struct ibus_state* state, uint_fast8_t channel_count) {
  state->state = 0;
  state->channel_count = channel_count;
}

int ibus_read(struct ibus_state* state, uint16_t* data, uint8_t ch) {
  switch (state->state) {
  case 0:
    if (ch == 0x20) {
      state->checksum = 0xFFFF - 0x20;
      state->state = 1;
    }
    break;
  case 1:
    if (ch == 0x40) {
      state->state = 2;
      state->checksum -= ch;
    } else {
      // Unknown packet type
      state->state = 0;
    }
    break;
  case 30:
    state->datal = ch;
    state->state = 31;
    break;
  case 31: {
    uint_fast16_t checksum = (ch << 8) | state->datal;
    state->state = 0;
    if (checksum == state->checksum)
      return 0;
  } break;
  default:
    // Ignore these bytes if we've filled all of the channels
    if (state->state / 2 <= state->channel_count) {
      if ((state->state & 1) == 0) {
        // Data low byte
        state->datal = ch;
      } else {
        // Data high byte
        data[(state->state / 2) - 1] = (ch << 8) | state->datal;
      }
    }
    state->checksum -= ch;
    ++state->state;
    break;
  }

  return -1;
}

