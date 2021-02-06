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
/*string CheckForCommands(char buf[], string commands[]){
  string buffer = string(buf);
  for(int i = 0; i < sizeof(commands); i++){
    if(buffer.find(commands[i]) != string::npos){
      //foud command
      if(commands[i] == "TEXT"){
        if(buffer.find("TCP") != string::npos){
          //Found both TEXT and TCP
          return "OK\n";
        }
      }
      else{
        return commands[i];
      }
    }
  }
}*/

int main(int argc, char *argv[]){

  //initCalcLib();
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

  //char *commands[8] = {"add", "sub", "mul", "div", "fadd", "fsub", "fmul", "fdiv"};
  //while(1){
    memset(buf, 0, sizeof(buf));
    int bytesRecived = recv(sock, &buf, sizeof(buf), 0);
    printf("%s", buf);
    if(string(buf).find("TEXT TCP") != string::npos){
      int sendRes = send(sock, "OK\n", sizeof("OK\n"), 0);
      if(sendRes == -1){
        printf("Could not send\n");
        return 1;
      }
      printf("OK\n");
    }
    else{
      //Close
      return 0;
    }

    memset(buf, 0, sizeof(buf));
    bytesRecived = recv(sock, buf, sizeof(buf), 0);
    printf("%s", buf);
    char *calc = strtok(buf, " "); 
    char *num1 = strtok(NULL, " ");
    char *num2 = strtok(NULL, " ");
    int i1, i2, iresult;
    double f1, f2, fresult;

    //printf("Operatoin:%s\n", calc);

    if(string(calc).at(0)!='f'){
      //printf("int value\n");
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
      printf("%d\n",iresult);
      
      int sendRes = send(sock, &iresult, sizeof(iresult), 0);
      if(sendRes == -1){
        printf("Could not send result\n");
        return 1;
      }
      
    }
    else{
      //printf("float value\n");
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
      printf("%8.8g\n",fresult);
      int sendRes = send(sock, &fresult, sizeof(fresult)+1, 0);
      if(sendRes == -1){
        printf("Could not send result\n");
        return 1;
      }
      
    }
    //Får inte sista sveret? Kan vara att jag skickar fel datatyp som svar?
    memset(buf, 0, sizeof(buf));
    bytesRecived = recv(sock, buf, sizeof(buf), 0);
    printf("%s\n", buf);
    
    
    
 
  //}
  
  //printf("%sBytes recived: %d\n", buf, bytesRecived);

  //Check the protcols

  /*int sendRes = send(sock, "OK\n", 3, sizeof("OK\n"));
  if(sendRes == -1){
    printf("Could not send\n");
    return 1;
  }
  printf("OK\n");

  memset(buf, 0, sizeof(buf));
  bytesRecived = recv(sock, buf, sizeof(buf), 0);
  //printf("%sBytes recived: %d\n", buf, bytesRecived);
  //printf("")*/


  
  close(sock);

  return 0;
}
