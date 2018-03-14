/**
* @file maintemp.c
* @brief main fxn for project1 - APES
* @author Andrew Kuklinski and Adam Nuhaily
* @date 03/11/2018
**/

#include "main.h"

pthread_t tempops_thread;    //creates new pthread
pthread_t lightops_thread;    //creates new pthread
pthread_t log_thread;

pthread_attr_t attr;         //standard attributes for pthread
file_t logfile;

int bizzounce;
mqd_t log_queue;
mqd_t ipc_queue;

int main()
{

  
  int checking;                    //check value for pthread creation
  input_struct * input1;           //input for pthread,couldnt get to work w/o

  //char* test_entry = "7\n";
  char buf1[255];


  input1 = (input_struct*)malloc(sizeof(input_struct));

  remote_socket_server_init();
  /*input1 = (input_struct*)malloc(sizeof(input_struct));
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


  if(log_queue != -1)
  {
    printf("Trying to push to queue...\n");
    mq_send(log_queue, "7", 1, 0);
    printf("mq_send: %s\n", strerror(errno));
  }

  else
    printf("log_queue == -1\n");


  pthread_join(tempops_thread, NULL);
  pthread_join(lightops_thread, NULL);

  pthread_join(log_thread, NULL);
  mq_close(log_queue);
  printf("mq_close err: %s\n", strerror(errno));

  pthread_join(log_thread, NULL);*/

}
