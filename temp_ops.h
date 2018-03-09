/**
* @file temp_ops.h
* @brief fxn prototypes for temp sensor thread and timer
* @author Andrew Kuklinski and Adam Nuhaily
* @date 03/11/2018
**/


#ifndef temp_ops_h_
#define temp_ops_h_

typedef struct input_struct{
  int member1;
}input_struct;

void *temp_ops();

void temp_ops_exit(int signum);

void metric_counter_init(unsigned long long int firedelay);




#endif /*__temp_ops_h_*/
