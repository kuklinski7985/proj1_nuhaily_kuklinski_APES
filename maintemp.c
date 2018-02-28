/**
* @file maintemp.c
* @brief main fxn for testing temperature sensor
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
#include "i2c_wrapper.h"
#include "tempsense.h"


int tempsensor;          //used for return value for open(), file indicator

int temp_addr = 0x48;       //slave address for the temp sensor
char readinfo[2] = {0};    //array for reading and sending data to sensor
char writeinfo[2] = {0};
char * tempsense_path = "/dev/i2c-2";

int main()
{
  /*tempsensor = i2c_init(tempsense_path, temp_addr);

  for(int i=0; i<5; i++)
    {
      r_temp_reg(tempsensor,readinfo);
      display_c(readinfo);
      display_f(readinfo);
      display_k(readinfo);
      printf("\n");
      sleep (2);
    }

  set_sd_mode(tempsensor);
  
  for(int i=0; i<5; i++)
    {
      r_temp_reg(tempsensor,readinfo);
      display_c(readinfo);
      display_f(readinfo);
      display_k(readinfo);
      printf("\n");
      sleep (2);
    }

  for(int j=0; j<3; j++)
    {
      resolution_setting(tempsensor,j);
    for(int i=0; i<5; i++)
    {
      r_temp_reg(tempsensor,readinfo);
      display_c(readinfo);
      display_f(readinfo);
      display_k(readinfo);
      printf("\n");
      sleep (2);
    }
    }

    set_TM(tempsensor);*/

}

  //writes to the pointer register to access the temp register
  //w_ptr_reg(tempsensor,TEMP_REG);

  //sleep(1);

  //r_current_config_reg(tempsensor);

  //set_sd_mode(tempsensor);
  //sleep(2);

  //set_sd_mode(tempsensor);
 
  /*int temp_final = 0;
  float celsius, fahr;
  
  while(1)
    {
      i2c_read(tempsensor, readinfo, 2);
      
      //combine MSB LSB, shift b/c 12 bit resolution
      temp_final = ((readinfo[0] << 8) | readinfo[1]) >> 4;

      celsius = temp_final * 0.0625;
      fahr = (1.8 * celsius) + 32;

      printf("F: %04f | C: %04f\n", fahr, celsius);

      sleep(1);
      }*/
  
  

