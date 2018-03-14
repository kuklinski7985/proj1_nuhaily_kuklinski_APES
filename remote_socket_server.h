/**
* @file remote_socket_server.h
* @brief fxns prototypes for remote socket initialization
* @author Andrew Kuklinski and Adam Nuhaily
* @date 03/11/2018
**/

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>

#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>



#ifndef remote_socket_server_h_
#define remote_socket_server_h_

/**
 *@brief initializes remote socket and keeps it open for data transfers
 *
 *@param "VOID" nothing
 *
 *@return VOID
 */

int remote_socket_server_init();

#endif /*__remote_socket_h_*/




