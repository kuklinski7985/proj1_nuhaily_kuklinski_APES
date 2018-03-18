/**
* @file main.c
* @brief main fxn for project1 - APES
* @author Andrew Kuklinski and Adam Nuhaily
* @date 03/11/2018
**/

#include "main.h"

pthread_t tempops_thread;    //creates new pthread
pthread_t lightops_thread;    //creates new pthread
pthread_t log_thread;
pthread_t socket_thread;
pthread_t hb_thread;

pthread_attr_t attr;         //standard attributes for pthread
file_t logfile;
file_t ipcfile;
file_t tempipcfile;

int bizzounce;
mqd_t log_queue;           //queue associated with logger
mqd_t ipc_queue;           //queue associated with main thread
mqd_t temp_ipc_queue;      //queue associated with temp sensor

struct mq_attr ipc_attr;          //attributes struct for ipc queue

int temp_hb_count;
int temp_hb_err;
int light_hb_count;
int light_hb_err;
int log_hb_count;
int log_hb_err;

int main()
{
  char ipc_queue_buff[256];
  
  ipc_queue_init();           //main queue created
  log_queue_init();
  temp_ipc_queue_init();      //temp sensor queue created
  light_ipc_queue_init();     //light sensor queue created

  int checking;                    //check value for pthread creation
  input_struct * input1;           //input for pthread,couldnt get to work w/o
  char msg_str[256];
  char buf1[255];
  ipcmessage_t ipc_msg;
 // remote_socket_server_init();

  input1 = (input_struct*)malloc(sizeof(input_struct));
  input1->member1 = 1234;
  pthread_attr_init(&attr);

  checking = pthread_create(&log_thread, &attr, logger, (void*)input1);
  if(checking)
  {
    fprintf(stderr, "Error creating log thread");
    return -1;
  }

  checking = pthread_create(&tempops_thread, &attr, temp_ops,(void*)input1);
  if(checking)
  {
    fprintf(stderr, "Error creating temp_ops thread");
    return -1;
  }

  checking = pthread_create(&lightops_thread, &attr, light_ops, (void*)input1);
  if(checking)
  {
    fprintf(stderr, "Error creating light_ops thread");
    return -1;
  }

  checking = pthread_create(&hb_thread, &attr, heartbeat, (void*)input1);
  if(checking)
  {
    fprintf(stderr, "Error creating heartbeat thread");
    return -1;
  }

  strcpy(ipc_msg.timestamp, getCurrentTimeStr());
  ipc_msg.type = INFO;
  ipc_msg.source = IPC_MAIN;
  ipc_msg.destination = IPC_LOG;
  ipc_msg.src_pid = getpid();
  strcpy(ipc_msg.payload, "Main thread initialized.\n");
  build_ipc_msg(ipc_msg, msg_str);
  mq_send(ipc_queue, msg_str, strlen(msg_str), 0);
  memset(msg_str, 0, strlen(msg_str));

 // mq_send(ipc_queue, "Seeding Message\0",16, 0);
  mq_getattr(ipc_queue, &ipc_attr);
  while(bizzounce == 0)
  {
    while(ipc_attr.mq_curmsgs > 0)
    { 
	    shuffler_king();
	    mq_getattr(ipc_queue, &ipc_attr);
    }
    if(temp_hb_err > 0)
    {
        memset(msg_str, 0, strlen(msg_str));
        strcpy(ipc_msg.timestamp, getCurrentTimeStr());
        ipc_msg.type = ERROR;
        ipc_msg.source = IPC_MAIN;
        ipc_msg.destination = IPC_LOG;
        ipc_msg.src_pid = getpid();
        strcpy(ipc_msg.payload, "Temperature thread timed out.\n");
        build_ipc_msg(ipc_msg, msg_str);
        mq_send(ipc_queue, msg_str, strlen(msg_str), 0);
        memset(msg_str, 0, strlen(msg_str));
    }
    mq_getattr(ipc_queue, &ipc_attr);
  }

  mq_close(ipc_queue);
  mq_close(temp_ipc_queue);
  mq_close(light_ipc_queue);
  if(mq_unlink("/ipcmain") == -1)
    {
      	printf("unlink error: %s\n", strerror(errno));
    }

  if(mq_unlink("/ipctemperature") == -1)
    {
      printf("unlink error: %s\n", strerror(errno));
    }

  pthread_join(tempops_thread, NULL);

  pthread_join(lightops_thread, NULL);

  pthread_join(log_thread, NULL);

  pthread_join(socket_thread,NULL);
  
  pthread_join(hb_thread, NULL);

  /*pthread_join(log_thread, NULL);
  mq_close(log_queue);
  printf("mq_close err: %s\n", strerror(errno));

  pthread_join(log_thread, NULL);*/

}

void* heartbeat()
{
  char msg_str[256];
  ipcmessage_t ipc_msg;

  strcpy(ipc_msg.timestamp, getCurrentTimeStr());
  ipc_msg.type = INFO;
  ipc_msg.source = IPC_HB;
  ipc_msg.destination = IPC_LOG;
  ipc_msg.src_pid = getpid();
  strcpy(ipc_msg.payload, "Heartbeat thread initialized.\n");
  build_ipc_msg(ipc_msg, msg_str);
  mq_send(ipc_queue, msg_str, strlen(msg_str), 0);

  temp_hb_count = 0;
  temp_hb_err = 0;
  light_hb_count = 0;
  light_hb_err = 0;
  log_hb_count = 0;
  log_hb_err = 0;

  timer_t temp_hb;
  //sets values for timer interval and initial expiration
  struct itimerspec temp_hb_interval;
  //descibe the way a process is to be notified about and event
  struct sigevent temp_hb_sig;

  temp_hb_sig.sigev_notify = SIGEV_THREAD;
  temp_hb_sig.sigev_notify_function = hb_warn;
  temp_hb_sig.sigev_value.sival_ptr = &temp_hb;
  temp_hb_sig.sigev_notify_attributes = NULL;

  temp_hb_interval.it_value.tv_sec = 1;
  temp_hb_interval.it_value.tv_nsec = 0;//10000000000;
  temp_hb_interval.it_interval.tv_sec = temp_hb_interval.it_value.tv_sec;//0;
  temp_hb_interval.it_interval.tv_nsec = temp_hb_interval.it_value.tv_nsec;//0;
  timer_create(CLOCK_REALTIME, &temp_hb_sig, &temp_hb);  //creates new timer
  timer_settime(temp_hb, 0, &temp_hb_interval, 0);    //this starts the counter

  while(bizzounce == 0)
  {
    if(temp_hb_count > 10)
    {
      printf("Heartbeat thread: error.\n");
      temp_hb_err = 1;
    }
    if(light_hb_count > 10)
    {
      printf("Heartbeat thread: error.\n");
      light_hb_err = 1;
    }
    if(log_hb_count > 10)
    {
      printf("Heartbeat thread: error.\n");
      log_hb_err = 1;
    }
  }
}

void hb_warn(union sigval arg)
{
  temp_hb_count++;
  light_hb_count++;
  log_hb_count++;
}
