/**
* @file lightsense.c
* @brief fxn definitions for temperature sensor
* @author Andrew Kuklinski and Adam Nuhaily
* @date 03/11/2018
**/

#include "lightsense.h"

void light_r_id_reg(int fd, char* buf)
{
  char id_reg[1];
  char readbuf[1];
  id_reg[0] = CMD | ID;

  i2c_write(fd, id_reg, 1);
  i2c_read(fd, readbuf, 1);

  strcpy(buf, readbuf);
}

void light_w_pwr(int fd, int state)
{
  char ctrl[2];
  ctrl[0] = CMD | CONTROL;

  if(state == 1)
  {
    ctrl[1] = POWER_ON;
  }
  else
  {
    ctrl[1] = POWER_OFF;
  }

  i2c_write(fd, ctrl, 2);
}

void light_r_pwr(int fd, char* readbuf) // must be run immediately after write to power reg for it to work
{
  char ctrl[1];
  i2c_read(fd, readbuf, 1);
}

void light_r_adc(int fd, int adc_sel, char* readbuf)
{
  char ctrl1[1];
  char ctrl2[1];
  char temp[1];
  ctrl1[0] = CMD;
  ctrl2[0] = CMD;

  if(adc_sel == 0)
  {
    ctrl1[0] |= DATA0_L;
    ctrl2[0] |= DATA0_H;
  }
  else
  {
    ctrl1[0] |= DATA1_L;
    ctrl2[0] |= DATA1_H;
  }

  i2c_write(fd, ctrl1, 1);
  i2c_read(fd, temp, 1);
  readbuf[0] = temp[0];
  i2c_write(fd, ctrl2, 1);
  i2c_read(fd, temp, 1);
  readbuf[1] = temp[0];
}

void light_w_timing_reg(int fd, int integ_tm, int gain, char* readbuf)
{
  char ctrl[2] = {CMD | TIMING, integ_tm | gain};
  i2c_write(fd, ctrl, 2);
  i2c_read(fd, readbuf, 2);
//  printf("Timing reg response: %x %x\n", readbuf[0], readbuf[1]);
    // having trouble deciphering the output of this guy
}

void light_w_intr_ctrl(int fd, int en, char per, char* readbuf)
{
  char ctrl[2];

  ctrl[0] = CMD | INTERRUPT;
  ctrl[1] = en | per;
  i2c_write(fd, ctrl, 2);
  i2c_read(fd, readbuf, 2);
}

void light_w_intr_thresh_low(int fd, char low_byte, char high_byte)
{
  char ctrl[2] = {CMD | TLOW_L, low_byte};
  i2c_write(fd, ctrl, 2);
  ctrl[0] = CMD | TLOW_H;
  ctrl[1] = high_byte;
  i2c_write(fd, ctrl, 2);
}

void light_w_intr_thresh_high(int fd, char low_byte, char high_byte)
{
  char ctrl[2] = {CMD | THIGH_L, low_byte};
  i2c_write(fd, ctrl, 2);
  ctrl[0] = CMD | THIGH_H;
  ctrl[1] = high_byte;
  i2c_write(fd, ctrl, 2);
}
