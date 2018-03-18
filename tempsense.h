/**
* @file tempsense.h
* @brief fxn prototypes for operation of tmp102 temperature sensor
* @author Andrew Kuklinski and Adam Nuhaily
* @date 03/11/2018
**/


#ifndef tempsense_h_
#define tempsense_h_


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include "i2c_wrapper.h"

#define PTRREG_SD_MASK      0x01
#define PTRREG_TM_MASK      0x02
#define PTRREG_POL_MASK     0X04
#define PTRREG_F1F0_MASK(x) (x<<4)
#define PTRREG_R1R0_MASK(X) (x<<6)
#define PTRREG_OS_MASK      0x80

/**
 *@brief enum for register configuration
 *
 *@param "VOID"
 *
 *@return VOID
 */
typedef enum{
  TEMP_REG, CONFIG_REG, TLOW_REG, THIGH_REG
}ptr_reg;

/**
 *@brief writes to pointer register
 *
 *@param file pointer and which register you want to access
 *
 *@return VOID
 */
void w_ptr_reg(int fd, ptr_reg ptrreg);

/**
 *@brief read from Tlow register
 *
 *@param file pointer and location to store information
 *
 *@return VOID
 */
void r_tlow_reg(int fd, char * buff);  //complete

/**
 *@brief write to Tlow register
 *
 *@param file pointer and location of infromation to send
 *
 *@return VOID
 */
void w_tlow_reg(int fd, char * buff);  

/**
 *@brief read from Thigh register
 *
 *@param file pointer and location to store information
 *
 *@return VOID
 */
void r_thigh_reg(int fd, char * buff);

/**
 *@brief write to Thighregister
 *
 *@param file pointer and location of infromation to send
 *
 *@return VOID
 */
void w_thigh_reg(int fd, char * buff);  

/**
 *@brief read temp data register
 *
 *@param file pointer and location to store the data
 *
 *@return VOID
 */
void r_temp_reg(int fd, char * buff);  

/**
 *@brief write to configuration register
 *
 *@param file pointer and location to store the data
 *
 *@return VOID
 */

void w_config_reg(int fd, char * buff); 

/**
 *@brief read the values of the configuration register
 *
 *@param file pointer
 *
 *@return VOID
 */
int r_config_reg(int fd);  

/**
 *@brief makes raw data from sensor into readable format, in celcius
 *
 *@param location to store the data
 *
 *@return VOID
 */
float display_c(char * buff); 

/**
 *@brief makes raw data from sensor into readable format, in fahrenheit
 *
 *@param location to store the data
 *
 *@return VOID
 */
float display_f(char * buff);  

/**
 *@brief makes raw data from sensor into readable format, in kelvin
 *
 *@param location to store the data
 *
 *@return VOID
 */
float display_k(char * buff); 

/**
 *@brief makes the temp sensor able to read negitive numbers
 *
 *@param location to store the data
 *
 *@return VOID
 */
int detect_twos(int in);

uint16_t convert_twos(uint16_t in);

#endif /*__tempsense_h_*/
