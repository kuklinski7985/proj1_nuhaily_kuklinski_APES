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
  //struct sigevent ipc_notify;       //when mq_notify for main triggers, function is called

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
