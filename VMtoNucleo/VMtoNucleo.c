// C library headers
#include <stdio.h>
#include <string.h>

//Linux headers
#include <fcntl.h>    //Contains file controls like 0 RDWR
#include <errno.h>    //Error integer and strerror() function
#include <termios.h>  //Contains POSIX terminal control definitions
#include <unistd.h>   // write(), read() close()

#include "SerialConfig.h"

int main(int argc, char *argv[]){

	//ADD: Ability to enter serial port from cmd
	//open the serial port. Change device path as needed
	int serial_port = open("/dev/ttyS2", O_RDWR);

	if(!ConfigSerialPort(serial_port)){
		printf("Error Configuring Serial Port\n");
		return 1;
	}

	char *msg = "Hello, there!";
	SendSerialMsg(serial_port, msg);

    while(1){
        char readBuf[255];
         ReadSerialMsg(serial_port, readBuf, 255);
    }
	

  	close(serial_port);

	return 0;
}
