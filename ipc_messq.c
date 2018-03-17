#include "ipc_messq.h"

#define IPC_ELEMENT_SIZE   128

extern file_t ipcfile;             //creates a file where queue info is stored, mounted
extern file_t tempipcfile;         //creates file where temperature queue info is stored, mounted
extern mqd_t ipc_queue;           //queue associated with main thread
extern mqd_t temp_ipc_queue;      //queue associated with temp sensor

extern struct mq_attr ipc_attr;
struct mq_attr temp_ipc_attr;
struct sigevent sigevent_temp_ipc_notify;       //when mq_notify for main triggers, function is called

void shuffler_king()
{
  /*-called everytime a new entry on the queue is present
   *-need to: read queue entry: queueHasData fxn?
   *-make decision on where to send message: switch statement
   *-connect to associated queue
   *-send message to that queue
   */

{
  char ipc_queue_buff[IPC_ELEMENT_SIZE];

  mq_receive(ipc_queue, ipc_queue_buff, IPC_ELEMENT_SIZE, NULL);
  printf("received message main queue: %s\n", ipc_queue_buff);
  //sleep(1);

    //change this to ipcmessage struct member destination
    int destination = 1;

    switch(destination){
  //change this to ipcmessage struct member destination
  int destination = 1;

  switch(destination){
    case(1):
      mq_send(temp_ipc_queue,"message sent from main to temp\0",31, 0);
      printf("message sent to receipient\n");
      break;
    case(2):
      //mq_send(light_ipc_queue,"message from main to light\0",27, 0);
      printf("case 2\n");
      break;
    case(3):
      //mq_send(logger_ipc_queue, "message from main to logger\0",28,0);
      printf("case 3\n");
      break;
    default:
      printf("Destination not valid\n");
    }

    //mq_getattr(ipc_queue, &ipc_attr);
    //mq_notify(ipc_queue, &ipc_notify);

    //}while(ipc_attr.mq_curmsgs != 0);
    //mq_notify(ipc_queue, &ipc_notify);
  }
}

void shuffler_mini()
{
  char temp_ipc_queue_buff[IPC_ELEMENT_SIZE];
  if (mq_notify(temp_ipc_queue, &sigevent_temp_ipc_notify) == -1)
    {
      printf("mq_notify error: %s\n", strerror(errno));
    }

  mq_getattr(temp_ipc_queue, &temp_ipc_attr);
  while(temp_ipc_attr.mq_curmsgs > 0)
    {
      mq_receive(temp_ipc_queue, temp_ipc_queue_buff, IPC_ELEMENT_SIZE, NULL);
      mq_getattr(temp_ipc_queue, &temp_ipc_attr);
      printf("remaining on temp queue %ld\n",temp_ipc_attr.mq_curmsgs);
      printf("temp queue received: %s\n",temp_ipc_queue_buff);
    }
  mq_send(ipc_queue,"message from temp to main\0",26, 0);
}

void ipc_queue_init()
{
  //struct mq_attr ipc_attr;
  //struct sigevent ipc_notify;       //when mq_notify for main triggers, function is called

  ipc_attr.mq_maxmsg = 255;
  ipc_attr.mq_msgsize = sizeof(char)*IPC_ELEMENT_SIZE;
  ipc_attr.mq_flags = 0;

  ipc_queue = mq_open("/ipcmain", O_CREAT | O_RDWR, 0666, &ipc_attr);

  //printf("main IPC queue status: ID %d | %s\n", ipc_queue, strerror(errno));

  ipc_notify.sigev_notify = SIGEV_THREAD;
  ipc_notify.sigev_notify_function = shuffler_king;   //fxn to call when triggered
  ipc_notify.sigev_notify_attributes = NULL;
  ipc_notify.sigev_value.sival_ptr = NULL;
  mq_notify(ipc_queue, &ipc_notify);      //queue and sigevent associated with new item on queue

  //printf("main mq_notify status: %s\n", strerror(errno));
}

//function that creates a queue for the temp sensor thread and initializes the sig notify
  //ipc_notify.sigev_notify = SIGEV_THREAD;
  //ipc_notify.sigev_notify_function = shuffler_king;   //fxn to call when triggered
  //ipc_notify.sigev_notify_attributes = NULL;
  //ipc_notify.sigev_value.sival_ptr = NULL;
}

void temp_ipc_queue_init()
{
  //struct mq_attr temp_ipc_attr;

  temp_ipc_attr.mq_maxmsg = 255;
  temp_ipc_attr.mq_msgsize = sizeof(char)*IPC_ELEMENT_SIZE;
  temp_ipc_attr.mq_flags = 0;

  temp_ipc_queue = mq_open("/ipctemperature", O_CREAT | O_RDWR, 0666, &temp_ipc_attr);

  printf("temperature IPC queue status: ID %d | %s\n", temp_ipc_queue, strerror(errno));

  temp_ipc_notify.sigev_notify = SIGEV_THREAD;
  temp_ipc_notify.sigev_notify_function = shuffler_king;   //fxn to call when triggered
  temp_ipc_notify.sigev_notify_attributes = NULL;
  temp_ipc_notify.sigev_value.sival_ptr = NULL;
  mq_notify(temp_ipc_queue, &temp_ipc_notify);      //queue and sigevent associated with new item on queue

  printf("temperature mq_notify status: %s\n", strerror(errno));

  sigevent_temp_ipc_notify.sigev_notify = SIGEV_THREAD;
  sigevent_temp_ipc_notify.sigev_notify_function = shuffler_mini;   //fxn to call when triggered
  sigevent_temp_ipc_notify.sigev_notify_attributes = NULL;
  sigevent_temp_ipc_notify.sigev_value.sival_ptr = NULL;
  if (mq_notify(temp_ipc_queue, &sigevent_temp_ipc_notify) == -1)
    {
      printf("mq_notify error: %s\n", strerror(errno));
    }
  
}

void build_ipc_msg(ipcmessage_t msg_struct, char* ipc_msg)
{
  char tmp[256];
//  printf("a\n");
  strcpy(ipc_msg, msg_struct.timestamp);
  strcat(ipc_msg, "\n");
//  printf("b\n");
  sprintf(tmp, "%d", msg_struct.type);
  strcat(ipc_msg, tmp);
  strcat(ipc_msg, "\n");
//  printf("c\n");
  sprintf(tmp, "%d", msg_struct.source);
  strcat(ipc_msg, tmp);
  strcat(ipc_msg, "\n");
//  printf("d\n");
  sprintf(tmp, "%d", (int)msg_struct.src_pid);
  strcat(ipc_msg, tmp);
  strcat(ipc_msg, "\n");
//  printf("e\n");
  sprintf(tmp, "%d", msg_struct.destination);
  strcat(ipc_msg, tmp);
  strcat(ipc_msg, "\n");
  //printf("f\n");
  strcat(ipc_msg, msg_struct.payload);
  strcat(ipc_msg, "\0"); // mq_ queues need a null character appended (may not be necessary because of strcat)
}

void decipher_ipc_msg(char* ipc_msg, ipcmessage_t* msg_struct)
{
  int i=0;
  int j=0;
  char tmp1[1];
  char tmp2[16];
  // timestamp
  for(i=0, j=0; ipc_msg[i] != '\n' && ipc_msg[i] != '\0'; i++, j++)
  {
    msg_struct->timestamp[j] = ipc_msg[i];
  }

  // message type
  for(i++, j=0; ipc_msg[i] != '\n' && ipc_msg[i] != '\0'; i++, j++)
  {
    tmp1[j] = ipc_msg[i];
  }
  msg_struct->type = (message_t)atoi(tmp1);

  // source process
  for(i++, j=0; ipc_msg[i] != '\n' && ipc_msg[i] != '\0'; i++, j++)
  {
    tmp1[j] = ipc_msg[i];
  }
  msg_struct->source = (location_t)atoi(tmp1);

  // source PID
  for(i++, j=0; ipc_msg[i] != '\n' && ipc_msg[i] != '\0'; i++, j++)
  {
    tmp2[j] = ipc_msg[i];
  }
  msg_struct->src_pid = (pid_t)atoi(tmp2);

  // destination process
  for(i++, j=0; ipc_msg[i] != '\n' && ipc_msg[i] != '\0'; i++, j++)
  {
    tmp1[j] = ipc_msg[i];
  }
  msg_struct->destination = (location_t)atoi(tmp1);

  // message payload (terminated by null char)
  for(i++, j=0; ipc_msg[i] != '\n' && ipc_msg[i] != '\0'; i++, j++)
  {
    msg_struct->payload[j] = ipc_msg[i];
  }
}
