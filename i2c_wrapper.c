/**
* @file i2c_wrapper.c
* @brief wrapper functions definitions for the the i2c read and write commands.
* mutex functionality
* @author Andrew Kuklinski
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

#include "i2c_wrapper.h"

extern int tempsensor;
int i2c_read(int fd, char* buff, size_t count)
{
  int testing;
  testing = read(fd, buff, count);
  if(testing != count)
    {
      printf("error reading\n");
      return -1;
    }
  if(testing < 0)
    {
      printf("read failed\n");
      return -1;
    }
  
  printf("[0]: %d | [1]: %d\n",buff[0],buff[1]);

  return 0;
}

int i2c_write(int fd, char * buff, size_t count)
{
  int testing;
  testing = write(fd, buff, count);
  if(testing != count)
    {
      printf("error writting\n");
      return -1;
    }
  if(testing < 0)
    {
      printf("write failed\n");
      return -1;
    }
  
  printf("[0]: %d | [1]: %d\n",buff[0],buff[1]);
  
  return 0;

}

int i2c_init(char * filepath, int addr)
{
  if((tempsensor = open(filepath, O_RDWR)) < 0)
    {
      printf("counld not open i2c bus!\n");
      return -1;
    }

  if(ioctl(tempsensor, I2C_SLAVE, addr) < 0)
    {
      printf("Fail: bus access / talk to salve\n");
      return -1;
    }

  return tempsensor;
}
