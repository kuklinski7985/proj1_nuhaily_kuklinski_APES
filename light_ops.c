/**
* @file light_ops.c
* @brief fxn definitions for temp sensor threading opertions and timer
* @author Andrew Kuklinski and Adam Nuhaily
* @date 03/11/2018
**/

#include "light_ops.h"

int bizzounce;

int lightsensor;
int lightsensor;          //used for return value for open(), file indicator

int light_addr = 0x39;       //slave address for the temp sensor
char light_readbuf[2] = {0};    //array for reading and sending data to sensor
char light_wrbuf[2] = {0};
char* i2c_path = "/dev/i2c-2";

void *light_ops()
{
  printf("entering light_ops\n");

  signal(SIGUSR1, light_ops_exit);    //signal handler for light_ops function
  lightsensor = i2c_init(i2c_path, light_addr);
  unsigned long long int delay_time = 500000000;  //in nanoseconds
  light_counter_init(delay_time);
  while(bizzounce == 0);  //keeps the thread alive to process signals and timer requests

  return 0;
}

void light_ops_exit(int signum)
{
  printf("exit signal received : light_ops thread!\n\n");
  bizzounce=1;
}

void light_timer_handler(union sigval arg)
{
  //
  light_r_id_reg(lightsensor, light_readbuf);
  //display_c(light_readbuf);
  printf("Light sensor ID: %s\n", light_readbuf);
  printf("Data 0: %d\n", light_r_adc0(lightsensor));

  //display_k(light_readbuf);
}

void light_counter_init(unsigned long long int firedelay)
{
  timer_t timer;
  //sets values for timer interval and initial expiration
  struct itimerspec timer_interval;

  //descibe the way a process is to be notified about and event
  struct sigevent timer_actions;

  timer_actions.sigev_notify = SIGEV_THREAD;
  timer_actions.sigev_value.sival_ptr = &timer;
  timer_actions.sigev_notify_function = light_timer_handler;
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
