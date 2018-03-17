/**
 *  @file logger.h
 *  @author Adam Nuhaily
 *  @date 11 Mar 2018
 *  @brief hw3 header file
 *  This source file implements the multithreaded program described in the
 *  homework 3 assignment document.
 */
#ifndef _LOGGER_H
#define _LOGGER_H

#define _GNU_SOURCE

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <time.h>
#include <stdarg.h>
#include <ctype.h>
#include <mqueue.h>
#include <errno.h>
#include "sync_fileio.h"

#define LOG_MAX_ELEMENTS  64
#define LOG_ELEMENT_SIZE  128
#define LOG_LINE_SIZE     255

typedef enum
{
  ERROR,
  MESSAGE,
  TEMP,
  LIGHT
} log_type_t;

typedef struct
{
  char timestamp[10];
  double float_data;
  char str_data[LOG_ELEMENT_SIZE];
  int int_data;
  char data_units[LOG_ELEMENT_SIZE]; // doesn't need to be this big
  log_type_t msg_type;

} log_struct_t;




void* logger();
static void queueHasData();
void log_exit();
void writeLogStr(file_t* logfile, log_struct_t logitem);
char* getCurrentTimeStr();
int8_t thread_sprintf(char* rtn_ascii, long lng, char format[]);



#endif
