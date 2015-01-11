#ifndef ACTIVE_H
#define ACTIVE_H
#include "libs.h"

struct Active{
int desk;
int id;
bool isLogged;
};

void wysactive(struct Active tab[], int size);
int getDesk(int id, struct Active tab[], int size);
int insertToTable(int id, int desk, struct Active tab[], int size);
int isNowLogged(int id, struct Active tab[], int size);
bool logOff(int id,struct Active tab[], int size);
#endif
