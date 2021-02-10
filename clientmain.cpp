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
  if(argc != 2){
    printf("Invalid input\n");
    exit(1);
  }
  char delim[]=":";
  char *Desthost=strtok(argv[1],delim);
  char *Destport=strtok(NULL,delim);
  // *Desthost now points to a sting holding whatever came before the delimiter, ':'.
  // *Dstport points to whatever string came after the delimiter. 

  //Convert port to int and host to string
  int port = atoi(Destport);
  string ipAddr = Desthost;
  //make socket
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock == -1){
    printf("Error could not make socket\n");
    exit(1);
  }
  
  
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
  int bytesRecived = recv(sock, &buf, sizeof(buf), 0);
  printf("%s", buf);

  string ok = "OK\n";
  
  
  if(string(buf).find("TEXT TCP") != string::npos){
    int sendRes = send(sock, ok.c_str(), ok.length(), 0);
    if(sendRes == -1){
      printf("Could not send\n");
      exit(1);
    }
    printf("OK\n");
  }
  else{
    printf("Protocols not supported.\n");
    close(sock);
    exit(1);
  }

  memset(buf, 0, sizeof(buf));
  bytesRecived = recv(sock, buf, sizeof(buf), 0);
  printf("%s", buf);
  char *calc = strtok(buf, " "); 
  char *num1 = strtok(NULL, " ");
  char *num2 = strtok(NULL, " ");
  int i1 = 0, i2 = 0, iresult = 0;
  float f1 = 0, f2 = 0, fresult = 0;
  string result = "";

  //Calculate result
  //Int
  if(string(calc).at(0)!='f'){
    i1 = stoi(num1);
    i2 = stoi(num2);
    if(string(calc) == "add"){
      iresult = i1+i2;
    }
    else if(string(calc)== "sub"){
      iresult = i1-i2;
    }
    else if(string(calc) == "mul"){
      iresult = i1*i2;
    }
    else if(string(calc) == "div"){
      iresult = i1/i2;
    }
      //printf("%d\n",iresult);
      result = to_string(iresult);
      
    }
    //floating point
    else{
      f1 = stod(num1);
      f2 = stod(num2);
      if(string(calc) == "fadd"){
        fresult = f1+f2;
      }
      else if(string(calc) == "fsub"){
        fresult = f1-f2;
      }
      else if(string(calc) == "fmul"){
        fresult = f1*f2;
      }
      else if(string(calc)== "fdiv"){
        fresult = f1/f2;
      }
      //printf("%8.8g\n",fresult);
      result = to_string(fresult);
      
    }
    result += '\n';
    
    //Send result
    
    int sendRes = send(sock, result.c_str(), result.length(), 0);
    if(sendRes == -1){
      printf("Could not send result\n");
      return 1;
    }
    printf("%s", result.c_str());
    //Get status (OK or ERROR)
    memset(buf, 0, sizeof(buf));
    bytesRecived = recv(sock, buf, sizeof(buf), 0);
    printf("%s\n", buf);
    
    
    
 




  
  close(sock);

  return 0;
}
