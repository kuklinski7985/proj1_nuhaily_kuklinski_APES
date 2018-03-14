/**
* @file lightsense.c
* @brief fxn definitions for temperature sensor
* @author Andrew Kuklinski and Adam Nuhaily
* @date 03/11/2018
**/

#include "lightsense.h"

void light_r_id_reg(int fd, char* buf)
{
  char id_reg[1] = {ID};

  i2c_write(fd, id_reg, 1);
  i2c_read(fd, buf, 2);
  printf("Buffer contents: ");

  printf("%x", buf[0]);
  printf("%x", buf[1]);
  printf("\n");

}

int light_r_adc0(int fd)
{
  char ctrl[1] = {CONTROL_BYTE | CONTROL};
  char cmd1[1] = {DATA0_H};
  char cmd2[1] = {DATA0_L};
  char i2c_readbuf[2];

  int val = 0;
  i2c_write(fd, ctrl, 1);
  //i2c_write(fd, {CONTROL_BYTE}, 1);
  i2c_read(fd, i2c_readbuf, 2);
  i2c_write(fd, cmd2, 1);
  i2c_read(fd, i2c_readbuf, 2);
  printf("Buffer contents: ");
  printf("%x", i2c_readbuf[0]);
  printf("%x\n", i2c_readbuf[1]);
  val |= ((int)i2c_readbuf[1] << 3 | (int)i2c_readbuf[0] << 2);
  i2c_write(fd, cmd1, 1);
  i2c_read(fd, i2c_readbuf, 2);
  printf("Buffer 2 contents: ");
  printf("%x", i2c_readbuf[0]);
  printf("%x", i2c_readbuf[1]);
  printf(" %s\n", i2c_readbuf);
/*
  i2c_write(fd, cmd2, 1);
  i2c_read(fd, i2c_readbuf, 2);
*/
  val |= ((int)i2c_readbuf[1] << 1 | (int)i2c_readbuf[0]);

  return val;

}
