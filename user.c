#include "user.h"

int getUsersFromFile(struct User tab[], int size){
	FILE * fp;
	fp = fopen("logs.txt","r");
	int i;
	for (i=0;i<size;i++){
		if(feof(fp)!=0){break;}
		fscanf(fp, "%d\n", &tab[i].id);
		fscanf(fp, "%s\n", tab[i].login);
		fscanf(fp, "%s\n", tab[i].password);
	}
	fclose(fp);
	return i;
}

int isLoginValid(char login[], char password[], struct User tab[],int tabsize){
	int i=0;
	for (i=0;i<tabsize;i++){
		if(strcmp(login, tab[i].login)==0){
			if(strcmp(password, tab[i].password)==0){
				return tab[i].id;
				break;
			}
		}
	}
	return -1;
}

bool isLoginAvailable(char login[], struct User tab[], int tabsize){
	int i=0;
	for(i=0;i<tabsize;i++){
		if(strcmp(login, tab[i].login)==0){
			return false;
			break;
		}
	}
	return true;
}

bool registerNewUser(char login[], char password[], struct User tab[], int tabsize, int lastintab){
	lastintab--;
	if(!isLoginAvailable(login ,tab, tabsize)){return false;}
	tab[lastintab+1].id=tab[lastintab].id +1;
	strcpy(tab[lastintab+1].login,login);
	strcpy(tab[lastintab+1].password, password);
	
/*	fstream f;
	f.open("logs.txt",ios::out | ios::app);
	f<<tab[lastintab+1].id<<endl;
	f<<tab[lastintab+1].login<<endl;
	f<<tab[lastintab+1].password<<endl;*/
	FILE * fp;
	fp = fopen("logs.txt","a");
	fprintf(fp, "%d\n", tab[lastintab+1].id);
	fprintf(fp, "%s\n", tab[lastintab+1].login);
	fprintf(fp, "%s\n", tab[lastintab+1].password);	
	fclose(fp);
	return true;
}

bool isIdExist(int id, struct User tab[], int size){
	int i;
	for(i=0;i<size;i++){
		if(tab[i].id==id){
			return true;
		}
	}
	return false;
}
