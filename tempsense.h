/**
* @file tempsense.h
* @brief fxn prototypes for operation of tmp102 temperature sensor
* @author Andrew Kuklinski and Adam Nuhaily
* @date 03/11/2018
**/


#ifndef tempsense_h_
#define tempsense_h_

typedef enum{
  TEMP_REG, CONFIG_REG, TLOW_REG, THIGH_REG
}ptr_reg;


/*used to change which registers to point to according to the following
 *0x00: temp reg
 *0x01: configuration reg
 *0x02: T-low reg
 *0x03: T-high reg*/
void w_ptr_reg(int fd, ptr_reg ptrreg);

void r_tlow_reg(int fd, char * buff);

void w_tlow_reg(int fd, char * buff, ptr_reg ptrreg);

void r_thigh_reg(int fd, char * buff);

void w_thigh_reg(int fd, char * buff, ptr_reg ptrreg);

void r_temp_reg(int fd, char * buff);

void set_sd_mode(int value);

int r_current_config_reg();


#endif /*__tempsense_h_*/
