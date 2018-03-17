/**
* @file main.c
* @brief main fxn for project1 - APES
* @author Andrew Kuklinski and Adam Nuhaily
* @date 03/11/2018
**/

#include "main.h"

int main()
{
  char ipc_queue_buff[128];
  
  ipc_queue_init();           //main queue created
  temp_ipc_queue_init();      //temp sensor queue created
  light_ipc_queue_init();     //light sensor queue created
  
  int checking;                    //check value for pthread creation
  input_struct * input1;           //input for pthread,couldnt get to work w/o

  //char* test_entry = "7\n";
  char buf1[255];

  //remote_socket_server_init();

  input1 = (input_struct*)malloc(sizeof(input_struct));
  input1->member1 = 1234;
  pthread_attr_init(&attr);

  /*checking = pthread_create(&log_thread, &attr, logger, (void*)input1);
  if(checking)
    {
      fprintf(stderr, "Error Creating log thread");
      return -1;
      }*/

  /*checking = pthread_create(&tempops_thread, &attr, temp_ops,(void*)input1);
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


  if(log_queue != -1)
  {
    printf("Trying to push to queue...\n");
    mq_send(log_queue, "7", 1, 0);
    printf("mq_send: %s\n", strerror(errno));
  }

  else
  printf("log_queue == -1\n");*/

  
  /******only needed for seeding the main queue******/
  int counter = 0;
  mq_send(ipc_queue, "Seeding Message\0",16, 0);
  mq_getattr(ipc_queue, &ipc_attr);
  while(bizzounce == 0)
  {
    while(ipc_attr.mq_curmsgs > 0)
      {
	shuffler_king();
	mq_getattr(ipc_queue, &ipc_attr);
	//printf("remaining on queue: %ld\n\n",ipc_attr.mq_curmsgs);
	counter++;
        //printf("counter: %d\n",counter);
	usleep(2000);
      }
    mq_getattr(ipc_queue, &ipc_attr);
    //bizzounce = 1;
  }

  mq_close(ipc_queue);
  mq_close(temp_ipc_queue);
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
  mq_close(log_queue);
  printf("mq_close err: %s\n", strerror(errno));


  /*pthread_join(log_thread, NULL);
  mq_close(log_queue);
  printf("mq_close err: %s\n", strerror(errno));

  pthread_join(log_thread, NULL);*/

}
