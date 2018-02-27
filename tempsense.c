/**
* @file tempsense.c
* @brief fxn definitions for temperature sensor
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
#include <fcntl.h>

#include "tempsense.h"
#include "i2c_wrapper.h"


void w_ptr_reg(int fd, ptr_reg ptrreg)
{
  char reg_pick[1] = {ptrreg};
  i2c_write(fd,reg_pick,1);
  }

void r_tlow_reg(int fd, char * buff)
{
  char reg_pick[1] = {TLOW_REG};
  i2c_write(fd,reg_pick,1);
  
  i2c_read(fd, buff,2);

}

void r_temp_reg(int fd, char * buff)
{
  i2c_read(fd,buff,2);
}


void set_sd_mode(inf fd, int value)
{
  char reg_pick[1] = {CONFIG_REG};
  i2c_write(fd, reg_pick, 1);

  
}

int r_current_config_reg()
{
  char reg_pick[1] = {CONFIG_REG};
  i2c_write(fd, reg_pick, 1);

  //i2c_read
  return;
