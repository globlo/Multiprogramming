#include <pthread.h>
#include <math.h>      
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;
#define THREADS 30
static pthread_mutex_t semB;
struct num
{
   int nb;
   //char bin_code[256];
    string bin_code;
   //char elian_code[256];
   string elian_code;
   int sockfd;
   char* hostname;
   int port;
   num(int newnum)
   {
      this->nb = newnum;
   }
};

void *thrd(void *arg)
{
   struct num *dt =(struct num *)arg;
   char buffer[256];
   		 // cout<<"thrd"<<dt->nb<<endl;
    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;
   
   int num;
    
    // if (argc < 3) {
    //    fprintf(stderr,"usage %s hostname port\n", argv[0]);
    //    exit(0);
    // }
    ///////////////////////////////////////////////////////////////////// creating socket
    portno = dt->port;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
       cout<<"ERROR opening socket"<<endl;

    server = gethostbyname(dt->hostname);
    if (server == NULL) {
       cout<<"ERROR, no such host"<<endl;
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        cout<<"ERROR connecting"<<endl;
    /////////////////////////////////////////////////////////////////////


    num =dt->nb;
    n = write(sockfd,&num,sizeof(int));             //write1
    if (n < 0) 
         cout<<"ERROR writing to socket"<<endl;

    bzero(buffer,256);
    n = read(sockfd,buffer,255);                   //read1
    if (n < 0) 
         cout<<"ERROR reading from socket"<<endl;
    //printf("number is %d, binary is %s\n",dt->nb,buffer);
   
    dt->bin_code =buffer;

   
    n = write(sockfd,&num,sizeof(int));            //write2
    if (n < 0) 
         cout<<"ERROR writing to socket"<<endl;

    bzero(buffer,256);
     n = read(sockfd,buffer,255);                  //read2
    if (n < 0) 
         cout<<"ERROR reading from socket"<<endl;
    //printf("number is %d, elian is %s\n",dt->nb,buffer);
    
    dt->elian_code = buffer;
    
   close(sockfd);
   return NULL;
}

int main(int argc, char* argv[])
{
    
    int count=0;
    string input="";
    string size="";
    num* numbers[30];
    
     getline(std::cin, size);            //get size of numbers
     int ct =atoi(size.c_str());
 


      while(getline(std::cin, input)){      //get numbers in each line & store
        numbers[count++] = new num(atoi(input.c_str()));}
                      

 // cout<<"dododod"<<endl;
        
     pthread_mutex_init(&semB, NULL);   
     pthread_t t[ct];    
     for(int i =0; i < ct; i++)   // use thread generating code
      {
          //cout<<"dododod"<< ct<<endl;
           numbers[i]->hostname = argv[1];
           numbers[i]->port = atoi(argv[2]);

        if(pthread_create(&t[i], NULL, thrd, numbers[i])) 
        {
            fprintf(stderr, "Error creating thread\n");
            return 1;
            //std::cout<<"ERRER created thread when "<<ads[i]->charac<<endl;
        }	
       // pthread_join(t[i], NULL); /*******************CHANGE*********************/
     
      }

    for(int i =0; i< count; i++)             
        pthread_join(t[i], NULL);
   
   for(int i =0; i< ct; i++) //print result
      std::cout<<"Value: "<<numbers[i]->nb<< ", Binary Code: "<<numbers[i]->bin_code
      <<", Elias-Gamma code: "<<numbers[i]->elian_code<<endl;

      //close socket?
    return 0;
}


