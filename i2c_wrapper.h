/**
* @file i2c_wrapper.h
* @brief wrapper prototypes for the the i2c read and write commands. includes
* mutex functionality
* @author Andrew Kuklinski
* @date 03/11/2018
**/


#ifndef i2c_wrapper_h_
#define i2c_wrapper_h_

int i2c_read(int fd, char* buff, size_t count);

int i2c_write(int fd, void *buf, size_t count);

int i2c_init(char * filepath, int addr);
#endif /*__i2c_wrapper_h_*/
