/**
* @file ipc_messq.c
* @brief fxn definition for queue creation and use
* @author Andrew Kuklinski and Adam Nuhaily
* @date 03/11/2018
**/

#include "ipc_messq.h"

void shuffler_king()      //main Q, receives messages from all Q's
{ 

  char ipc_queue_buff[IPC_ELEMENT_SIZE];
  char log_str[IPC_ELEMENT_SIZE];
  ipcmessage_t ipc_msg;
  //mq_receive(ipc_queue, msg_str,256,NULL);
  mq_receive(ipc_queue, ipc_queue_buff, IPC_ELEMENT_SIZE, NULL);
  decipher_ipc_msg(ipc_queue_buff, &ipc_msg);
  //decipher_ipc_msg(msg_str, &ipc_msg);
  //printf("Main Q read message: %s | %s | %d\n", testing.payload, testing.timestamp, testing.destination);
  //printf("destination: %d\n", ipc_msg.destination);

  switch(ipc_msg.destination){
      case(IPC_MAIN):
        //add functionality to process message
        //doesnt need to send out message like others
        //print_ipc_msg(ipc_msg);
        break;
       case(IPC_LOG):
        //print_ipc_msg(ipc_msg);
        manage_ipc_msg(ipc_msg, 1, log_str);
        mq_send(log_queue, log_str, strlen(log_str), 0);
        break;
      case(IPC_USER):
        break;
      case(IPC_TEMP):
        mq_send(temp_ipc_queue, ipc_queue_buff,strlen(ipc_queue_buff),0);
        break;
      case(IPC_LIGHT):
        mq_send(light_ipc_queue, ipc_queue_buff,strlen(ipc_queue_buff),0);
        //print_ipc_msg(ipc_msg);
        printf("light case*****************\n");
        break;
      case(IPC_NONE):
      default:
        printf("Destination %d not valid\n", ipc_msg.destination);
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
  ipc_attr.mq_maxmsg = 255;
  ipc_attr.mq_msgsize = sizeof(char)*IPC_ELEMENT_SIZE;
  ipc_attr.mq_flags = 0;

  ipc_queue = mq_open("/ipc_main", O_CREAT | O_RDWR, 0666, &ipc_attr);
  printf("IPC queue init status: %s\n", strerror(errno));

}

void log_queue_init()
{
  struct mq_attr log_attr;
  log_attr.mq_maxmsg = 255;
  log_attr.mq_msgsize = sizeof(char)*IPC_ELEMENT_SIZE;
  log_attr.mq_flags = 0;

  log_queue = mq_open("/log", O_CREAT | O_RDWR, 0666, &log_attr);
  printf("Log queue init status: %s\n", strerror(errno));
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
  if (mq_notify(light_ipc_queue, &sigevent_light_ipc_notify) == -1)
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

void decipher_ipc_msg(char* ipc_msg, ipcmessage_t* msg_struct)
{
  int i=0;
  int j=0;
  char tmp1[1];
  char tmp2[16];
 // printf("aa\n");
  // timestamp
  for(i=0, j=0; ipc_msg[i] != '\n' && ipc_msg[i] != '\0'; i++, j++)
  {
    msg_struct->timestamp[j] = ipc_msg[i];
  }
  msg_struct->timestamp[j] = '\0';
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
//printf("gg\n");
  // message payload (terminated by null char)
  for(i++, j=0; ipc_msg[i] != '\n' && ipc_msg[i] != '\0'; i++, j++)
  {
    msg_struct->payload[j] = ipc_msg[i];
  }
  msg_struct->payload[j] = '\0';
//  printf("rr\n");
 // printf("payload: %s\n", msg_struct->payload);
}

void build_ipc_msg(ipcmessage_t msg_struct, char* ipc_msg)
{
  char tmp[256];
 // printf("a\n");
  strcpy(ipc_msg, msg_struct.timestamp);
  strcat(ipc_msg, "\n");
 // printf("b\n");
  sprintf(tmp, "%d", msg_struct.type);
  strcat(ipc_msg, tmp);
  strcat(ipc_msg, "\n");
 // printf("c\n");
  sprintf(tmp, "%d", msg_struct.source);
  strcat(ipc_msg, tmp);
  strcat(ipc_msg, "\n");
 // printf("d\n");
  sprintf(tmp, "%d", (int)msg_struct.src_pid);
  strcat(ipc_msg, tmp);
  strcat(ipc_msg, "\n");
 // printf("e\n");
  sprintf(tmp, "%d", msg_struct.destination);
  strcat(ipc_msg, tmp);
  strcat(ipc_msg, "\n");
//  printf("f\n");
  strcat(ipc_msg, msg_struct.payload);
  strcat(ipc_msg, "\n");
}

void manage_ipc_msg(ipcmessage_t msg, int log_en, char* log_str)
{
  char tmp[256];
 // printf("55\n");
  switch(msg.type)
  {
    case(DATA):
      //printf("%s", msg.timestamp);
    //  strcpy(log_str, msg.timestamp);
    // printf("1\n");
      if(msg.source == IPC_LIGHT)
      {
        //printf("Light sensor reads: %s lumens.\n", msg.payload);
        sprintf(tmp, "%s%s%s%s.\n", msg.timestamp, "Light sensor reads: ", msg.payload, " lumens");
    
      }
      else if(msg.source == IPC_TEMP)
      {
        printf("Temp sensor reads: %s degF.\n", msg.payload);
      } // we need to be able to switch between degree units, maybe another ipcmessage_t element for units?
      break;
    case(INFO):
      //  printf("64\n");    
      if(msg.source == IPC_LIGHT || msg.source == IPC_TEMP)
      {
        sprintf(tmp, "%s\n", msg.payload);
      }
      //  printf("66\n");      
      break;
    default:
      break;
  }
  
  strcpy(log_str, tmp);
  printf("%s", log_str);
}

