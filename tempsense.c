/**
* @file tempsense.c
* @brief fxn definitions for temperature sensor
* @author Andrew Kuklinski and Adam Nuhaily
* @date 03/11/2018
**/

#include "tempsense.h"

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
void w_tlow_reg(int fd, char * buff)
{
  char reg_pick[1] = {TLOW_REG};
  i2c_write(fd,reg_pick,1);

  i2c_write(fd,buff,2);

}

void r_thigh_reg(int fd, char * buff)
{
  char reg_pick[1] = {THIGH_REG};
  i2c_write(fd,reg_pick,1);

  i2c_read(fd, buff,2);

}
void w_thigh_reg(int fd, char * buff)
{
  char reg_pick[1] = {THIGH_REG};
  i2c_write(fd,reg_pick,1);

  i2c_write(fd,buff,2);

}

void r_temp_reg(int fd, char * buff)
{
  char reg_pick[1] = {TEMP_REG};
  i2c_write(fd, reg_pick, 1);
  i2c_read(fd,buff,2);
}


void w_config_reg(int fd, char * buff)
{
  char reg_pick[1] = {CONFIG_REG};
  i2c_write(fd, reg_pick, 1);

  char config_reg_val[1] = {0};
  i2c_read(fd,config_reg_val,1);
  printf("config_reg: %x\n",config_reg_val[0]);

  config_reg_val[0] |= buff[0];
  printf("config_reg: %x\n",config_reg_val[0]);

  //i2c_write(fd, reg_pick, 1);
  i2c_write(fd,config_reg_val,1);

}

/*int resolution_setting(int fd, int value)
{

  if (value > 3)
    {
      printf("value must be between 0 and 3\n");
      exit(1);
    }

  char reg_pick[1] = {CONFIG_REG};
  i2c_write(fd, reg_pick, 1);

  char config_reg_val[1] = {0};
  i2c_read(fd,config_reg_val,1);
  printf("config_reg: %x\n",config_reg_val[0]);

  switch(value){
     case 0:
       config_reg_val[0] &= ~(1<<7);
       config_reg_val[0] &= ~(1<<6);
       print("resolution 9 bits, 27.5ms\n");
       break;
     case 1:
       config_reg_val[0] |= (1<<6);
       print("resolution 10 bits, 55ms\n");
       break;
     case 2:
       config_reg_val[0] |= (1<<7);
       print("resolution 11 bits, 110ms\n");
       break;
     case 3:
       config_reg_val[0] |= ((1<<7) | (1<<6));
       print("resolution 12 bits, 220ms\n");
       break;
     default:
       printf("invalid entry\n");
  }

  printf("config_reg resolution: %x\n",config_reg_val[0]);

  i2c_write(fd,config_reg_val,1);

  return config_reg_val[0];

  }*/


int r_config_reg(int fd)
{
  char reg_pick[1] = {CONFIG_REG};
  i2c_write(fd, reg_pick, 1);

  char config_reg_val[1] = {0};
  i2c_read(fd,config_reg_val,1);

  printf("current configuration value: %x\n",config_reg_val[0]);

  return config_reg_val[0];
}

float display_c(char * buff)
{
  int temp_final;
  float celsius;
  temp_final = ((buff[0] << 8) | buff[1]) >> 4;
  celsius = temp_final * 0.0625;
  printf("degree-C: %04f\n",celsius);
  return celsius;
}

float display_f(char * buff)
{
  int temp_final;
  float farh;
  temp_final = ((buff[0] << 8) | buff[1]) >> 4;
  farh = (1.8*(temp_final * 0.0625) +32);
  printf("degree-F: %04f\n",farh);
  return farh;
}

float display_k(char * buff)
{
  int temp_final;
  float kelvin;
  temp_final = ((buff[0] << 8) | buff[1]) >> 4;
  kelvin = (temp_final * 0.0625) + 273.15;
  printf("degree-K: %04f\n",kelvin);
  return kelvin;
}
