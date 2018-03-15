/**
* @file ipc_messq.h
* @brief fxn prototypes for operation of tmp102 temperature sensor
* @author Andrew Kuklinski and Adam Nuhaily
* @date 03/11/2018
**/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <time.h>
#include <stdarg.h>
#include <ctype.h>
#include <mqueue.h>
#include <errno.h>

#include "logger/logger.h"



#ifndef ipc_messq_h_
#define ipc_messq_h_

extern mqd_t ipc_queue;
extern file_t ipcfile;

/*types of messages that are possible*/
typedef enum{
  QUERRY, DATA, INFO, TERMINATE
}message_t;

/*locations messages can be sent to and received from*/
typedef enum{
  IPC_LOG, IPC_TEMP, IPC_LIGHT, IPC_MAIN, IPC_SOCKET, IPC_USER
}location_t;


/*struct to define messages passed around to all parts of the system*/

typedef struct ipcmessage{
  message_t type;                   //message identifier
  location_t source;                //where message originates from
  pid_t src_pid;                    //pid of process creating the message
  location_t destination;           //final destination for message
  long int sensedata;               //data being requested from sensors
}ipcmessage_t;


void ipc_queue_init();
void shuffler_king();

void temp_ipc_queue_init();
void shuffler_mini();


#endif /* __ipc_messq_h_*/
