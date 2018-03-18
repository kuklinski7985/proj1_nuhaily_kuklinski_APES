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

/**
 *@brief enum for choosing the temp units
 */
typedef enum {
    UNITS_NONE, UNITS_F, UNITS_C, UNITS_K
} temp_unit_t;

/**
 *@brief function called when thread is created
 *
 *@param "VOID" nothing
 *
 *@return VOID
 */
void *temp_ops();

/**
 *@brief exit function for thread when joined
 *
 *@param "VOID" nothing
 *
 *@return VOID
 */
void temp_ops_exit(int signum);

/**
 *@brief verifies the temp sensor is alive, returns data, and ready to go
 *
 *@param "VOID" nothing
 *
 *@return VOID
 */
int temp_power_test();

/**
 *@brief counter for the temp sensor to transmit data to the logger at specific intervals
 *
 *@param "VOID" nothing
 *
 *@return VOID
 */
void metric_counter_init(unsigned long long int firedelay);

//void temp_hb(union sigval arg);

#endif /*__temp_ops_h_*/
