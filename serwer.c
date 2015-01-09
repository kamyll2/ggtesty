#include "libs.h"

void childend(int signo){
wait(NULL);
}

main(int argc, char* argv[])
{
int on=1;
int desk=socket(PF_INET,SOCK_STREAM,0);
setsockopt(desk,SOL_SOCKET,SO_REUSEADDR,(char*)&on,sizeof(on));

if(desk<0){printf("socket error\n");}
struct sockaddr_in adres;
struct hostent* addrent;

signal(SIGCHLD, childend);

//addrent = gethostbyname(argv[1]);

adres.sin_family=PF_INET;
adres.sin_port=htons(1234); //port
//adres.sin_port=htons(atoi(argv[2]));
adres.sin_addr.s_addr=INADDR_ANY; //ip
//memcpy(&adres.sin_addr.s_addr, addrent->h_addr, addrent->h_length);

if(bind(desk,(struct sockaddr*)&adres,sizeof(adres))<0){
printf("bind error\n");}

listen(desk, 10);

struct sockaddr_in klient;
int rozm=sizeof(struct sockaddr_in);
while(1){
int klidesk;
klidesk=accept(desk,(struct sockaddr*)&klient,&rozm);
if(fork()==0){
	close(desk);
while(1){
	char buff[100];
	int n = read(klidesk,&buff,100);
	//write(1, &buff, n);
	printf("%s", buff);
	
	printf("\n%d odczytanych bitow\n",n);
	if(strcmp(buff,"login")==0){
		//char * temp;
		//sprintf(temp,"%d",id);
		write(klidesk,"1",100);
					
		//id_tab[id_tab_pos].id=id;
		//id++;
		//id_tab[id_tab_pos].desk=it;
		//id_tab_pos++;
	}
	else if(buff[0] == 'm'){
		printf("spamer\n");
		write(klidesk, "nie spamuj", 11);
	}
	else if(strcmp(buff, "exit")==0){
		write(klidesk, "exit",100);
		printf("bye bye\n");
		break;
		//close(klidesk);
		//exit(EXIT_SUCCESS);	
	}
	else{
		write(klidesk, "Hello World\n", 13);}
	}
close(klidesk);
exit(EXIT_SUCCESS);

}
else{close(klidesk);}
printf("\n");
}
close(desk);

}


