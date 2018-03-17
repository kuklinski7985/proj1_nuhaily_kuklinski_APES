/**
* @file main.c
* @brief main fxn for project1 - APES
* @author Andrew Kuklinski and Adam Nuhaily
* @date 03/11/2018
**/

#include "main.h"

// establish comm message protocol
// light sensor task
// logger task
// test mqueue with struct both by struct cast and void pointer cast

pthread_t tempops_thread;    //creates new pthread
pthread_t lightops_thread;    //creates new pthread
pthread_t log_thread;

pthread_attr_t attr;         //standard attributes for pthread
file_t logfile;
file_t ipcfile;
file_t tempipcfile;

int bizzounce;
mqd_t log_queue;           //queue associated with logger
mqd_t ipc_queue;           //queue associated with main thread
mqd_t temp_ipc_queue;      //queue associated with temp sensor

struct mq_attr ipc_attr;          //attributes struct for ipc queue

int main()
{
  ipc_queue_init();      //main queue created
  temp_ipc_queue_init();      //temp sensor queue created

  //char ipc_queue_buff[128] = "testing from main\0";


  int checking;                    //check value for pthread creation
  input_struct * input1;           //input for pthread,couldnt get to work w/o

  //char* test_entry = "7\n";
  char buf1[255];


  input1 = (input_struct*)malloc(sizeof(input_struct));

//  remote_socket_server_init();

  input1 = (input_struct*)malloc(sizeof(input_struct));
  input1->member1 = 1234;
  pthread_attr_init(&attr);

  checking = pthread_create(&log_thread, &attr, logger, (void*)input1);

  checking = pthread_create(&tempops_thread, &attr, temp_ops,(void*)input1);
  if(checking)
    {
      fprintf(stderr, "Error Creating temp_ops thread");
      return -1;
    }

  checking = pthread_create(&lightops_thread, &attr, light_ops, (void*)input1);
  if(checking)
  {
    fprintf(stderr, "Error Creating light_ops thread");
    return -1;
  }


/*  if(log_queue != -1)
  {
    printf("Trying to push to queue...\n");
    mq_send(log_queue, "7", 1, 0);
    printf("mq_send: %s\n", strerror(errno));
  }*/

  else
    printf("log_queue == -1\n");

  while(bizzounce == 0);

  //mq_send(ipc_queue, "while testing outside\0", 128, 0);
/*  while(bizzounce == 0)  //keeps the thread alive
  {
    //this for loop only for filling the ipc main queue
    for(int i=0; i<5;i++)
      {
	printf("loop %d\n", i);
	mq_send(ipc_queue, "testing 1\0",10, 0);

	//printf("Error number while: %s\n\n", strerror(errno));
	//usleep(10000);
      }
    mq_getattr(ipc_queue, &ipc_attr);
    printf("number of queued items: %ld\n",ipc_attr.mq_curmsgs);

    // this while loop for reading off the queue

    while(ipc_attr.mq_curmsgs > 0)
      {
	     shuffler_king();
	     mq_getattr(ipc_queue, &ipc_attr);
	     printf("remaining on queue: %ld\n",ipc_attr.mq_curmsgs);
      }
    printf("done*******************\n");
    mq_getattr(ipc_queue, &ipc_attr);
    printf("number of queued items: %ldn",ipc_attr.mq_curmsgs);
    bizzounce = 1;
  }*/

  mq_close(ipc_queue);
  //mq_close(temp_ipc_queue);
  if(mq_unlink("/ipcmain") == -1)
    {
      	printf("unlink error: %s\n", strerror(errno));
    }

  pthread_join(tempops_thread, NULL);
  pthread_join(lightops_thread, NULL);

  pthread_join(log_thread, NULL);
  mq_close(log_queue);
  printf("mq_close err: %s\n", strerror(errno));


  pthread_join(log_thread, NULL);
  mq_close(log_queue);
  printf("mq_close err: %s\n", strerror(errno));

  pthread_join(log_thread, NULL);

}
