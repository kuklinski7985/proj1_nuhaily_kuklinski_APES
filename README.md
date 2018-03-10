# proj1_nuhaily_kuklinski_APES
project 1 code for ECEN5013-APES

File breakdown for Project 1 - Adavnced Practical Embedded System Programming

i2c_wrapper.c/.h : these files contain functions to wrap the i2c read and write functions for the BBG.

myusrled.c/.h : function to interact with the leds on the BBG board.  Can manipulate USRLED 1,2, and 3.

temp_ops.c/.h : functions that are associated with the threads that are created in main and POSIX timers for data aquisition.  These timers
                could also be used for required heartbeat of sensors
                
tempsense.c/.h : contain functions to interact with the TMP102 temperature sensor.  The sensor is connected to the I2C-2 bus, 



BBG Pinout:

Pin #	Pin Name	Connection
1	DGND		TMP102 DGND
3	VDD_3V3		TMP102 VDD
19	I2C2_SCL	TMP102 SCL
20	I2C2_SDA	TMP102 SDA
