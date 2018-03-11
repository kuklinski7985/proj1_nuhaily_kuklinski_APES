//testing for server messages

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>

#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080

int remote_socket_init();
int main(int argc, char const *argv[])
{
  struct sockaddr_in addr;
  struct sockaddr_in sock_addr;

  int opt = 1;
  
  int addrlen = sizeof(addr);
  int sock_handle;
  int server_sock;
  char rec_mess[32] = {0};

  char * test_message = "SERVER: Steady and relentless.";

  sock_handle = socket(AF_INET, SOCK_STREAM, 0);

  setsockopt(sock_handle, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(PORT);

  bind(sock_handle, (struct sockaddr*)&addr,sizeof(addr));

  listen(sock_handle,3);

  do
    {
      server_sock = accept(sock_handle, (struct sockaddr*) &addr, (socklen_t*)&addrlen);
      read(server_sock,rec_mess,32);
      printf("Message from Client: %s\n",rec_mess);
      send(server_sock, test_message, strlen(test_message),0);
      printf("Server message sent\n");
    }while(1);

  return 0;
}
