/**
* @file lightsense.h
* @brief fxn prototypes for operation of tmp102 temperature sensor
* @author Andrew Kuklinski and Adam Nuhaily
* @date 03/11/2018
**/


#ifndef lightsense_h_
#define lightsense_h_


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "i2c_wrapper.h"

#define PTRREG_SD_MASK      0x01
#define PTRREG_TM_MASK      0x02
#define PTRREG_POL_MASK     0X04
#define PTRREG_F1F0_MASK(x) (x<<4)
#define PTRREG_R1R0_MASK(X) (x<<6)
#define PTRREG_OS_MASK      0x80

#define CONTROL_BYTE        ((char)0x03)

typedef enum{
  CONTROL,
  TIMING,
  TLOW_L,
  TLOW_H,
  THIGH_L,
  THIGH_H,
  INTERRUPT,
  UNUSEDA,
  CRC,
  UNUSEDB,
  ID,
  UNUSEDC,
  DATA0_L,
  DATA0_H,
  DATA1_L,
  DATA1_H
} lightsensor_reg;

void light_r_id_reg(int fd, char* buf);
int light_r_adc0(int fd);


#endif /*__lightsense_h_*/
