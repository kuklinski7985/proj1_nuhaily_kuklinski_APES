/**
* @file ipc_messq.h
* @brief fxn prototypes for queue creation and use
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

#define IPC_ELEMENT_SIZE   128

extern file_t ipcfile;         //creates a file where queue info is stored, mounted
extern file_t tempipcfile;     //creates file where temperature queue info is stored, mounted

extern mqd_t ipc_queue;        //queue associated with main thread
extern mqd_t temp_ipc_queue;    //queue associated with temp sensor
extern mqd_t light_ipc_queue;

extern struct mq_attr ipc_attr;
struct mq_attr temp_ipc_attr;
struct mq_attr light_ipc_attr;
struct sigevent sigevent_temp_ipc_notify;

struct sigevent sigevent_light_ipc_notify;


#define PAYLOAD_MAX_SIZE    256

extern struct mq_attr ipc_attr;

#ifndef ipc_messq_h_
#define ipc_messq_h_

extern mqd_t ipc_queue;
extern file_t ipcfile;

/*types of messages that are possible*/
typedef enum{
  QUERRY, DATA, INFO, TERMINATE
} message_t;

/*locations messages can be sent to and received from*/
typedef enum{
  IPC_LOG, IPC_TEMP, IPC_LIGHT, IPC_MAIN, IPC_SOCKET, IPC_USER
} location_t;


/*struct to define messages passed around to all parts of the system*/

<<<<<<< HEAD
typedef struct ipcmessage{
  char* timestamp;
=======
typedef struct ipcmessage {
  char timestamp[10];
>>>>>>> 722f41353ffab0040e92ac4ea5ca58bf7b45667a
  message_t type;                   //message identifier
  location_t source;                //where message originates from
  pid_t src_pid;                    //pid of process creating the message
  location_t destination;           //final destination for message
<<<<<<< HEAD
  char * payload;               //data being requested from sensors
=======
  char payload[PAYLOAD_MAX_SIZE];   // message to transmit
>>>>>>> 722f41353ffab0040e92ac4ea5ca58bf7b45667a
} ipcmessage_t;

void ipc_queue_init();
void shuffler_king();

void temp_ipc_queue_init();
<<<<<<< HEAD
void shuffler_mini_temp();

void light_ipc_queue_init();

void shuffler_mini_light();

=======
void shuffler_mini();
void build_ipc_msg(ipcmessage_t msg_struct, char* ipc_msg);
void decipher_ipc_msg(char* ipc_msg, ipcmessage_t* msg_struct);
>>>>>>> 722f41353ffab0040e92ac4ea5ca58bf7b45667a

#endif /* __ipc_messq_h_*/
