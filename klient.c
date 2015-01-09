#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>

int findnewline(char * buff, int size)
{
if(size == 0){return 0;}

int i;
for (i=size; i>=0;i--){
	printf("b=%c;\n",buff[i]);
	if(buff[i]=='\n'){printf("index %d\n",i);return 1;}
}
return 0;
}

main(int argc, char* argv[])
{
printf("main\n");
int desk=socket(PF_INET,SOCK_STREAM,0);
if(desk<0){printf("socket error\n");}
struct sockaddr_in adres;
struct hostent* addrent;

addrent = gethostbyname(argv[1]);

adres.sin_family=PF_INET;
//adres.sin_port=htons(13); //port
adres.sin_port=htons(atoi(argv[2]));
//adres.sin_addr.s_addr=inet_addr("150.254.32.130"); //ip
memcpy(&adres.sin_addr.s_addr, addrent->h_addr, addrent->h_length);

if(connect(desk,(struct sockaddr*)&adres,sizeof(adres))<0){
printf("connect error\n");}
char buff[100];
//int n = read(desk,&buff,25);
char * sendbuff;
sendbuff = "login";
int size = 11;
int tempsize = 0;

/*while(1)
{
	if(size == tempsize){break;}
	tempsize+= write(desk, sendbuff + tempsize, size - tempsize);
	//write(desk,sendbuff, 6);
	printf("writen %d\n",tempsize);
}*/
	write(desk, sendbuff, size);
char * recbuff;
recbuff = "                    ";

int recsize = 0;
/*while(1)
{
	if(findnewline(&buff,recsize)){printf("readbreak\n");break;}
	recsize += read(desk,&buff+recsize,30);
	if(recsize<0){printf("recsize<0\n");break;}
	printf("recwhile, %d\n",recsize);
}*/
read(desk, &buff, 30);
printf("%s readed\n", buff);
//write(1,&buff,recsize);

char  msg[100];
scanf("%s",msg);
write(desk,msg,255);
printf("%s zioom\n",msg);
printf("\n");
close(desk);

}
