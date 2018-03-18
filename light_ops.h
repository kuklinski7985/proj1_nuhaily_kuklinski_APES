/**
* @file light_ops.h
* @brief fxn prototypes for temp sensor thread and timer
* @author Andrew Kuklinski and Adam Nuhaily
* @date 03/11/2018
**/


#ifndef light_ops_h_
#define light_ops_h_

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
#include <math.h>
//#include "i2c_wrapper.h"
#include "lightsense.h"
#include "ipc_messq.h"

#define DEFAULT_BUF_SIZE    256

void *light_ops();
void light_ops_exit(int signum);
void light_counter_init(unsigned long long int firedelay);
int light_power_test();
float counts_to_lux(int ch0, int ch1);

#endif /*__light_ops_h_*/
