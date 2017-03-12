#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int xor(char a, char b){
	if(a=='1'&&b=='1')
		return 0;
	if(a=='1'||b=='1')
		return 1;
	return 0;
}
void main(int argc,char *argv[])
{
char c='a';
printf("%c\n",c);
int i,j;
char byte[17];
char f='1';
int g=1;
//sprintf((char *)byte[0],"%d",g);
//printf("#######%s\n",byte);
//for(i=0;i<8;i++){
//	j=!!((c<<i)&0x80);
//	byte[i]=(char)j;
//	printf("%d",!!j);
//}
bzero(byte,strlen(byte));
char buffer[256];
printf("\nenter the message\n");
bzero(buffer,256);
fgets(buffer,255,stdin);
printf("the message entered is %s",buffer);
int l=strlen(buffer)-1;
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
        c=buffer[i];
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
    }
if(!strcmp(buffer,"ACK"))
	printf("hello tehre\n");
l=strlen(byte);
printf("the length of buffer is %d\n",l);
strcpy(buffer,"vamsi");
l=strlen(buffer);
printf("the length of buffer is %d\n",l);
}
