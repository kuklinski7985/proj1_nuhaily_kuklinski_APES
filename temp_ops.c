/**
* @file temp_ops.c
* @brief fxn definitions for temp sensor threading opertions and timer
* @author Andrew Kuklinski and Adam Nuhaily
* @date 03/11/2018
**/

#include "temp_ops.h"
#include "ipc_messq.h"

extern int bizzounce;
int tempsensor;

int tempsensor;          //used for return value for open(), file indicator

int temp_addr = 0x48;       //slave address for the temp sensor
char readinfo[2] = {0};    //array for reading and sending data to sensor
char writeinfo[2] = {0};
char * tempsense_path = "/dev/i2c-2";

void *temp_ops()
{
 // printf("entering temp_ops\n");

  signal(SIGUSR1, temp_ops_exit);    //signal handler for temp_ops function
  tempsensor = i2c_init(tempsense_path, temp_addr);
  unsigned long long int delay_time = 500000000;  //in nanoseconds
  metric_counter_init(delay_time);
  while(bizzounce == 0)
    {
    //  mq_send(ipc_queue,"message from temp to main\0",26, 0);
      usleep(500000);   //500000 sends every half a second
    }

  return 0;
}

void temp_ops_exit(int signum)
{
  printf("exit signal received : temp_ops thread!\n\n");
  bizzounce=1;
}

void handler_timer(union sigval arg)
{
  //
  r_temp_reg(tempsensor,readinfo);
  //display_c(readinfo);
  //display_f(readinfo);
  //display_k(readinfo);
}

void metric_counter_init(unsigned long long int firedelay)
{
  timer_t timer;
  //sets values for timer interval and initial expiration
  struct itimerspec timer_interval;

  //descibe the way a process is to be notified about and event
  struct sigevent timer_actions;

  timer_actions.sigev_notify = SIGEV_THREAD;
  timer_actions.sigev_value.sival_ptr = &timer;
  timer_actions.sigev_notify_function = handler_timer;
  timer_actions.sigev_notify_attributes = NULL;

  timer_interval.it_value.tv_sec = firedelay / 10000000000;
  timer_interval.it_value.tv_nsec = firedelay % 10000000000;
  timer_interval.it_interval.tv_sec = timer_interval.it_value.tv_sec;//0;
  timer_interval.it_interval.tv_nsec = timer_interval.it_value.tv_nsec;//0;

  int timer_status;
  timer_status = timer_create(CLOCK_REALTIME, &timer_actions, &timer);  //creates new timer
  if(timer_status == -1)
    {
      printf("timer create failed\n");
      return;
    }

  timer_status = timer_settime(timer, 0, &timer_interval,0);    //this starts the counter
  if(timer_status == -1)
    {
      printf("timer start failed\n");
      return;
    }
}
