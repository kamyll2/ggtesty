#include "active.h"

void wysactive(struct Active tab[], int size){
	int i;
	printf("i\tID\tDESK\n");
	for(i=0;i<size;i++){
		if(tab[i].isLogged==true){
			printf("%d\t%d\t%d\n",i,tab[i].id,tab[i].desk);
		}
	}
}

int getDesk(int id, struct Active tab[], int size){
	int i;
	for(i=0;i<size;i++){
		if(tab[i].isLogged==true){
			if(tab[i].id == id){
				return tab[i].desk;
			}
		}
	}
	return -1;
}

int insertToTable(int id, int desk, struct Active tab[], int size){
	int i;
	for(i=0;i<size;i++){
		if(tab[i].isLogged==false){
			tab[i].isLogged=true;
			tab[i].id=id;
			tab[i].desk=desk;
			return i;
		}
	}
	return -1;
}

int isNowLogged(int id, struct Active tab[], int size){
	int i;
	for(i=0;i<size;i++){
		if(tab[i].isLogged==true && tab[i].id==id){
			return i;
		}
	}
	return -1;
}

bool logOff(int id,struct Active tab[], int size){
	tab[isNowLogged(id,tab,size)].isLogged=false;
}

