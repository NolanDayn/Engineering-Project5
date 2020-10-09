#ifndef __SERIALCONFIG__H
#define __SERIALCONFIG__H

int ConfigSerialPort(int serial_port);
int SendSerialMsg(int serial_port, char* msg);
int ReadSerialMsg(int serial_port, char *readBuf, int buffSize);

#endif