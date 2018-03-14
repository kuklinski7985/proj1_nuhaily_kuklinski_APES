
//client to request data from the sensors

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080

typedef struct teststruct{
  char test_message[64];
  int testvalue;
}teststruct;

int main(int argc, char const *argv[])
{
  //struct sockaddr_in addr;
  struct sockaddr_in sock_addr;    //struct that contains the port and Ip addr
  int sock_handle;                 //assigned socket tracking value
  //char rec_mess[64] = {0};         //storing received message
  teststruct rec_mess;

  char * test_message = "Be like water.";

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
  send(sock_handle, test_message, strlen(test_message),0);
  printf("Client message sent\n");
  //int valread = read(sock_handle, rec_mess, 64);
  int valread = read(sock_handle, &rec_mess, sizeof(rec_mess));
  printf("received message: %s\n", rec_mess.test_message);
  printf("received value: %d\n", rec_mess.testvalue);

  return 0;
}
