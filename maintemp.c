#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int tempsensor;          //used for return value for open()

int temp_addr = 0x48;       //slave address for the temp sensor

int main()
{
  int testing =0;           //verify read and write operations
  char * tempsense_path = "/dev/i2c-2";
  if((tempsensor = open(tempsense_path, O_RDWR)) < 0)
    {
      printf("counld not open i2c bus!\n");
      return -1;
    }

  if(ioctl(tempsensor, I2C_SLAVE, temp_addr) < 0)
    {
      printf("Fail: bus access / talk to salve\n");
      return -1;
    }

  /*rw_tempsensor * info;
  info = (rw_tempsensor*)malloc((sizeof)rw_tempsensor);
  if(info == NULL)
    {
      printf("malloc failed\n");
      return -1;
      }*/

  /*if(write(tempsense_path, info,3) != 3)
    {
      printf("writting data to sensor failed\n");
      return -1;
      }*/

  char tempbuff[1] = {0x00};
  write(tempsensor,tempbuff, 1);

  sleep(1);
  char info[2] = {0};    //array for reading and sending data to sensor

  int temp_final = 0;
  float celsius, fahr;
  
  while(1)
    {
      testing = read(tempsensor, info, 2);
      if(testing != 2)
	{
	  printf("error reading\n");
	}
      if(testing < 0)
	{
	  printf("read failed\n");
	  return -1;
	}
      printf("[0]: %d | [1]: %d\n",info[0],info[1]);
      
      //combine MSB LSB, shift b/c 12 bit resolution
      temp_final = ((info[0] << 8) | info[1]) >> 4;

      celsius = temp_final * 0.0625;
      fahr = (1.8 * celsius) + 32;

      printf("F: %04f | C: %04f\n", fahr, celsius);

      sleep(1);
      }
  
  
}
