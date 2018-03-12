/**
* @file remote_socket.c
* @brief fxn definitions to create a remote socket which can access temp and light data
* @author Andrew Kuklinski and Adam Nuhaily
* @date 03/11/2018
**/

#include "remote_socket.h"

//this code acts as a request server

int remote_socket_init()
{
  struct sockaddr_in addr;
  struct sockaddr_in sock_addr;

  int opt = 1;
  
  int addrlen = sizeof(addr);
  int sock_handle;
  int server_sock;
  char rec_mess[32] = {0};

  char * test_message = "SERVER: Message Received successfully from client";

  printf("start of server\n");
  
  sock_handle = socket(AF_INET, SOCK_STREAM, 0);
  if(sock_handle == 0)
    {
      printf("socket allocation failed\n");
      return -1;
    }

  setsockopt(sock_handle, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(PORT);

  bind(sock_handle, (struct sockaddr*)&addr,sizeof(addr));

  printf("bind complete\n");

      listen(sock_handle,3);
      printf("just after listen");
      server_sock = accept(sock_handle, (struct sockaddr*) &addr, (socklen_t*)&addrlen);
      printf("just after accept\n");
      read(server_sock,rec_mess,32);
      printf("Message from Client: %s\n",rec_mess);
      send(server_sock, test_message, strlen(test_message),0);
      printf("Server message sent\n");
    

  return 0;
}
