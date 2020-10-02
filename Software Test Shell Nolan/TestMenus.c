#include "TestMenus.h"

static uint8_t buffer[BUFFER_SIZE];

void MainMenu(){
	
	uint8_t n;
	int choice;
	
	char* header = "\r\n\n--------------------------------\n\r" 
								 "* Mobile Controller Test Check *\n\r"
								 "* By: Nolan Thomas  V: 1.0     *\n\r"
								 "--------------------------------\n\r"
								 "Build Date: ";
	
	
	char* menu =  "\r\nMain Menu\n\n\r"
		            "1. Stepper Motor Check \n\r"
								"2. Servo Check \n\r"
								"3. DC Motor Check (Open Loop) \n\r"
								"4. DC Motor Encoder Check (Motor Speed) \n\r"
								"5. LCD Check \n\r"
								"6. RS232 Comms Check \n\r"
								"7. Gyro & Accelerometer Check \n\r"
								"Please Enter The Corresponding Number For Test: ";
	
	n = sprintf((char *)buffer, "%s%s\n", header, BUILD_DATE);
	USART_Write(USART2, buffer, n);
	n = sprintf((char *)buffer, "%s", menu);
	USART_Write(USART2, buffer, n);
	
	scanf ("%d", &choice);
	if(!checkInput(choice,1,7)){
			return;
	}
	else{
		
		switch(choice){
			case(1):
				StepperMenu();
			break;
			case(2):
				ServoMenu();
			break;
			case(3):
				DCMotorMenu();
			break;
			case(4):
				EncoderMenu();
			break;
			case(5):
				LCDMenu();
			break;
			case(6):
				RS232Menu();
			break;
			case(7):
				IMUMenu();
			break;
			default:
				return;
		}
	}
}

void StepperMenu(){
	
	uint8_t n;
	int choice;
	uint8_t positionPrompt[] = "\n\r Enter Location To Set Stepper (0 to 180): \r\n";
	uint8_t stepPrompt[] = "\n\r Set Step Size\r\n"
												 "1. Half\n\r"
												 "2. Full\n\r";
	
	while(1){
		char* header = "\r\n\n******************************\n\r"
												 "* Stepper Motor Testing Menu *\n\r"
												 "******************************\n\r";
		
		
		char* menu =  "1. Home Stepper \n\r"
									"2. Check Limit Switches \n\r"
									"3. Send Stepper To Position \n\r"
									"4. Set Step Type \n\r"
									"5. Get Stepper Position \n\r"
									"6. Exit Menu \n\r"
									"Please Enter The Corresponding Number For Test: ";
		
		n = sprintf((char *)buffer, "%s\n", header);
		USART_Write(USART2, buffer, n);
		n = sprintf((char *)buffer, "%s", menu);
		USART_Write(USART2, buffer, n);
		
		
		scanf ("%d", &choice);
		if(!checkInput(choice,1,5)){
				return;
		}
		
			switch(choice){			
				case(1):
				//	HomeStepper();
				break;
				case(2):
				// CheckLimitSwitches();
				break;
				case(3):
					USART_Write(USART2, positionPrompt,strlen((char*)positionPrompt));
					scanf ("%d", &choice);
					if(!checkInput(choice,STEPPER_MIN_POS,STEPPER_MAX_POS)){
						return;
					}	
					//StepperToPosition(choice);
				break;
				case(4):
					USART_Write(USART2, stepPrompt,strlen((char*)stepPrompt));
					scanf ("%d", &choice);
					if(!checkInput(choice,1,2)){
						return;
					}	
					//setStepType(choice);
				break;
				case(5):
					//getStepperPosition();
				break;
				default:
					return;
			}
	}
}

void ServoMenu(){
	
		uint8_t n;
		int choice;
		uint8_t prompt[] = "\n\rPlease Enter Servo Position To Set 0 to 90"; 
	
		char* header = "\r\n\n******************************\n\r"
												 "*     Servo Testing Menu     *\n\r"
												 "******************************\n\r";
		
		
		
		n = sprintf((char *)buffer, "%s\n", header);
		USART_Write(USART2, buffer, n);
		
		USART_Write(USART2,prompt, strlen((char*)prompt));
		scanf ("%d", &choice);
		
		if(!checkInput(choice,SERVO_MIN_RANGE,SERVO_MAX_RANGE)){
						return;
		}	
		//ServoSetPosition(choice);
		
		return;
}
void DCMotorMenu(){
	
	uint8_t n;
	int choice;
	uint8_t prompt[] = "\n\r Set Motor Duty Cycle -100% to 100% (negative values for backwards) : \r\n";
	
	while(1){
		char* header = "\r\n\n******************************\n\r"
												 "* DC Motor Testing Menu      *\n\r"
												 "******************************\n\r";
		
		
		char* menu =  "1. Turn On Motor Left\n\r"
									"2. Turn On Motor Right \n\r"
									"3. Turn Off Left Motor \n\r"
									"4. Turn Off Right Motor \n\r"
									"5. Exit Menu \n\r"
									"Please Enter The Corresponding Number For Test: ";
		
		n = sprintf((char *)buffer, "%s\n", header);
		USART_Write(USART2, buffer, n);
		n = sprintf((char *)buffer, "%s", menu);
		USART_Write(USART2, buffer, n);
		
		
		scanf ("%d", &choice);
		if(!checkInput(choice,1,5)){
				return;
		}
		
			switch(choice){			
				case(1):
					USART_Write(USART2, prompt,strlen((char*)prompt));
					scanf ("%d", &choice);
					if(!checkInput(choice,-100,100)){
						return;
					}	
					//DriveLeftMotor(choice);
				break;
				case(2):
					USART_Write(USART2, prompt,strlen((char*)prompt));
					scanf ("%d", &choice);
					if(!checkInput(choice,-100,100)){
						return;
					}	
					//DriveRightMotor(choice);
				break;
				case(3):
					//DriveLeftMotor(0);
				break;
				case(4):
					//DriveRightMotor(0);
				break;
				default:
					return;
			}
	}
	
}

void EncoderMenu(){
	
	uint8_t n;
	int choice = 0;
	int leftMotorSpeed = 0;
	int rightMotorSpeed = 0;
	
	char* header =   "\r\n\n******************************\n\r"
												 "* Motor Encoder Testing Menu *\n\r"
												 "******************************\n\r";
		
	n = sprintf((char *)buffer, "%s\n", header);
	USART_Write(USART2, buffer, n);
	
	while(1){
		//leftMotorSpeed = getLeftEnc();
		//rightMotorSpeed = getRightEnc();
		n = sprintf((char *)buffer, "Left Motor RPM:%d	Right Motor RPM:%d\n\r Enter 1 to Quit\n\r", leftMotorSpeed, rightMotorSpeed);
		USART_Write(USART2, buffer, n);
		scanf("%d", &choice);
		
		if(choice == 1){
			return;
		}
	}		
		
}
void LCDMenu(){
	
		uint8_t n;
		char msg[128];
		
		uint8_t prompt[] = "\n\rPlease Enter Message To Display On LCD"; 
	
		char* header = "\r\n\n******************************\n\r"
												 "*      LCD Testing Menu      *\n\r"
												 "******************************\n\r";
		
		
		
		n = sprintf((char *)buffer, "%s\n", header);
		USART_Write(USART2, buffer, n);
		
		USART_Write(USART2,prompt, strlen((char*)prompt));
		scanf ("%s", msg);
		
		//LCDDisplay(msg);
		
		return;
	
}

void RS232Menu(){
	
	uint8_t n;
	int choice;
	uint8_t prompt[] = "\n\r Enter Message You Would Like To Send : \r\n";
	char msg[256];
	
	while(1){
		char* header = "\r\n\n******************************\n\r"
												 "*  RS232 Comms Testing Menu  *\n\r"
												 "******************************\n\r";
		
		
		char* menu =  "1. Send Message\n\r"
									"2. Echo Mode\n\r"
									"3. Exit Menu \n\r"
									"Please Enter The Corresponding Number For Test: ";
		
		n = sprintf((char *)buffer, "%s\n", header);
		USART_Write(USART2, buffer, n);
		n = sprintf((char *)buffer, "%s", menu);
		USART_Write(USART2, buffer, n);
		
		
			scanf ("%d", &choice);
		if(!checkInput(choice,1,5)){
				return;
		}
		
			switch(choice){			
				case(1):
					USART_Write(USART2, prompt,strlen((char*)prompt));
					scanf ("%s", msg);
					//SendMsgRS232(msg);
				break;
				case(2):
					//EchoRS232(choice);
				break;
				default:
					return;
			}
		
	}
	
}
void IMUMenu(){
	uint8_t n;
	uint8_t data[200];
	strcpy((char*)data, "Data: N/A");
	
	char* header =   "\r\n\n******************************\n\r"
												 "* IMU Testing Menu           *\n\r"
												 "******************************\n\r";

	n = sprintf((char *)buffer, "%s\n", header);
	USART_Write(USART2, buffer, n);
	
	//getIMUData(data)
	
	USART_Write(USART2,data, strlen((char*)data));
}


int8_t checkInput(int input, int lowerBound, int upperBound){
	
	if(input < lowerBound || input > upperBound ){
		uint8_t errorMsg[] = "\n\r Selection Invalid\n\r";
		USART_Write(USART2, errorMsg,strlen((char*)errorMsg));
		return -1;
	}
	
	return input;
	
}

