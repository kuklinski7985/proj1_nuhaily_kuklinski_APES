/**
* @file tempsense.h
* @brief fxn prototypes for operation of tmp102 temperature sensor
* @author Andrew Kuklinski and Adam Nuhaily
* @date 03/11/2018
**/


#ifndef tempsense_h_
#define tempsense_h_

#define PTRREG_SD_MASK      0x01
#define PTRREG_TM_MASK      0x02
#define PTRREG_POL_MASK     0X04
#define PTRREG_F1F0_MASK(x) (x<<4)
#define PTRREG_R1R0_MASK(X) (x<<6)
#define PTRREG_OS_MASK      0x80

typedef enum{
  TEMP_REG, CONFIG_REG, TLOW_REG, THIGH_REG
}ptr_reg;

void w_ptr_reg(int fd, ptr_reg ptrreg);  //complete

void r_tlow_reg(int fd, char * buff);  //complete

void w_tlow_reg(int fd, char * buff);  //complete

void r_thigh_reg(int fd, char * buff);  //complete

void w_thigh_reg(int fd, char * buff);  //complete

void r_temp_reg(int fd, char * buff);  //complete

void w_config_reg(int fd, char * buff);  //complete

int r_config_reg(int fd);  //complete

float display_c(char * buff);  //complete

float display_f(char * buff);  //complete

float display_k(char * buff);  //complete


#endif /*__tempsense_h_*/
