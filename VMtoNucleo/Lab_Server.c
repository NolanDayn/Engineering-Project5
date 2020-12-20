/*  Lab_Server.c: Acts as a server for receiving commands
*   from client's controller using TCP sockets. It will then
*   send these commands over serial interface to the Nucleo
*   
*   Writen By Nolan Thomas
*   December 4, 2020
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <sys/wait.h>

#include <fcntl.h> //Contains file controls like O_RDWR
#include <errno.h> //Error integer and strerror() function
#include <termios.h> //Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

#define BUFF_SIZE 5
static int serial_port;
static int server_socket, client_socket;

void exitHandler(); //Handles killing of processes

int main(int argc, char* argv[]){

	socklen_t client_len;
	struct sockaddr_in client_addr, server_addr;
	char bufClient[BUFF_SIZE];
	char msgFromNucleo[BUFF_SIZE+1];
	char msgToNucleo[BUFF_SIZE];

	serial_port = open("/dev/ttyS0", O_RDWR);

	if(serial_port < 0){
		printf("ERROR: Could not open serial port\n");
		return 1;
	}

	// Configuring the serial port
	// Create new termios struct
  	struct termios tty;
	
	// Read in existing settings, and handle any error
	if(tcgetattr(serial_port, &tty) != 0) {
		printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
		return 1;
	}

	// IMPORTANT - parity bit setting, size setting, and stop bit setting.  Match with Embedded target!
  	tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
 	tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
  	tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
  	tty.c_cflag |= CS8; // 8 bits per byte (most common)

	//tty.c_lflag |= ICANON;

	//Not so important.  more about formatting and confirmation of transmission.
  	//[RESEARCH]: any impact if these echo-related flags are set or cleared in non-canonical mode?
  	tty.c_lflag &= ~ECHO; // Disable echo
  	tty.c_lflag &= ~ECHOE; // Disable erasure
  	tty.c_lflag &= ~ECHONL; // Disable new-line echo
  

	tty.c_cc[VMIN] = 4;      //VMIN determines the min number of characters rcv'd before unblocking read()

	// Set in/out baud rate to be 9600
  	cfsetispeed(&tty, B9600);
  	cfsetospeed(&tty, B9600);

	// Save tty settings, also checking for error
  	if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
      printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
      return 1;
  	}

	signal(SIGINT, exitHandler);


    if(argc != 2){
        printf ("usage: c PORT_NUMBER\n");
		return 1;
    }


    //Create a socket for the server
	if ((server_socket = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
		printf ("ERROR: Unable to creater server socket \n");
		return 1;
	}


	// initialize our server address info for binding purposes
	memset (&server_addr, 0, sizeof (server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl (INADDR_ANY);
	server_addr.sin_port = htons (atoi(argv[1]));

    if (bind (server_socket, (struct sockaddr *)&server_addr, 
	sizeof (server_addr)) < 0) {
		printf ("ERROR: can't bind server socket\n");
		close (server_socket);
		return 2;
	}

    //Listen on the socket
    if (listen (server_socket, 5) < 0) {
		printf ("grrr, can't listen on socket\n");
		close (server_socket);
		return 3;
	}
    /*
    * For this server we will only accept on incoming connection at
    * a time because multiple would lead to confusion and trouble.
    */

    client_len = sizeof (client_addr);
    if ((client_socket = accept (server_socket, 
    (struct sockaddr *)&client_addr, &client_len)) < 0) {
        printf ("Error: can't accept a packet from client\n");
        close (server_socket);
        return 4;
    }	

    printf("Got connection!\n");

    while(1){

		read (client_socket, bufClient,  sizeof(bufClient));

		strcpy(msgToNucleo, bufClient);
		write(serial_port, msgToNucleo, sizeof(msgToNucleo));

		int num_bytes = read(serial_port, &msgFromNucleo, sizeof(msgFromNucleo));
		if(num_bytes < 0){
			printf("Error reading: %s",strerror(errno));
			return 1;
		}

		if(strncmp(msgFromNucleo, msgToNucleo, BUFF_SIZE) != 0){
			printf("WARNING: MSG sent to robot not the same as returned!\n");
		}	

		printf("From Client: %s\n", bufClient);
		printf("From Server: %s\n", msgToNucleo);
		printf("From Robot: %c%c%c%c%c\n", msgFromNucleo[0],msgFromNucleo[1],msgFromNucleo[2],msgFromNucleo[3],msgFromNucleo[4]);


		write (client_socket, bufClient, BUFF_SIZE);
    }

    return 0;

}

void exitHandler(){
    close (server_socket);
	close(client_socket);
	close(serial_port);
	exit(1);
}
