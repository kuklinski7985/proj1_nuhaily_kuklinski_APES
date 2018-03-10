/**
* @file temp_ops.h
* @brief fxn prototypes for temp sensor thread and timer
* @author Andrew Kuklinski and Adam Nuhaily
* @date 03/11/2018
**/


#ifndef temp_ops_h_
#define temp_ops_h_

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

void *temp_ops();

void temp_ops_exit(int signum);

void metric_counter_init(unsigned long long int firedelay);




#endif /*__temp_ops_h_*/
