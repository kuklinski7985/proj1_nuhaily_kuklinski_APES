/**
* @file maintemp.c
* @brief main fxn for project1 - APES
* @author Andrew Kuklinski and Adam Nuhaily
* @date 03/11/2018
**/

#include "main.h"

pthread_t tempops_thread;    //creates new pthread
pthread_attr_t attr;         //standard attributes for pthread

int main()
{
  int checking;                    //check value for pthread creation
  input_struct * input1;           //input for pthread,couldnt get to work w/o

  input1 = (input_struct*)malloc(sizeof(input_struct));
  input1->member1 = 1234;
  pthread_attr_init(&attr);
  checking = pthread_create(&tempops_thread, &attr, temp_ops,(void*)input1);
  if(checking)
    {
      fprintf(stderr, "Error Creating temp_ops thread");
      return -1;
    }

  pthread_join(tempops_thread,NULL);

}
