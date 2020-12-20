/* Game_Controller_Interface: For reading in inputs from Xbox controller
*  and then converting them to commands for the mobile robot and sending them
*  to Lab computer over internet using TCP/IP sockets.
*
*   Written By Nolan Thomas
*   December 1, 2020
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "GameController.h"
#include "Msg_Specs.h"

pthread_mutex_t cmd_mutex = PTHREAD_MUTEX_INITIALIZER;
void* InputUpdate();
void SetCmdNull(char* cmd);

static int CMD_NUM = 0;
static char CMD[CMD_BUF_SIZE][CMD_LENGTH];

int main(int argc, char* argv[]){

    pthread_t controllerThread;
    pthread_attr_t attr;

    int client_socket, len;
    struct sockaddr_in server_addr;
    struct hostent *host;
    char buffer[CMD_LENGTH];

    if(argc != 3){
        printf("usage: c IP_ADDRESS PORT_NUMBER\n");
        return 1;
    }

    //create a socket for communication
    if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("Unable to create socket");
        return 3;
    }

    //Initialize struct to get a socket to host
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));

    //Attempt connection to server
    if(connect(client_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0){
        printf("Unable to connect to server!\n");
        close(client_socket);
        return 4;
    }

    //initialize thread attribute object
    pthread_attr_init(&attr);
    //allows main thread to be blocked and wait for thread completion
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
    //enable l-l kernal-user thread mapping model
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

        //set up mutex
    if(pthread_mutex_init(&cmd_mutex, NULL) != 0){
       printf("mutex init failed!\n");
       return 1;
    }

    //create subject thread
    if(pthread_create(&controllerThread, &attr, InputUpdate, (void*)0) != 0){
        printf("Failed creating subject thread\n");
        return 2;
    }

    while(1){
        //Convert to command format
        //send command
        pthread_mutex_lock(&cmd_mutex); 
        printf("Sent CMD: %s\n",CMD[CMD_NUM]);   
        snprintf(buffer, CMD_LENGTH, "%s", CMD[CMD_NUM]);
        write(client_socket, buffer, CMD_LENGTH);
        len = read (client_socket, buffer, CMD_LENGTH);  
        printf("Server Confirmation: %s\n", buffer);
        SetCmdNull(CMD[CMD_NUM]);
        if(CMD_NUM > 0){
            CMD_NUM--;
        }
        pthread_mutex_unlock(&cmd_mutex); 
        usleep(CMD_DELAY);
    }

    //end subject thread once all observer threads are complete
    if(pthread_cancel(controllerThread) != 0){
        printf("Failed to cancel the subject thread\n");
        return 5;
    }

    //printf("%d %d %d %d\n", e.time, e.value, e.type, e.number);
    pthread_mutex_destroy(&cmd_mutex); //Clean up mutex
    pthread_attr_destroy(&attr);    //cleanup thread attributes object

    close(client_socket);
    return 0;
}




void* InputUpdate(){

    //dev/input file for controller input
    const char* joystickFile = "/dev/input/js2";
    struct js_event e;
    MotorData motorData;

    int fd = open(joystickFile, O_RDONLY);
    if( fd < 0){
        printf("Unable to open %s\n", joystickFile);
        exit(1);
    }
    else{
        printf("Opened joystick file successfully");
    }


    while(1){

        read(fd, &e, sizeof(e));
        if(e.type == JS_EVENT_BUTTON){

            pthread_mutex_lock(&cmd_mutex); 

            if(CMD_NUM >= CMD_BUF_SIZE){
                CMD_NUM = 0;
            }

            switch(e.number){
                case(CENTER_BUTTON):
                    printf("Center Button Pressed\n");
                    CenterCmd(CMD[CMD_NUM]);
                    CMD_NUM++;
                    break;
                default:
                    break;
            }
            pthread_mutex_unlock(&cmd_mutex); 
        }
        else if(e.type == JS_EVENT_AXIS){
            pthread_mutex_lock(&cmd_mutex); 

            if(CMD_NUM >= CMD_BUF_SIZE - 1){
                CMD_NUM = 0;
            }

            switch(e.number){
                case(STEPPER_AXIS):
                    printf("Stepper Axis Moved\n");
                    StepperCmd(CMD[CMD_NUM], e.value);
                    break;
                case(SERVO_AXIS):
                    printf("Servo Axis Moved\n");
                    ServoCmd(CMD[CMD_NUM], e.value);
                    break;
                case(MOTOR_FORWARD_AXIS):
                    motorData.y = e.value;
                    MotorCmd(CMD, &CMD_NUM ,&motorData);
                    break;
                case(MOTOR_TURN_AXIS):
                    motorData.x = e.value;
                    MotorCmd(CMD, &CMD_NUM,&motorData);
                    printf("Motor Turning Axis Moved\n");
                    break;                
                default:
                    break;
            }
            CMD_NUM++; //Increment the command buffer
            pthread_mutex_unlock(&cmd_mutex); 

        }
    }
}


void SetCmdNull(char* cmd){
    memset(cmd, '0', CMD_LENGTH);
}

