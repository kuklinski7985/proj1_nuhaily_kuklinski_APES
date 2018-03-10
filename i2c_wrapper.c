/**
* @file i2c_wrapper.c
* @brief wrapper functions definitions for the the i2c read and write commands.
* mutex functionality
* @author Andrew Kuklinski
* @date 03/11/2018
**/

#include "i2c_wrapper.h"

extern int i2ctarget;

int i2c_read(int fd, char* buff, size_t count)
{
  int ret;
  ret = read(fd, buff, count);
  if(ret != count)
    {
      printf("I2C read error. Requested bytes = %d, returned = %d\n", count, \
        ret);
      return -1;
    }
  if(ret < 0)
    {
      printf("read failed\n");
      return -1;
    }

  //printf("[0]: %d | [1]: %d\n",buff[0],buff[1]);

  return 0;
}

int i2c_write(int fd, char * buff, size_t count)
{
  int ret;
  ret = write(fd, buff, count);
  if(ret != count)
    {
      printf("I2C write error. Requested bytes = %d, returned = %d\n", count, \
        ret);
      return -1;
    }
  if(ret < 0)
    {
      printf("write failed\n");
      return -1;
    }

  //printf("[0]: %d | [1]: %d\n",buff[0],buff[1]);

  return 0;

}

int i2c_init(char * filepath, int addr)
{
  if((i2ctarget = open(filepath, O_RDWR)) < 0)
    {
      printf("Could not open I2C bus!\n");
      return -1;
    }

  if(ioctl(i2ctarget, I2C_SLAVE, addr) < 0)
    {
      printf("Fail: bus access / talk to slave\n");
      return -1;
    }

  printf("I2C init complete.\n");
  return i2ctarget;
}
