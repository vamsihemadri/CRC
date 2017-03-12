#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <signal.h> 
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
    char byte[9];
    char c;
    int bit;
for(i=0;i<l;i++){
        c=input[i];
        printf("the character is %c\n",c);
        bzero(byte,9);
        for(j=0;j<8;j++){
            bit=!!((c<<j)&0x80);
            if(bit==1)
                byte[j]='1';
            else
                byte[j]='0';
        }
        byte[9]='\0';
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
