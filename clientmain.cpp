#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/* You will to add includes here */
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <iostream>
// Enable if you want debugging to be printed, see examble below.
// Alternative, pass 
#define DEBUG


// Included to get the support library
//#include <calcLib.h>

using namespace  std;


int main(int argc, char *argv[]){

  /*
    Read first input, assumes <ip>:<port> syntax, convert into one string (Desthost) and one integer (port). 
     Atm, works only on dotted notation, i.e. IPv4 and DNS. IPv6 does not work if its using ':'. 
  */
  char delim[]=":";
  char *Desthost=strtok(argv[1],delim);
  char *Destport=strtok(NULL,delim);
  //string ipAddr = argv[0];
  //int port = atoi(argv[1]);
  // *Desthost now points to a sting holding whatever came before the delimiter, ':'.
  // *Dstport points to whatever string came after the delimiter. 

  /* Do magic */
  //make socket
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock == -1){
    printf("Error could not make socket\n");
    return 1;
  }
  int port = atoi(Destport);
  string ipAddr = Desthost;
  
  struct sockaddr_in hint;
  hint.sin_family = AF_INET;
  hint.sin_port = htons(port);
  inet_pton(AF_INET, ipAddr.c_str(), &hint.sin_addr);

  //koppla till servern
  int connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));
  if(connectRes == -1){
    printf("Error could not connect\n");
    return 1;
  }

  //prata med servern
  char buf[10000];

  memset(buf, 0, sizeof(buf));
  int bytesRecived = recv(sock, buf, sizeof(buf), 0);
  printf("%s\n%d", buf, bytesRecived);

  //Check the protcols

  int sendRes = send(sock, "OK\n", 3, sizeof("OK\n"));
  if(sendRes == -1){
    printf()
  }
  printf("OK\n");



  
  close(sock);

  return 0;
}
