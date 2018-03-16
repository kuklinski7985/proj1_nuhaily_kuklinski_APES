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

#define POWER_ON        0x03
#define POWER_OFF       0x00
#define CMD             0x80
#define GAIN_HIGH       0x01
#define GAIN_LOW        0x00
#define INTEG_13MS      0x00
#define INTEG_101MS     0x01
#define INTEG_402MS     0x10
#define INTR_DISABLE    0x00
#define INTR_ENABLE     0x01

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
void light_w_pwr(int fd, int state);
void light_r_pwr(int fd, char* readbuf);

void light_r_adc(int fd, int adc_sel, char* readbuf);
void light_w_timing_reg(int fd, int integ_tm, int gain, char* readbuf);
void light_w_intr_ctrl(int fd, int en, char per, char* readbuf);
void light_w_intr_thresh_low(int fd, char low_byte, char high_byte);
void light_w_intr_thresh_high(int fd, char low_byte, char high_byte)

#endif /*__lightsense_h_*/
