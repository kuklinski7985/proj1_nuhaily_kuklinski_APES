/**
* @file light_ops.c
* @brief fxn definitions for light sensor threading opertions and timer
* @author Andrew Kuklinski and Adam Nuhaily
* @date 03/11/2018
**/

#include "light_ops.h"

int bizzounce;
int lightsensor;          //used for return value for open(), file indicator

float light_previous;     // previously-measured light value
char * light_global;
int light_addr = 0x39;       //slave address for the temp sensor

char light_readbuf[2];    //array for reading and sending data to sensor
char light_wrbuf[2] = {0};
char* i2c_path = "/dev/i2c-2";

extern int light_hb_count;
extern int light_hb_err;

/**
 * @brief Light sensor thread handler function.
 * 
 * @return void* 
 */
void *light_ops()
{
  char sensorid[2];
  ipcmessage_t ipc_msg;
  char msg_str[DEFAULT_BUF_SIZE];
  light_previous = 1.0; // initialize at day-night border
  //signal(SIGUSR1, light_ops_exit);    //signal handler for light_ops function

  // Form and send thread wakeup message through IPC queue to logfile
  strcpy(ipc_msg.timestamp, getCurrentTimeStr());
  ipc_msg.type = INFO;
  ipc_msg.source = IPC_LIGHT;
  ipc_msg.destination = IPC_LOG;
  ipc_msg.src_pid = getpid();
  strcpy(ipc_msg.payload, "Light sensor ops thread initialized.\n");
  build_ipc_msg(ipc_msg, msg_str);
  mq_send(ipc_queue, msg_str, strlen(msg_str), 0);

  // Initialize light sensor
  lightsensor = i2c_init(i2c_path, light_addr);

  // Light sensor power-on and ID test
  if(light_power_test() == 2) 
  {
    light_r_id_reg(lightsensor, sensorid);
    // Send light sensor power-on state to logfile through main IPC queue
    strcpy(ipc_msg.timestamp, getCurrentTimeStr());
    ipc_msg.type = INFO;
    ipc_msg.source = IPC_LIGHT;
    ipc_msg.destination = IPC_LOG;
    ipc_msg.src_pid = getpid(); // pid_t
    sprintf(ipc_msg.payload, "%s%x%s", "Connecting to light sensor: ID=0x", sensorid[0], "\0");
    build_ipc_msg(ipc_msg, msg_str);
      printf("light str:\n%s\n", msg_str);
    mq_send(ipc_queue, msg_str, strlen(msg_str), 0);
  }
  else
  {
    light_r_id_reg(lightsensor, sensorid);
    // Send light sensor power-on state to logfile through main IPC queue
    strcpy(ipc_msg.timestamp, getCurrentTimeStr());
    ipc_msg.type = MSG_ERROR;
    ipc_msg.source = IPC_LIGHT;
    ipc_msg.destination = IPC_LOG;
    ipc_msg.src_pid = getpid(); // pid_t
    sprintf(ipc_msg.payload, "%s%s", "Light sensor failed to connect.", "\0");
    build_ipc_msg(ipc_msg, msg_str);
    mq_send(ipc_queue, msg_str, strlen(msg_str), 0);
  }

  // Initialize light counter
  unsigned long long int delay_time = 500000000;  //in nanoseconds
  light_counter_init(delay_time);

  while(bizzounce == 0)
  {
    // Light thread heartbeat/watchdog update
    light_hb_count = 0;
    light_hb_err = 0;
  }

  // Form and send light sensor graceful exit message to log
  strcpy(ipc_msg.timestamp, getCurrentTimeStr());
  ipc_msg.type = INFO;
  ipc_msg.source = IPC_TEMP;
  ipc_msg.destination = IPC_LOG;
  ipc_msg.src_pid = getpid();
  strcpy(ipc_msg.payload, "Light sensor ops thread exiting.\n");
  build_ipc_msg(ipc_msg, msg_str);
  mq_send(ipc_queue, msg_str, strlen(msg_str), 0);

  return 0;

}

/**
 * @brief Handle exit signal for light sensor thread
 * 
 * @param signum 
 */
void light_ops_exit(int signum)
{
  printf("exit signal received : light_ops thread!\n\n");
  bizzounce=1;
}

/**
 * @brief Handler function for light sensor timer overflow. Collect data and send to log.
 * 
 * @param sigval 
 */
void light_timer_handler(union sigval arg)
{
  char readbuf[2];
  char msg_str[DEFAULT_BUF_SIZE];
  int ch0;
  int ch1;
  ipcmessage_t ipc_msg;
  float light_current = 0;
  
  // Read 2-byte data from ADC and format
  light_r_adc(lightsensor, 0, readbuf);
  ch0 = (int)readbuf[1] << 8 | (int)readbuf[0];
  light_r_adc(lightsensor, 1, readbuf);
  ch1 = (int)readbuf[1] << 8 | (int)readbuf[0];

  // Form logfile string and transmit through IPC queue
  strcpy(ipc_msg.timestamp, getCurrentTimeStr());
  ipc_msg.type = DATA;
  ipc_msg.source = IPC_LIGHT;
  ipc_msg.destination = IPC_LOG;
  ipc_msg.src_pid = getpid(); // pid_t
  light_current = counts_to_lux(ch0, ch1);
  sprintf(ipc_msg.payload, "%f", light_current);

  build_ipc_msg(ipc_msg, msg_str);

  // now place on ipc queue, main can display and translate to log_type_t
  mq_send(ipc_queue, msg_str, strlen(msg_str), 0);
  memset(msg_str, 0, strlen(msg_str));

  // Detect day/night transitions, form logfile and send through IPC queue
  if(light_current > 1 && light_previous <= 1)
  {
    strcpy(ipc_msg.timestamp, getCurrentTimeStr());
    ipc_msg.type = INFO;
    ipc_msg.source = IPC_LIGHT;
    ipc_msg.destination = IPC_LOG;
    ipc_msg.src_pid = getpid(); // pid_t
    strcpy(ipc_msg.payload, "It is now daytime (lux > 1.0).\n");
    build_ipc_msg(ipc_msg, msg_str);
    mq_send(ipc_queue, msg_str, strlen(msg_str), 0);
  }

  else if(light_current < 1 && light_previous >= 1)
  {
    strcpy(ipc_msg.timestamp, getCurrentTimeStr());
    ipc_msg.type = INFO;
    ipc_msg.source = IPC_LIGHT;
    ipc_msg.destination = IPC_LOG;
    ipc_msg.src_pid = getpid(); // pid_t
    strcpy(ipc_msg.payload, "It is now nighttime (lux < 1.0).\n");
    build_ipc_msg(ipc_msg, msg_str);
    mq_send(ipc_queue, msg_str, strlen(msg_str), 0);
  }
  light_previous = light_current;
  
}

/**
 * @brief Initialize light sensor counter
 * 
 * @param firedelay 
 */
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

  // Ignore passed-in value for now, configure for 1 second iteration
  timer_interval.it_value.tv_sec = 1; //firedelay / 10000000000;
  timer_interval.it_value.tv_nsec = 0; //firedelay % 10000000000;
  timer_interval.it_interval.tv_sec = timer_interval.it_value.tv_sec;//0;
  timer_interval.it_interval.tv_nsec = timer_interval.it_value.tv_nsec;//0;

  // Create timer with configured settings
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

/**
 * @brief Test light sensor by attempting to power it on and read control register
 * 
 * @return int 
 */
int light_power_test()
{
  char readbuf[1];  // read buffer from light sensor
  int pass_count = 0;

  light_w_pwr(lightsensor, 0);
  light_r_pwr(lightsensor, readbuf);
  if(readbuf[0] == 0x00)
  {
    pass_count++;
  }

  light_w_pwr(lightsensor, 1);
  light_r_pwr(lightsensor, readbuf);
  if(readbuf[0] == 0x03)
  {
    pass_count++;
  }

  return pass_count;
}

/**
 * @brief Convert 2-byte ADC counts to lux (float) (see ADPS-9301 datasheet)
 * 
 * @param ch0 
 * @param ch1 
 * @return float 
 */
float counts_to_lux(int ch0, int ch1)
{
  float range = (float)ch1 / (float)ch0;
  float lux;

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
