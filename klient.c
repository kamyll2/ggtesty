#include "libs.h"


int main(int argc, char* argv[])
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

printf("login, m+wiadomosc, exit\n");

char  msg[100];
while(strcmp(buff,"Login valid")!=0){
	scanf("%s",msg);
	write(desk,msg,100);
	read(desk,buff,100);
	printf("\n\t\t\t\t%s - otrzymano od serwera\n",buff);
	printf("Podaj login:\n");
	scanf("%s",msg);
	write(desk,msg,100);
	read(desk,buff,100);
	printf("\n\t\t\t\t%s - otrzymano od serwera\n",buff);
	printf("Podaj haslo:\n");
	scanf("%s",msg);
	write(desk,msg,100);
	read(desk,buff,100);
	printf("\n\t\t\t\t%s - otrzymano od serwera\n",buff);
}
if(fork()==0)
{
	while(strcmp(buff,"exit")!=0){
	read(desk,buff,100);
	printf("\n\t\t\t\t%s - otrzymano od  serwera\n",buff);
}
exit(EXIT_SUCCESS);
}

while(strcmp(msg,"exit")!=0){
	scanf("%s",msg);
	write(desk,msg,100);
	//read(desk, buff, 100);
	//printf("%s - tako rzecze serwer\n",buff);
}
close(desk);
return 0;
}
