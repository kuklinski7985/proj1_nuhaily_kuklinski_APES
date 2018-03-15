/**
* @file light_ops.c
* @brief fxn definitions for temp sensor threading opertions and timer
* @author Andrew Kuklinski and Adam Nuhaily
* @date 03/11/2018
**/

#include "light_ops.h"

int bizzounce;

int lightsensor;          //used for return value for open(), file indicator

int light_addr = 0x39;       //slave address for the temp sensor
char light_readbuf[2];    //array for reading and sending data to sensor
char light_wrbuf[2] = {0};
char* i2c_path = "/dev/i2c-2";

void *light_ops()
{
  char readbuf[2];

  printf("entering light_ops\n");

  signal(SIGUSR1, light_ops_exit);    //signal handler for light_ops function
  lightsensor = i2c_init(i2c_path, light_addr);

  light_power_test();

//  light_id_test();
  light_r_id_reg(lightsensor, readbuf);
  printf("Light ID Register: 0x%x\n", readbuf[0]);



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
  char readbuf[2];
  int ch0;
  int ch1;
  printf("Read data0: ");
  light_r_adc(lightsensor, 0, readbuf);
  printf("%x %x\n", readbuf[1], readbuf[0]);
  ch0 = (int)readbuf[1] << 8 | (int)readbuf[0];
  printf("Read data1: ");
  light_r_adc(lightsensor, 1, readbuf);
  printf("%x %x\n", readbuf[1], readbuf[0]);
  ch1 = (int)readbuf[1] << 8 | (int)readbuf[0];
  printf("Lux: %f\n", counts_to_lux(ch0, ch1));
  //display_c(light_readbuf);
//  printf("Light sensor ID: %c\n", light_readbuf[0]);
//  printf("Data 0: %d\n", light_r_adc0(lightsensor));

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

void light_power_test()
{
  char readbuf[1];  // read buffer from light sensor

  printf("--Power test--\n");
  printf("Power off: \n");

  /*i2c_write(lightsensor, pwr_reg, 2);
  i2c_read(lightsensor, readbuf, 1);*/
  light_w_pwr(lightsensor, 0);
  light_r_pwr(lightsensor, readbuf);
  printf("Power reg buf contents: ");
  printf("%x", readbuf[0]);
  printf("\n");

  printf("Power on: \n");
  //pwr_reg[1] = CONTROL_BYTE;
  /*i2c_write(lightsensor, pwr_reg, 2);
  i2c_read(lightsensor, readbuf, 1);*/
  light_w_pwr(lightsensor, 1);
  light_r_pwr(lightsensor, readbuf);
  printf("Power reg buf contents: ");
  printf("%x", readbuf[0]);
  printf("\n");

}

float counts_to_lux(int ch0, int ch1)
{
  float range = (float)ch1 / (float)ch0;
  float lux;
  printf("ch0: %d ch1: %d range: %f\n", ch0, ch1, range);
  if(range > 1.3)
  {
    lux = 0;
  }
  else if(0.8 < range && range <= 1.3)
  {
    lux = (0.00146*ch0) - (0.00112*ch1);
  }
  else if(0.61 < range && range <= 0.8)
  {
    lux = (0.0128*ch0) - (0.0153*ch1);
  }
  else if(0.5 < range && range <= 0.61)
  {
    lux = (0.0224*ch0) - (0.031*ch1);
  }
  else if(0 < range && range <= 0.5)
  {
    lux = (0.0304*ch0) - (0.062*ch0*powf(ch1/ch0, 1.4));
  }
  else
  {
    lux = -1;
  }

  return lux;
}
