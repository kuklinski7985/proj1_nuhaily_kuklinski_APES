#include "ipc_messq.h"

#define IPC_ELEMENT_SIZE   128

extern file_t ipcfile;             //creates a file where queue info is stored, mounted
extern file_t tempipcfile;         //creates file where temperature queue info is stored, mounted
extern mqd_t ipc_queue;           //queue associated with main thread
extern mqd_t temp_ipc_queue;      //queue associated with temp sensor

struct mq_attr ipc_attr;
struct sigevent ipc_notify;       //when mq_notify for main triggers, function is called

void shuffler_king()
{
  /*-called everytime a new entry on the queue is present
   *-need to: read queue entry: queueHasData fxn?
   *-make decision on where to send message: switch statement
   *-connect to associated queue
   *-send message to that queue
   */
  
  char ipc_queue_buff[IPC_ELEMENT_SIZE];
  if (mq_getattr(ipc_queue, &ipc_attr) == -1)
    {
        printf("getattr status: %s\n",strerror(errno));
	return;
    }

  printf("number of elements in ipc_queue %ld\n", ipc_attr.mq_curmsgs);


  do{
    mq_receive(ipc_queue, ipc_queue_buff, IPC_ELEMENT_SIZE, NULL);
    //printf("Error number receive: %s\n", strerror(errno) );

    //printf("received message: %s\n", ipc_queue_buff);

    //change this to ipcmessage struct member destination
    int destination = 1;
  
    switch(destination){
    case(1):
      //mq_send(temp_ipc_queue, ipc_queue_buff, IPC_ELEMENT_SIZE, 0);
      printf("message sent to receipient\n");
      break;
    case(2):
      printf("case 2\n");
      break;
    case(3):
      printf("case 3\n");
      break;
    default:
      printf("Destination not valid\n");
    }

    mq_getattr(ipc_queue, &ipc_attr);
    //mq_notify(ipc_queue, &ipc_notify);
  
  }while(ipc_attr.mq_curmsgs != 0);
  mq_notify(ipc_queue, &ipc_notify);
}

void shuffler_mini()
{
  //char temp_ipc_queue_buff[IPC_ELEMENT_SIZE];
  //mq_receive(temp_ipc_queue, temp_ipc_queue_buff, IPC_ELEMENT_SIZE, NULL);
  printf("entering and exit shuffler mini for temp thread\n");
  //printf("received: %s\n",temp_ipc_queue_buff);
}

//creates ipc queue and establishes sig_notify when there is an item on the queue
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
void temp_ipc_queue_init()
{
  struct mq_attr temp_ipc_attr;
  struct sigevent temp_ipc_notify;       //when mq_notify triggers, function is called

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

}

