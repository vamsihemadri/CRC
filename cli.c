#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <signal.h> 
int xor(char a, char b){
    if(a=='1'&&b=='1')
        return 0;
    if(a=='1'||b=='1')
        return 1;
    return 0;
}
void my_function(int sig){
  //printf("cntrl C captured\n");//
  exit(0);
}
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, my_function);
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    char input[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
        while(1){
    printf("Please enter the message: ");
    bzero(buffer,256);
    bzero(input,256);
    fgets(input,255,stdin);
    printf("the message entered is %s\n",buffer);
    int l=strlen(input)-1;
    int i,j;
    printf("the length is %d\n",l);
    char byte[17];
    char c;
    int bit;
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
    for(i=0;i<l;i++){
        c=input[i];
        printf("the character is %c\n",c);
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

        printf("the message i am sending is %s\n",byte);

        n = write(sockfd,byte,strlen(byte));
        if (n < 0) 
             error("ERROR writing to socket");
        bzero(buffer,256);
        n = read(sockfd,buffer,255);
        if (n < 0) 
             error("ERROR reading from socket");
        //printf("the value read is %s\n",buffer);
        if(!strcmp(buffer,"ACK")){
            printf("the value read is %s\n",buffer);
            }
        else{
            printf("the value read is NACK\n");
            i=i-1;
        }
        printf("%s\n",buffer);
    }
    }
    close(sockfd);
    return 0;
}
