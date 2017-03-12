/* A simple server in the internet domain using TCP
   The port number is passed as an argument 
   This version runs forever, forking off a separate 
   process for each connection
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h> 
int xor(char a, char b){
  if(a=='1'&&b=='1')
    return 0;
  if(a=='1'||b=='1')
    return 1;
  return 0;
}
volatile sig_atomic_t flag = 0;
void my_function(int sig){
  flag = 1; // set flag
  //printf("cntrl C captured\n");//
  exit(0);
}
void dostuff(int); /* function prototype */
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
      signal(SIGINT, my_function);
     int sockfd, newsockfd, portno, pid;
     socklen_t clilen;
     struct sockaddr_in serv_addr, cli_addr;

     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     while (1) {
         newsockfd = accept(sockfd, 
               (struct sockaddr *) &cli_addr, &clilen);
         if (newsockfd < 0) 
             error("ERROR on accept");
         pid = fork();
         if (pid < 0)
             error("ERROR on fork");
         if (pid == 0)  {
             close(sockfd);
             dostuff(newsockfd);
             //printf("i am in vamsi\n");
             exit(0);
         }
         else close(newsockfd);
     } /* end of while */
     close(sockfd);
     return 0; /* we never get here */
}
void dostuff (int sock)
{
  signal(SIGINT, my_function);
   int n,i,j;
   int bit;
   char c;
   char buffer[256];
   char divisor[10]={'1','0','0','0','0','0','1','1','1','\0'};
printf("the divisor is %s\n",divisor);
char dividend[10];
bzero(dividend,strlen(dividend));
dividend[9]='\0';
char rem[10];
bzero(rem,strlen(rem));
rem[9]='\0';
char quo[9];
bzero(quo,strlen(quo));
quo[8]='\0';
char zeros[8];
bzero(zeros,strlen(zeros));
char byte[17];
bzero(byte,strlen(byte));
   while(1){
   bzero(buffer,256);
   n = read(sock,buffer,255);
   if (n < 0) error("ERROR reading from socket");
   printf("Here is the message: %s\n",buffer);
int l=strlen(buffer);
for(i=0;i<l;i++){
        byte[i]=buffer[i];
}
        byte[16]='\0';
        int k;
        printf("the byte is %s\n",byte);
        for(k=0;k<9;k++){
          dividend[k]=byte[k];
          }
        int scratch;
        for(j=0;j<8;j++){
          //printf("the ##########dividend is %s\n",dividend);
          if(dividend[0]=='1'){
            //printf("for once i am inside this\n");
            for(k=0;k<9;k++){
              scratch=xor(dividend[k],divisor[k]);
    
              rem[k]=(scratch==1)?'1':'0';
            }
            //printf("the ##########inner remainder is %s\n",rem);
            quo[j]='1';
          }
          else{
            //printf("the ##########dividend is %s\n",dividend);
            for(k=0;k<9;k++){
                scratch=xor(dividend[k],zeros[k]);
              //printf("the scratch is %d \n",scratch);
              rem[k]=(scratch==1)?'1':'0';
            }
            //printf("the ##########inner remainder is %s\n",rem);
            quo[j]='0';
          }
          for(k=0;k<8;k++)
            dividend[k]=rem[k+1];
          //printf("the ##########dividend is %s\n",dividend);
          if(j!=8)
          dividend[8]=byte[j+9];

        }
        printf("the final remainder is %s\n",rem);
        printf("the final quotient is %s\n",quo);
        for(k=0;k<9;k++)
          byte[k+7]=(xor(byte[k+7],rem[k])==1)?'1':'0';
        //printf("the length of rem is %d\n",strlen(rem));
        int flag=0;
        for(k=0;k<strlen(rem);k++){
          if(rem[k]=='1')
            flag=1;
        }
        if(flag==1)
          n=write(sock,"NACK",1);
        else{
          n = write(sock,"ACK",3);
        }
   if (n < 0) error("ERROR writing to socket");
   if(flag){
    printf("Signal Caught\n");
    shutdown(sock,SHUT_WR);//disables further send operations.
    read(sock,buffer,256);//read if still data present.
    close(sock);//finally closing the socket.
    exit(0);
    return;
   }
}}
