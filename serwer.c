#include "libs.h"
#include "user.h"
#include "active.h"
#include "parser.h"
#include <pthread.h>

#define ACTIVESIZE  20
#define USERSIZE  20

struct Active activetab[ACTIVESIZE];
struct User  usertab[USERSIZE];
int maxuser;

Message serMes;

struct cln{
	int cfd;
	struct sockaddr_in caddr;
};

void *  watek(void * arg){

	struct cln* c = (struct cln*)arg;
	int klidesk = c->cfd;
	char buff[100];
	int kliid,klipoz;
	Message kliMes;
	while(1){
		int n = read(klidesk,buff,100);
		if(n<1){printf("read error\n");return 0;}
		//write(1, &buff, n);
		printf("%s", buff);
		kliMes = parseCharToMessage(buff);
		printf("\n%d odczytanych bitow\n",n);
		if(kliMes.type==1){
			//char * temp;
			//sprintf(temp,"%d",id);
			/*write(klidesk,"dawaj cieciu",100);
			char login[100];
			read(klidesk,&login,100);
			write(klidesk,"mam login",100);
			char password[100];
			read(klidesk,&password,100);
			//write(klidesk,"mam haslo",100);
			printf("%s\t%s\n",login,password);*/
			kliid = isLoginValid(kliMes.login,kliMes.password,usertab,maxuser);
			if(kliid==-1){
				write(klidesk,"<type>21<body>Invalid Login<end>",100);
			}
			else{
				write(klidesk,"<type>11<body>Login valid<end>",100);			
				/*id_tab[id_tab_pos[0]].desk=klidesk;
				id_tab[id_tab_pos[0]].id=kliid;
				id_tab_pos[0]++;*/
				char msg[100];
				int x= getMessage(kliid,msg);
				while(x==1){
					write(klidesk,msg,100);
					x=getMessage(kliid,msg);
				}
				klipoz=insertToTable(kliid,klidesk,activetab,ACTIVESIZE);
				wysactive(activetab,ACTIVESIZE);
		   printf("Zapisano klidesk %d, id_tab %d\n",klidesk,activetab[klipoz].desk);
			}
		}
		else if(kliMes.type==2){
			/*write(klidesk,"dawaj cieciu",100);
			char login[100];
			read(klidesk,&login,100);
			write(klidesk,"mam login",100);
			char password[100];
			read(klidesk,&password,100);
			//write(klidesk,"mam haslo",100);
			printf("%s\t%s\n",login,password);*/	
			if(!isLoginAvailable(kliMes.login,usertab,maxuser)){
				write(klidesk, "<type>4<body>Login already in use<end>",100);
			}
			else{
				registerNewUser(kliMes.login, kliMes.password, usertab, USERSIZE, maxuser);
				maxuser++;
				write(klidesk, "<type>4<body>Registration success!<end>", 100);
			}	
		}
		else if(kliMes.type==0){
			int id=buff[0] - '0';
			int tempdesk = getDesk(kliMes.to,activetab,ACTIVESIZE);
			if(tempdesk==-1){
				if(!isIdExist(kliMes.to,usertab,maxuser)){
					write(klidesk, "<type>6<body>Not found user with this ID<end>",100);
				}
				else{
					saveMessage(buff);
					//write(klidesk, "niezalogowany",100);
				}
			}
			else{
				//write(klidesk, "wyslano",100);
				write(tempdesk,buff,100);
			}
			//write(id_tab[0].desk, &buff, 100);
			//printf("Wyslano  na klidesk id_tab0 %d\n",id_tab[0].desk);
		}
		/*else if(buff[0] == 'm'){
			printf("spamer\n");
			write(klidesk, "nie spamuj", 11);
		}*/
		else if(kliMes.type==7){
			write(klidesk, "<type>8<body>Logout success!<end>",100);
			printf("bye bye\n");
			logOff(kliid,activetab,ACTIVESIZE);
			wysactive(activetab,ACTIVESIZE);
			break;
		//close(klidesk);
		//exit(EXIT_SUCCESS);	
		}
	else{
		write(klidesk, "<type>9<body>Something went wrong<end>", 100);
	}
}
return 0;
close(klidesk);
}

int main(int argc, char* argv[])
{
pthread_t tid;
socklen_t slt;

/*const int shmkey = 4326;
int shmid = shmget(shmkey, 20*sizeof(struct Active), IPC_CREAT|0600);
activetab = (struct Active *)shmat(shmid,NULL,0);
*/
int i;
for (i=0;i<ACTIVESIZE;i++){
	activetab[i].id = -1; activetab[i].desk = -1; activetab[i].isLogged = false;
}
/*
int shmid2 = shmget(5326, 20*sizeof(struct User),IPC_CREAT|0600);
usertab = (struct User *)shmat(shmid2,NULL,0);
*/
/*int shmid3 = shmget(5327, sizeof(int), IPC_CREAT|0600);
maxuser = (int *)shmat(shmid3,NULL,0);*/
maxuser = getUsersFromFile(usertab,USERSIZE);

int on=1;
int desk=socket(PF_INET,SOCK_STREAM,0);
setsockopt(desk,SOL_SOCKET,SO_REUSEADDR,(char*)&on,sizeof(on));

if(desk<0){printf("socket error\n");}
struct sockaddr_in adres;
struct hostent* addrent;

//addrent = gethostbyname(argv[1]);
adres.sin_family=PF_INET;
adres.sin_port=htons(1234); //port
//adres.sin_port=htons(atoi(argv[2]));
adres.sin_addr.s_addr=INADDR_ANY; //ip
//memcpy(&adres.sin_addr.s_addr, addrent->h_addr, addrent->h_length);

if(bind(desk,(struct sockaddr*)&adres,sizeof(adres))<0){
	printf("bind error\n");
}

listen(desk, 10);

struct sockaddr_in klient;
int rozm=sizeof(struct sockaddr_in);
while(1){
	struct cln* c = malloc(sizeof(struct cln));
	slt = sizeof(c->caddr);

	c->cfd=accept(desk,(struct sockaddr*)&klient,&rozm);

	pthread_create(&tid, NULL, watek, c);
	pthread_detach(tid);

	printf("\n");
}
close(desk);

return 0;
}
