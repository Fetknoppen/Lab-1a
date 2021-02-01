#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/* You will to add includes here */
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
// Enable if you want debugging to be printed, see examble below.
// Alternative, pass 
#define DEBUG


// Included to get the support library
//#include <calcLib.h>

int main(int argc, char *argv[]){

  /*
    Read first input, assumes <ip>:<port> syntax, convert into one string (Desthost) and one integer (port). 
     Atm, works only on dotted notation, i.e. IPv4 and DNS. IPv6 does not work if its using ':'. 
  */
  char delim[]=":";
  char *Desthost=strtok(argv[1],delim);
  char *Destport=strtok(NULL,delim);
  // *Desthost now points to a sting holding whatever came before the delimiter, ':'.
  // *Dstport points to whatever string came after the delimiter. 

  /* Do magic */
  // make a socket
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock == -1){
    return 1; //could not make socket
  }


  int port=atoi(Destport);
  std::string ip = std::string(Desthost);
  

  sockaddr_in hint;
  hint.sin_family = AF_INET;
  hint.sin_port = htons(port);
  //convert the string ip to an ip adress
  inet_pton(AF_INET, ip.c_str(), &hint.sin_addr);

  #ifdef DEBUG 
    printf("Host %s, and port %d.\n",Desthost,port);
  #endif

  //connect to server
  if(connect(sock, (sockaddr*)&hint, sizeof(hint))==-1){
    return 1; // could not connect to server
  }

  char buf[4096];
  std::string userInput;

  //Change order here, the server talks first
  //Talk to server
  do{
    //Enter lines of text
    std::cout<<"< ";
    getline(std::cin, userInput);
    
    //Send to server
    if(send(sock, userInput.c_str(), userInput.size()+1, 0)==-1){
      std::cout<<"Could not send to server\n";
      continue;
    }

    //Wait for response
    memset(buf, 0, 4096);
    int bytesReceived = recv(sock, buf, 4096, 0);

    //Desplay response
    std::cout<<"SERVER>"<<std::string(buf, bytesReceived)<<"\n";
    


  }while(true);

  close(sock);

  return 0;
}
