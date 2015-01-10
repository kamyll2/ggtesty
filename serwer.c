#include "libs.h"

struct desk_id{
int desk;
int id;
};

void childend(int signo){
wait(NULL);
}

main(int argc, char* argv[])
{
const int shmkey = 4321;
int shmid = shmget(shmkey, 10*sizeof(struct desk_id), IPC_CREAT|0600);

struct desk_id * id_tab;
id_tab = (struct desk_id *)shmat(shmid,NULL,0);
int i;
for (i=0;i<10;i++){
id_tab[i].id = -1; id_tab[i].desk = -1;
}
int id_tab_pos=0;
int id=0;

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
		write(klidesk,"4",100);
					
		//id_tab1[0].id=id;
		id_tab[0].desk=klidesk;
		printf("Zapisano klidesk %d, id_tab %d\n",klidesk,id_tab[0].desk);
		//id_tab_pos++;
	}
	else if(strcmp(buff,"login1")==0){
		write(klidesk,"5",100);
		
		id_tab[1].id=id;
		//id++;
		id_tab[1].desk=klidesk;
		//id_tab_pos++;
		printf("Zapisano klidesk %d, id_tab %d\n",klidesk,id_tab[1].desk);
			
	}
	else if(buff[0] == '4'){
		write(id_tab[0].desk, &buff, 100);
		printf("Wyslano  na klidesk id_tab0 %d\n",id_tab[0].desk);
	}
	else if(buff[0] == '5'){
		write(id_tab[1].desk, &buff, 100);
		printf("Wyslano na klidesk id_tab1 %d\n",id_tab[1].desk);
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
//else{close(klidesk);}
printf("\n");
}
close(desk);

}


