#ifndef USER_H
#define USER_H

#include "libs.h"
#include <stdio.h>
#include <stdlib.h>

struct User{
int id;
char login[20];
char password[20];
};

int getUsersFromFile(struct User tab[], int size);
int isLoginValid(char login[], char password[], struct User tab[],int tabsize);
bool isLoginAvailable(char login[], struct User tab[], int tabsize);
bool registerNewUser(char login[], char password[],struct User tab[], int tabsize, int lastintab);
bool isIdExist(int id, struct User tab[], int size);
void remLine(int numline);
void saveMessage(char msg[]);
int getMessage(int id, char msg[]);
#endif
