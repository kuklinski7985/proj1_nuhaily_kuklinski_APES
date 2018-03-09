/**
* @file maintemp.c
* @brief main fxn for project1 - APES
* @author Andrew Kuklinski and Adam Nuhaily
* @date 03/11/2018
**/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include "i2c_wrapper.h"
#include "tempsense.h"
#include "temp_ops.h"

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

  

