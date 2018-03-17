/**
* @file remote_socket_server.c
* @brief fxn definitions to create a remote socket which can access temp and light data
* @author Andrew Kuklinski and Adam Nuhaily
* @date 03/11/2018
**/

#include "remote_socket_server.h"

/*struct for data to be sent to client*/
typedef struct teststruct{
  char test_message[64];
  int testvalue;
}teststruct;

#define MYPORT 8080

int remote_socket_server_init()
{
  struct sockaddr_in addr;
  //struct sockaddr_in sock_addr;

  int opt = 1;
  
  int addrlen = sizeof(addr);
  int sock_handle;
  int server_sock;
  char rec_mess[64] = {0};
  teststruct my_teststruct;
  strcpy(my_teststruct.test_message, "testes 1 2 3 ?");
  my_teststruct.testvalue = 42890;

  //char * test_message = "Message Received successfully from client";
  

  printf("start of server\n");
  
  sock_handle = socket(AF_INET, SOCK_STREAM, 0);
  if(sock_handle == 0)
    {
      printf("socket allocation failed\n");
      return -1;
    }

  if(setsockopt(sock_handle, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
      printf("error with setsockopt\n");
      return -1;
    }

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(MYPORT);

  if(bind(sock_handle, (struct sockaddr*)&addr,sizeof(addr)))
    {
      printf("error on bind\n");
      return -1;
    }

  printf("Listening mode for server\n");
  while(1)
    {
      listen(sock_handle,5);

      server_sock = accept(sock_handle, (struct sockaddr*) &addr, &addrlen);
      if(server_sock < 0)
	{
	  printf("Accept failure\n");
	  return -1;
	}

      
      printf("Accept complete\n");
      read(server_sock,rec_mess,64);
      printf("Message from Client: %s\n",rec_mess);
      //send(server_sock, test_message, strlen(test_message),0);
      send(server_sock, &my_teststruct, sizeof(my_teststruct),0);
      printf("Server message sent to client\n");
    }
    
    while(bizzounce == 0);

  return 0;
}
