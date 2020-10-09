/* Program demonstrating how to read input from controller
*  or joystick in linux.
*
*  October 9, 2020
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define JS_EVENT_BUTTON 0x01 //Button pressed/released
#define JS_EVENT_AXIS 0x02   //Joystick moved
#define JS_EVENT_INIT 0x80   //Initial state of device

//Struct representing device input data structure linux
struct js_event{

    unsigned int time;  //Event timestamp in milliseconds
    short value;        //value
    unsigned char type; //event type
    unsigned char number; //axis/button number
};

int main(){

    const char* joystickFile = "/dev/input/js1";

    int fd = open(joystickFile, O_RDONLY);

    if( fd < 0){
        printf("%s\n", joystickFile);
    }
    else{
        printf("Opened joystick file successfully");
    }

    struct js_event e;

    while(1){
        read(fd, &e, sizeof(e));

        //printf("%d %d %d %d\n", e.time, e.value, e.type, e.number);

        if(e.type == JS_EVENT_BUTTON || e.type == JS_EVENT_AXIS){
            
            if(e.type == JS_EVENT_BUTTON){
                printf("Button:%d   Value:%d", (int)e.number, e.value);
            }

            else{
                printf("Axis:%d   Value:%d", (int)e.number, e.value);
            }
        }
    }

    return 0;
}