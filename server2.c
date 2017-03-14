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
#include <time.h>

int errval;
void CRC(char byte[17],char rem[9],char dividend[17],char divisor[10],char *quo,char *zeros);
void CRC_gen(char input,char byte[17],char rem[9],char dividend[17],char divisor[10],char *quo,char *zeros);
   int sfd=-1;
   int poll=-1;

   int clientnum=0;


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
  if(poll!=-1)
  {
    write(poll,"~",1);
    close(poll);
  }
  if(sfd!=-1)
    close(sfd);
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
   srand(time(NULL));
      float prob;
      printf("Enter BER: ");
      scanf("%f",&prob);
      errval = prob*RAND_MAX;
      if(errval<0)
        errval = RAND_MAX;
      //printf("%lf = errval\n",errval);
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
      sfd = sockfd;
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
        clientnum++;
         pid = fork();
         if (pid < 0)
             error("ERROR on fork");
         if (pid == 0)  {
             close(sockfd);
             sfd = -1;
             dostuff(newsockfd);
             //printf("i am in vamsi\n");
             exit(0);
         }
         else {sfd=sockfd;close(newsockfd);}
     } /* end of while */
     close(sockfd);
     return 0; /* we never get here */
}
void dostuff (int sock)
{
  signal(SIGINT, my_function);
  poll=sock;
   int n,i,j;
   int bit;
   char c;
   char buffer[256];
   char divisor[10]={'1','0','0','0','0','0','1','1','1','\0'};
//printf("the divisor is %s\n",divisor);
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
//printf("Client %d: ",clientnum);
while(1){
 bzero(buffer,256);
  n = read(sock,buffer,255);
 if (n < 0) error("ERROR reading from socket");
   //printf("Here is the message: %s\n",buffer);
if(buffer[0] == '~')
{
  printf("Client %d closed the connection\n",clientnum);
  close(sock);
  exit(0);
}
int l=strlen(buffer);
bzero(byte,strlen(byte));
for(i=0;i<l;i++){
        byte[i]=buffer[i];
}
        byte[16]='\0';
        int k;
       // printf("the byte is %s\n",byte);
        for(k=0;k<9;k++){
          dividend[k]=byte[k];
          }
        CRC(byte,rem,dividend,divisor,quo,zeros);
        int flag=0;
        for(k=0;k<strlen(rem);k++){
          if(rem[k]=='1')
            flag=1;
        }
        //sleep(6);
        if(flag==1)
        {
          CRC_gen('&',byte,rem,dividend,divisor,quo,zeros);
          for(k=0;k<16;k++)
          {
            if(rand()<=errval)
              if(byte[k]=='1')
                byte[k] = '0';
              else
                byte[k]='1';
          }
          n=write(sock,byte,strlen(byte));
        }
        else{
          CRC_gen('$',byte,rem,dividend,divisor,quo,zeros);
          char rec;
          rec = 0;
          for(k=0;k<8;k++)
          {
            if(buffer[k] == '1'){
              rec = rec + 1;
            }
            if(k<7)
            rec = rec << 1;
          }
          //rec = rec >> 1;
          printf("client %d: %c\n",clientnum,rec);
          for(k=0;k<16;k++)
          {
             if(rand()<errval)
               if(byte[k]=='1')
                 byte[k] = '0';
               else
                 byte[k]='1';
          }
          n = write(sock,byte,strlen(byte));
        }
   if (n < 0) error("ERROR writing to socket");
}
}
void CRC(char byte[17],char rem[9],char dividend[17],char divisor[10],char *quo,char *zeros) {
        int k,j;
        //printf("the byte is %s\n",byte);
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
        //printf("the final remainder is %s\n",rem);
        //printf("the final quotient is %s\n",quo);
        for(k=0;k<9;k++)
            byte[k+7]=(xor(byte[k+7],rem[k])==1)?'1':'0';

        //printf("the message i am sending is %s\n",byte);
}
void CRC_gen(char input,char byte[17],char rem[9],char dividend[17],char divisor[10],char *quo,char *zeros) {
    char c;
    c=input;
    int bit,j,i;
        //printf("the character is %c\n",c);
        bzero(byte,17);
        for(j=0;j<8;j++){
            bit=!!((c<<j)&0x80);
            if(bit==1)
                byte[j]='1';
            else
                byte[j]='0';
        }
            for(j=8;j<16;j++)
                byte[j]='0';
        byte[16]='\0';
        int k;
        //printf("the byte is %s\n",byte);
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
        //printf("the final remainder is %s\n",rem);
        //printf("the final quotient is %s\n",quo);
        for(k=0;k<9;k++)
            byte[k+7]=(xor(byte[k+7],rem[k])==1)?'1':'0';

        //printf("the message i am sending is %s\n",byte);
}