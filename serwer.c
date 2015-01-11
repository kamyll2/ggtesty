#include "libs.h"
#include "user.h"
#include "active.h"
/*
struct desk_id{
int desk;
int id;
bool active;
};

*/
void childend(int signo){
wait(NULL);
}

main(int argc, char* argv[])
{
const int shmkey = 4326;
int shmid = shmget(shmkey, 20*sizeof(struct Active), IPC_CREAT|0600);
struct Active * activetab;
activetab = (struct Active *)shmat(shmid,NULL,0);

int i;
for (i=0;i<20;i++){
activetab[i].id = -1; activetab[i].desk = -1; activetab[i].isLogged = false;
}
struct User usertab[20];
int maxuser = getUsersFromFile(usertab,20);

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
	char buff[100];
	int kliid,klipoz;
while(1){
	int n = read(klidesk,&buff,100);
	//write(1, &buff, n);
	printf("%s", buff);
	
	printf("\n%d odczytanych bitow\n",n);
	if(strcmp(buff,"login")==0){
		//char * temp;
		//sprintf(temp,"%d",id);
		write(klidesk,"dawaj cieciu",100);
		char login[100];
		read(klidesk,&login,100);
		write(klidesk,"mam login");
		char password[100];
		read(klidesk,&password,100);
		//write(klidesk,"mam haslo",100);
		printf("%s\t%s\n",login,password);
		kliid = isLoginValid(login,password,usertab,maxuser);
		if(kliid==-1){
			write(klidesk,"Login invalid",100);
		}
		else{
			write(klidesk,"Login valid",100);			
			/*id_tab[id_tab_pos[0]].desk=klidesk;
			id_tab[id_tab_pos[0]].id=kliid;
			id_tab_pos[0]++;*/
			char msg[100];
			int x= getMessage(kliid,msg);
			while(x==1){
				write(klidesk,msg,100);
				x=getMessage(kliid,msg);
			}
			klipoz=insertToTable(kliid,klidesk,activetab,20);
			wysactive(activetab,20);
		   printf("Zapisano klidesk %d, id_tab %d\n",klidesk,activetab[klipoz].desk);
		}
	}
	else if(strcmp(buff,"login1")==0){
		write(klidesk,"5",100);
		
		activetab[1].desk=klidesk;
		printf("Zapisano klidesk %d, id_tab %d\n",klidesk,activetab[1].desk);
			
	}
	else if((buff[0] - '0')>0 && (buff[0] - '0')<10){
		int id=buff[0] - '0';
		int tempdesk = getDesk(id,activetab,20);
		if(tempdesk==-1){
			if(!isIdExist(id,usertab,maxuser)){
				write(klidesk, "nie ma takiego usera",100);
			}
			else{
				saveMessage(buff);
				write(klidesk, "niezalogowany",100);
			}
		}
		else{
			write(klidesk, "wyslano",100);
			write(tempdesk,buff,100);
		}
		//write(id_tab[0].desk, &buff, 100);
		//printf("Wyslano  na klidesk id_tab0 %d\n",id_tab[0].desk);
	}
	/*else if(buff[0] == '5'){
		write(id_tab[1].desk, &buff, 100);
		printf("Wyslano na klidesk id_tab1 %d\n",id_tab[1].desk);
	}*/
	else if(buff[0] == 'm'){
		printf("spamer\n");
		write(klidesk, "nie spamuj", 11);
	}
	else if(strcmp(buff, "exit")==0){
		write(klidesk, "exit",100);
		printf("bye bye\n");
		logOff(kliid,activetab,20);
		wysactive(activetab,20);
		break;
		//close(klidesk);
		//exit(EXIT_SUCCESS);	
	}
	else{
		write(klidesk, "nieznany wzorzec", 100);
	}
	}
close(klidesk);
exit(EXIT_SUCCESS);

}
//else{close(klidesk);}
printf("\n");
}
close(desk);

}


