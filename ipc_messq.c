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
  char ipc_queue_buff[IPC_ELEMENT_SIZE];
  
  mq_receive(ipc_queue, ipc_queue_buff, IPC_ELEMENT_SIZE, NULL);
  printf("received message main queue: %s\n", ipc_queue_buff);
  //sleep(1);

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
  
  ipc_attr.mq_maxmsg = 255;
  ipc_attr.mq_msgsize = sizeof(char)*IPC_ELEMENT_SIZE;
  ipc_attr.mq_flags = 0;

  ipc_queue = mq_open("/ipcmain", O_CREAT | O_RDWR, 0666, &ipc_attr);

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

  sigevent_temp_ipc_notify.sigev_notify = SIGEV_THREAD;
  sigevent_temp_ipc_notify.sigev_notify_function = shuffler_mini;   //fxn to call when triggered
  sigevent_temp_ipc_notify.sigev_notify_attributes = NULL;
  sigevent_temp_ipc_notify.sigev_value.sival_ptr = NULL;
  if (mq_notify(temp_ipc_queue, &sigevent_temp_ipc_notify) == -1)
    {
      printf("mq_notify error: %s\n", strerror(errno));
    }
  
}

