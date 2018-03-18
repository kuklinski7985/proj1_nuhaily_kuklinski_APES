/**
* @file remote_socket_server.c
* @brief fxn definitions to create a remote socket which can access temp and light data
* @author Andrew Kuklinski and Adam Nuhaily
* @date 03/11/2018
**/

#include "remote_socket_server.h"

/*struct for data to be sent to client*/
/*typedef struct teststruct{
  char test_message[64];
  int testvalue;
}teststruct;*/

#define MYPORT 8090

extern int bizzounce;
extern char * temp_previous;
extern char * light_global;
extern temp_unit_t temp_unit_sel;

int socket_read_flag = 0;

void *remote_socket_server_init()
{
  struct sockaddr_in addr;

  int opt = 1;
  
  int addrlen = sizeof(addr);
  int sock_handle;
  int server_sock;

  ipcmessage_t * socket_msg_in;
  char msg_in[256];
  ipcmessage_t * socket_msg_out;
  char msg_out[256];
  int readval;

  //char rec_mess[64] = {0};
  //teststruct my_teststruct;
  //strcpy(my_teststruct.test_message, "testes 1 2 3 ?");
  //my_teststruct.testvalue = 42890;

  //char * test_message = "Message Received successfully from client";
  

  printf("start of server\n");
  
  if((sock_handle = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
    printf("socket allocation failed\n");
  }


  if(setsockopt(sock_handle, SOL_SOCKET, (SO_REUSEADDR | SO_REUSEPORT), &opt, sizeof(opt)))
    {
      printf("error with setsockopt\n");
 
    }

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(MYPORT);

  if(bind(sock_handle, (struct sockaddr*)&addr,sizeof(addr))<0)
    {
      printf("error on bind\n");
       printf("bing errno %s\n", strerror(errno));
    }

  printf("Listening mode for server\n");
  while(bizzounce == 0)
    {
      listen(sock_handle,5);

      if((server_sock = accept(sock_handle, (struct sockaddr*) &addr, &addrlen)) <0)
      {
        printf("Accept failure\n");
      }

      printf("Message accepted from client\n");
      //read(server_sock,rec_mess,64);
      readval = read(server_sock, msg_in, strlen(msg_in));
      printf("%s\n", msg_in);
      //printf("Message from Client: %s\n",rec_mess);
      //send(server_sock, test_message, strlen(test_message),0);
      decipher_ipc_msg(msg_in, socket_msg_in);
      
      printf("test here\n");
      
      



      /************getting info from global variables**********/
      /*strcpy(socket_msg_out->timestamp, getCurrentTimeStr());
      socket_msg_out->type = DATA;
      socket_msg_out->source = IPC_SOCKET;
      switch(socket_msg_in->destination)
      {
        case(IPC_LIGHT):
          //socket_msg_out->payload = light_global;
          //sprintf(socket_msg_out->payload, "%s",light_global);
          socket_msg_out->source = IPC_LIGHT;
          break;
        case(IPC_TEMP):
          socket_read_flag = 1;        //able to read the global temp sensor

          if(socket_msg_in->units_temp == UNITS_C)
          {
            temp_unit_sel = UNITS_C;
          }
          if(socket_msg_in->units_temp == UNITS_F)
          {
            temp_unit_sel = UNITS_F;
          }
          if(socket_msg_in->units_temp == UNITS_K)
          {
            temp_unit_sel = UNITS_K;
          }
          else
          {
            temp_unit_sel = UNITS_NONE;
          }

          //socket_msg_out->payload = temp_previous;
          strcpy(socket_msg_out->payload, temp_previous);
          socket_msg_out->source = IPC_TEMP;
          socket_read_flag = 0;
          break;
        case(IPC_MAIN):
          bizzounce = 1;
          break;
      }*/

      
      
      //send(server_sock, &socket_msg_out, sizeof(socket_msg_out),0);
      printf("Server message sent to client\n");
    }

}
