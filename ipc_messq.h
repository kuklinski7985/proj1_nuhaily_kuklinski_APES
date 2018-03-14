/**
* @file ipc_messq.h
* @brief fxn prototypes for operation of tmp102 temperature sensor
* @author Andrew Kuklinski and Adam Nuhaily
* @date 03/11/2018
**/


#ifndef ipc_messq_h_
#define ipc_messq_h_

/*types of messages that are possible*/
typedef enum{
  QUERRY, DATA, INFO, TERMINATE
}message_t;

/*locations messages can be sent to and received from*/
typedef enum{
  LOG, TEMP, LIGHT, MAIN, SOCKET, USER
}location_t;


/*struct to define messages passed around to all parts of the system*/

typedef struct ipcmessage{
  message_t type;                   //message identifier
  location_t source;                //where message originates from
  pid_t src_pid;                    //pid of process creating the message
  location_t destination;           //final destination for message
  long int sensedata;               //data being requested from sensors
}ipcmessage_t;

#endif /* __ipc_messq_h_*/
