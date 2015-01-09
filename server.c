#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

struct desk_id{
	int desk;
	int id;
}

main(int argc, char* argv[])
{
	struct desk_id id_tab[10];
	int id_tab_pos=0;
	int id=0;

	int on=1,klidesk,nFound,it,fdmax;
	static struct timeval timeout;
	int desk=socket(PF_INET,SOCK_STREAM,0);

	setsockopt(desk,SOL_SOCKET,SO_REUSEADDR,(char*)&on,sizeof(on));

	if(desk<0){printf("socket error\n");}
	struct sockaddr_in adres;
	struct hostent* addrent;

	fd_set mask, rmask, wmask;

	//addrent = gethostbyname(argv[1]);

	adres.sin_family=PF_INET;
	adres.sin_port=htons(1234); //port
	//adres.sin_port=htons(atoi(argv[2]));
	adres.sin_addr.s_addr=INADDR_ANY; //ip
	//memcpy(&adres.sin_addr.s_addr, addrent->h_addr, addrent->h_length);

	if(bind(desk,(struct sockaddr*)&adres,sizeof(adres))<0){
		printf("bind error\n");}

	listen(desk, 10);

	FD_ZERO(&mask);
	FD_ZERO(&rmask);
	FD_ZERO(&wmask);
	fdmax = desk;

	struct sockaddr_in klient;
	int rozm=sizeof(struct sockaddr);

	while(1){
		FD_SET(desk, &rmask);
		wmask = mask;
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;

		nFound = select(fdmax + 1, &rmask, &wmask, (fd_set*) 0, &timeout);

		if(nFound < 0){printf("Select error\n");}
		if(nFound == 0){printf("timeous\n");}

		if(FD_ISSET(desk, &rmask))
		{
			klidesk = accept(desk, (struct sockaddr*)&klient, &rozm);
			if(klidesk<0){printf("accept error\n");exit(1);}
			else{printf("jeee\n");}
			FD_SET(klidesk, &mask);
			if(klidesk > fdmax){fdmax=klidesk;}
		}

		for(it = 0; it <= fdmax; it++)
		{
			if(FD_ISSET(it, &wmask))
			{
				char buff[20];
				int n = read(it,&buff,10);
				write(1, &buff, n);
				printf("\n");
				if(strcmp(buff,"login")==0){
					//char * temp;
					//sprintf(temp,"%d",id);
					write(it,"1\n",15);
					
					id_tab[id_tab_pos].id=id;
					id++;
					id_tab[id_tab_pos].desk=it;
					id_tab_pos++;

				}
				else{
					write(it, "Hello World\n", 13);}
				FD_CLR(it, &mask);
				close(it);
			}
		}
		printf("\n");
	}
	close(desk);

}


