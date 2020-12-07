// A simple server in the internet domain using TCP
// The port nu1mber is passed as an argument

// Please note this is a C program
// It compiles without warnings with gcc

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <math.h>      
#include <string>
#include <iostream>
#include <sys/wait.h>


using namespace std;


string decToBin(int num);
string decToEli(int num);
void fireman(int);

// void error(char *msg)
// {
//     perror(msg);
//     exit(1);
// }
void fireman(int)
{
    while(waitpid(-1,NULL, WNOHANG) > 0)
        std::cout<<"A child process ended" <<endl;
}
int main(int argc, char *argv[])
{
      signal(SIGCHLD, fireman);
    
        int sockfd, newsockfd, portno, clilen;
        char buffer[256];
        struct sockaddr_in serv_addr, cli_addr;
        int n;
        if (argc < 2) {
            printf("ERROR, no port provided\n");
            exit(1);
        }
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) 
            printf("ERROR opening socket");
        bzero((char *) &serv_addr, sizeof(serv_addr));
        portno = atoi(argv[1]);
        serv_addr.sin_family = AF_INET;           //family(protocol)
        serv_addr.sin_addr.s_addr = INADDR_ANY;   //ip address INADDR_ANY... can recieve request from any computer on internet
        serv_addr.sin_port = htons(portno);         //htons to make sure/convert to common internet language, (tons family)
        if (bind(sockfd, (struct sockaddr *) &serv_addr,
                sizeof(serv_addr)) < 0) 
                printf("ERROR on binding");
        listen(sockfd,5);
        clilen = sizeof(cli_addr);

      int num;
      string bin = "";
      string elian ="";
      //pid_t pid;
    while(true)
    {    
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *)&clilen);
        if (newsockfd < 0) 
            printf("ERROR on accept");
        
        if(fork()==0)
        {
            /* Begin read/write*/
            n = read(newsockfd,&num,sizeof(int));                //read1
            if (n < 0) printf("ERROR reading from socket");
            //printf("Here is the message: %d\n",num);
 
            bin = decToBin(num);                                 //generate bin
            bzero(buffer,256);                          
            for(int i=0; i< bin.size();i++)
                buffer[i] = bin.at(i);
        
            n = write(newsockfd,buffer,255);                    //write1
            if (n < 0) printf("ERROR writing to socket");
            /* End read/write*/

            /* Begin read/write*/
            n = read(newsockfd,&num,sizeof(int));               //read2
            if (n < 0) printf("ERROR reading from socket");
            //printf("**Here is the message: %d\n",num);
        
            elian =decToEli(num);                               //generate elian
            bzero(buffer,256);                        
            for(int i=0; i< elian.size();i++)
                buffer[i] = elian.at(i);
            
            n = write(newsockfd,buffer,255);                    //write2 
            if (n < 0) printf("ERROR writing to socket");
            /* End read/write*/
            _exit(0);

      }
        close(newsockfd);
    }
    close(sockfd);
   
     return 0; 
}

string decToBin(int num)  //dec to Elias gamma code
{
  long long int binary = 0;
  long long int base = 1;
  string bin="";
  while(num>0)
  {
    binary = binary + ( num % 2 ) * base;
    num = num / 2;
    base = base * 10;
  }
   
    bin = bin + std::to_string(binary);
   
   //std::cout<<"bin is "<<bin<<std::endl;
   return bin;
}
string decToEli(int num)  //dec to Elias gamma code
{
    int oNum =num;
  long long int binary = 0;
  long long int base = 1;
  string bin="";
  while(num>0)
  {
    binary = binary + ( num % 2 ) * base;
    num = num / 2;
    base = base * 10;
  }
   
   for(int i =0 ; i< floor(log2(oNum)); i++)
       bin = bin + "0";
    
    bin = bin + std::to_string(binary);
   
   //std::cout<<"bin is "<<bin<<std::endl;
   return bin;
}
