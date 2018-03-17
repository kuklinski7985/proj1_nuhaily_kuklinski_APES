/**
* @file ipc_messq.c
* @brief fxn definition for queue creation and use
* @author Andrew Kuklinski and Adam Nuhaily
* @date 03/11/2018
**/

#include "ipc_messq.h"

void shuffler_king()
{ 
  char ipc_queue_buff[IPC_ELEMENT_SIZE];
  ipcmessage_t * main_message = (ipcmessage_t*)malloc(sizeof(ipcmessage_t));

  mq_receive(ipc_queue, ipc_queue_buff, IPC_ELEMENT_SIZE, NULL);
  printf("Main Q read message: %s\n", ipc_queue_buff);
  
  //change this to ipcmessage struct member destination
  int destination = 2;
  
  switch(destination){
    case(1):
      mq_send(temp_ipc_queue,"message sent from main to temp\0",31, 0);
      break;
    case(2):
      mq_send(light_ipc_queue,"message from main to light\0",27, 0);
      break;
    case(3):
      //mq_send(logger_ipc_queue, "message from main to logger\0",28,0);
      break;
    default:
      printf("Destination not valid\n");
  }
  free(main_message);
}

void shuffler_mini_temp()
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
      //printf("remaining on temp queue %ld\n",temp_ipc_attr.mq_curmsgs);
      //printf("Temp Q read message: %s\n",temp_ipc_queue_buff);
    }
}

void ipc_queue_init()
{
  //struct mq_attr ipc_attr;
  
  ipc_attr.mq_maxmsg = 255;
  ipc_attr.mq_msgsize = sizeof(char)*IPC_ELEMENT_SIZE;
  ipc_attr.mq_flags = 0;

  ipc_queue = mq_open("/ipcmain", O_CREAT | O_RDWR, 0666, &ipc_attr);

}

void temp_ipc_queue_init()
{
  //struct mq_attr temp_ipc_attr;

  temp_ipc_attr.mq_maxmsg = 255;
  temp_ipc_attr.mq_msgsize = sizeof(char)*IPC_ELEMENT_SIZE;
  temp_ipc_attr.mq_flags = 0;

  temp_ipc_queue = mq_open("/ipctemperature", O_CREAT | O_RDWR, 0666, &temp_ipc_attr);

  sigevent_temp_ipc_notify.sigev_notify = SIGEV_THREAD;
  sigevent_temp_ipc_notify.sigev_notify_function = shuffler_mini_temp;
  sigevent_temp_ipc_notify.sigev_notify_attributes = NULL;
  sigevent_temp_ipc_notify.sigev_value.sival_ptr = NULL;
  if (mq_notify(temp_ipc_queue, &sigevent_temp_ipc_notify) == -1)
    {
      printf("mq_notify error: %s\n", strerror(errno));
    }
  
}

void light_ipc_queue_init()
{
  //struct mq_attr light_ipc_attr;

  light_ipc_attr.mq_maxmsg = 255;
  light_ipc_attr.mq_msgsize = sizeof(char)*IPC_ELEMENT_SIZE;
  light_ipc_attr.mq_flags = 0;

  light_ipc_queue = mq_open("/ipclight", O_CREAT | O_RDWR, 0666, &temp_ipc_attr);

  sigevent_light_ipc_notify.sigev_notify = SIGEV_THREAD;
  sigevent_light_ipc_notify.sigev_notify_function = shuffler_mini_light;
  sigevent_light_ipc_notify.sigev_notify_attributes = NULL;
  sigevent_light_ipc_notify.sigev_value.sival_ptr = NULL;
  if (mq_notify(temp_ipc_queue, &sigevent_temp_ipc_notify) == -1)
    {
      printf("mq_notify error: %s\n", strerror(errno));
    }
  
}

void shuffler_mini_light()
{
  char light_ipc_queue_buff[IPC_ELEMENT_SIZE];
  printf("entering light shuffler\n");
  
  if (mq_notify(light_ipc_queue, &sigevent_light_ipc_notify) == -1)
    {
      printf("mq_notify error: %s\n", strerror(errno));
    }
  
  mq_getattr(light_ipc_queue, &light_ipc_attr);
  while(light_ipc_attr.mq_curmsgs > 0)
    {
      mq_receive(light_ipc_queue, light_ipc_queue_buff, IPC_ELEMENT_SIZE, NULL);
      mq_getattr(light_ipc_queue, &light_ipc_attr);
      //printf("remaining on temp queue %ld\n",light_ipc_attr.mq_curmsgs);
      printf("Light Q read message: %s\n",light_ipc_queue_buff);
      sleep(1);
    }
}
