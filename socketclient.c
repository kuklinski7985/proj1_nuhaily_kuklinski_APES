/**
* @file socketclient.c
* @brief fxn external program that creates a socket in order to obtain sensor information, client side
* @author Andrew Kuklinski and Adam Nuhaily
* @date 03/11/2018
**/


#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <stdint.h>
#include <netinet/in.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <stdarg.h>
#include <ctype.h>
#include <mqueue.h>


#include "ipc_messq.h"
#include "temp_ops.h"
#include "logger/logger.h"
#include "socketclient.h"

#define PORT 8090

/*typedef struct teststruct{
  char test_message[64];
  int testvalue;
}teststruct;*/

int main(int argc, char const *argv[])
{

  ipcmessage_t light_message;
  char msg_str[256];
  ipcmessage_t * temp_message;
  ipcmessage_t * main_message;

  ipcmessage_t * received_message;

int option;
printf("Enter1: Light, 2:Temp, 3:Terminate\n");
option = getchar();

  //struct sockaddr_in addr;
  struct sockaddr_in sock_addr;    //struct that contains the port and Ip addr
  int sock_handle;                 //assigned socket tracking value
  //char rec_mess[64] = {0};         //storing received message
  //teststruct rec_mess;

  //char * test_message = "Be like water.";

  sock_handle = socket(AF_INET, SOCK_STREAM, 0);
  if(sock_handle < 0)
    {
      printf("Socket not created\n");
      return -1;
    }

  sock_addr.sin_family = AF_INET;
  sock_addr.sin_port = htons(PORT);

  if(inet_pton(AF_INET, "127.0.0.1", &sock_addr.sin_addr)<=0)
    {
      printf("Invalid address\n");
      return -1;
    }
  
  if(connect(sock_handle, (struct sockaddr*)&sock_addr, sizeof(sock_addr))<0)
    {
      printf("connection to server FAILED!\n");
      return -1;
    }



switch(option)
{
  case '1':
    strcpy(light_message.timestamp, getCurrentTimeStr());
    light_message.type = QUERY;
    light_message.source = IPC_SOCKET;
    light_message.destination = IPC_LIGHT;
    light_message.src_pid = getpid();
    light_message.payload[256] = '7';
    light_message.units_temp = UNITS_K;
    build_ipc_msg(light_message, msg_str);
    printf("%s",msg_str);
    send(sock_handle,msg_str, strlen(msg_str),0);
    printf("case1\n");
    break;
  case 2:
    strcpy(temp_message->timestamp, getCurrentTimeStr());
    temp_message->type = QUERY;
    temp_message->source = IPC_SOCKET;
    temp_message->destination = IPC_TEMP;
    temp_message->units_temp = UNITS_K;
    send(sock_handle, temp_message, sizeof(temp_message),0);
    break;
  case 3:
    strcpy(temp_message->timestamp, getCurrentTimeStr());
    main_message->type = TERMINATE;
    main_message->source = IPC_SOCKET;
    main_message->destination = IPC_MAIN;
    send(sock_handle, main_message, sizeof(main_message),0);
    break;
  default:
    printf("Option not valid, please rerun\n");
    break;

}

  printf("Client message sent\n");
  //int valread = read(sock_handle, rec_mess, 64);
  //int valread = read(sock_handle, &received_message, sizeof(received_message));
  //printf("received value: %s\n", received_message->payload);

  return 0;
}
