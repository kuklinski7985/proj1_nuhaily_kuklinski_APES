/**
* @file main.h
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
#include "light_ops.h"
#include "remote_socket.h"
//#include "myusrled.h"

typedef struct input_struct{
  int member1;
} input_struct;




